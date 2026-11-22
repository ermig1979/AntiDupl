/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <math.h>
#include <algorithm>
#include <chrono>
#include <fstream>

// libjxl's Butteraugli, compiled into the static jxl.lib, with headers the
// overlay port src/ports/libjxl installs. Its headers turn their debug
// checks on unless NDEBUG is defined, which this project's release builds
// don't define: the inline functions compiled here would then differ from
// the ones in vcpkg's release jxl.lib. NDEBUG follows the runtime library,
// as vcpkg's choice between its debug and release libraries does.
#if !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#define AD_JXL_NDEBUG
#endif
#include "lib/jxl/butteraugli/butteraugli.h"
#include "lib/jxl/memory_manager_internal.h"
#ifdef AD_JXL_NDEBUG
#undef NDEBUG
#undef AD_JXL_NDEBUG
#endif

#include "adButteraugli.h"
#include "adDataCollector.h"
#include "adEngine.h"
#include "adImage.h"
#include "adImageData.h"
#include "adOptions.h"
#include "adStatus.h"

namespace ad
{
    // The stored scores' file: this signature, the format and score
    // versions, the record count, then TKey and score pairs.
    static const TUInt32 SCORE_FILE_SIGNATURE = 0x53424441; // "ADBS"
    static const TUInt32 SCORE_FILE_FORMAT = 1;

    // Butteraugli wants linear light, 1.0 for white, which it takes to be
    // shown at BUTTERAUGLI_INTENSITY_TARGET.
    static const float * LinearLut()
    {
        static const std::vector<float> lut = []
        {
            std::vector<float> lut(256);
            for (int i = 0; i < 256; ++i)
            {
                const double c = i / 255.0;
                lut[i] = float(c <= 0.04045 ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4));
            }
            return lut;
        }();
        return lut.data();
    }

    // libjxl's default allocator; its images take one.
    static JxlMemoryManager * MemoryManager()
    {
        static JxlMemoryManager manager = []
        {
            JxlMemoryManager manager;
            if (!jxl::MemoryManagerInit(&manager, nullptr))
                manager = JxlMemoryManager{};
            return manager;
        }();
        return &manager;
    }

    // The turn TImageComparer::Accept gives its reduced images: a mirror
    // (TPixelData::Mirror) for the MIRROR_ ones, then a quarter turn
    // counterclockwise (TPixelData::Turn) per 90 degrees.
    static void Transform(const TView & src, TView & dst, adTransformType transform)
    {
        const bool mirror = transform >= AD_TRANSFORM_MIRROR_TURN_0;
        const int turns = int(transform) % 4;
        const size_t w = src.width, h = src.height;
        dst.Recreate(turns % 2 ? h : w, turns % 2 ? w : h, TView::Bgr24);
        for (size_t y = 0; y < h; ++y)
        {
            const TUInt8 * p = src.data + y * src.stride;
            for (size_t x = 0; x < w; ++x, p += 3)
            {
                size_t tx = mirror ? w - 1 - x : x, ty = y, tw = w, th = h;
                for (int i = 0; i < turns; ++i)
                {
                    const size_t nx = ty, ny = tw - 1 - tx;
                    tx = nx; ty = ny;
                    std::swap(tw, th);
                }
                TUInt8 * q = dst.data + ty * dst.stride + tx * 3;
                q[0] = p[0]; q[1] = p[1]; q[2] = p[2];
            }
        }
    }

    static adTransformType Inverse(adTransformType transform)
    {
        if (transform == AD_TRANSFORM_TURN_90)
            return AD_TRANSFORM_TURN_270;
        if (transform == AD_TRANSFORM_TURN_270)
            return AD_TRANSFORM_TURN_90;
        return transform; // a half turn, and every mirrored one, undo themselves
    }

    // The order a pair is scored in, whichever image the search met first.
    static bool Before(const TImageData * a, const TImageData * b)
    {
        if (a->crc32c != b->crc32c)
            return a->crc32c < b->crc32c;
        return a->size < b->size;
    }

    static void ToLinear(const TView & bgr, jxl::Image3F & rgb)
    {
        const float * lut = LinearLut();
        for (size_t y = 0; y < bgr.height; ++y)
        {
            const TUInt8 * p = bgr.data + y * bgr.stride;
            float * r = rgb.PlaneRow(0, y), * g = rgb.PlaneRow(1, y), * b = rgb.PlaneRow(2, y);
            for (size_t x = 0; x < bgr.width; ++x, p += 3)
            {
                b[x] = lut[p[0]];
                g[x] = lut[p[1]];
                r[x] = lut[p[2]];
            }
        }
    }

    // Butteraugli's map of two B, G, R views of the same size, the first
    // taken as the reference; fails only when memory runs out.
    static jxl::Status Diffmap(const TView & first, const TView & second, jxl::ImageF & diffmap)
    {
        JXL_ASSIGN_OR_RETURN(jxl::Image3F rgb0, jxl::Image3F::Create(MemoryManager(), first.width, first.height));
        JXL_ASSIGN_OR_RETURN(jxl::Image3F rgb1, jxl::Image3F::Create(MemoryManager(), second.width, second.height));
        ToLinear(first, rgb0);
        ToLinear(second, rgb1);
        jxl::ButteraugliParams params;
        params.intensity_target = BUTTERAUGLI_INTENSITY_TARGET;
        return jxl::ButteraugliDiffmap(rgb0, rgb1, params, diffmap);
    }

    // Keypoints from a picture's grey pixels at full size, on a copy of
    // about the level's area.
    static void ImageKeypoints(const TView & gray, const TAlignLevel & level, TKeypoints & keypoints)
    {
        const double scale = std::min(1.0, sqrt(double(level.area) / (double(gray.width) * gray.height)));
        const size_t w = std::max<size_t>(1, size_t(gray.width * scale + 0.5));
        const size_t h = std::max<size_t>(1, size_t(gray.height * scale + 0.5));
        TView copy(w, h, TView::Gray8);
        AreaResize(gray, copy);
        FindKeypoints(copy.data, w, h, copy.stride, level.keypointsMax, level.pyramidLevels, keypoints);
    }

    // The most memory the keypoints of so many images take at a level,
    // with their index.
    static size_t LevelBytes(const TAlignLevel & level, size_t imageCount)
    {
        return imageCount * (ALIGN_IMAGE_OVERHEAD_BYTES + level.keypointsMax * TKeypoints::BYTES_PER_POINT) +
            TKeypointIndex::Bytes(imageCount, level.keypointsMax);
    }

    // Whether two pictures, by their keypoints' transform from the second to
    // the first, share enough of the smaller one to compare.
    static bool EnoughOverlap(const TKeypoints & first, const TKeypoints & second, const TAffine & transform)
    {
        const TBox box = OverlapBox(transform, second.width, second.height, first.width, first.height);
        const double area = double(box.Width()) * box.Height();
        const double secondShare = area / (double(second.width) * second.height);
        const double firstShare = area * Simd::Square(transform.Scale()) / (double(first.width) * first.height);
        return std::max(firstShare, secondShare) >= ALIGN_OVERLAP_MIN;
    }
    //-------------------------------------------------------------------------
    bool TButteraugliStore::TKey::operator==(const TKey & key) const
    {
        return memcmp(this, &key, sizeof(TKey)) == 0;
    }

    size_t TButteraugliStore::TKeyHash::operator()(const TKey & key) const
    {
        const TUInt32 * p = (const TUInt32 *)&key;
        size_t hash = 0;
        for (size_t i = 0; i < sizeof(TKey) / sizeof(TUInt32); ++i)
            hash = hash * 0x9E3779B1u + p[i];
        return hash;
    }
    //-------------------------------------------------------------------------
    TButteraugliStore::TButteraugliStore(TEngine *pEngine, size_t imageCount)
        : m_pOptions(pEngine->Options())
        , m_pEngine(pEngine)
        , m_alignedMemory()
        , m_pLevel(NULL)
        , m_participants(0)
        , m_arrived(0)
        , m_indexBuilt(false)
        , m_path(pEngine->UserPath() + TEXT("\\butteraugli.adp"))
        , m_imageBytes(0)
        , m_changed(false)
    {
        static_assert(sizeof(TKey) == 40, "TKey is written to the file as it is");
        if (m_pOptions->compare.algorithmComparing == AD_COMPARING_BUTTERAUGLI_ALIGNED)
        {
            ChooseLevel(imageCount);
            m_pIndex.reset(new TKeypointIndex(imageCount, ALIGN_INDEX_KEY_ENTRIES_MAX, ALIGN_INDEX_HITS_MIN));
        }
        Load();
    }

    void TButteraugliStore::ChooseLevel(size_t imageCount)
    {
        MEMORYSTATUSEX status = { sizeof(status) };
        size_t room = 0;
        if (::GlobalMemoryStatusEx(&status))
        {
            const size_t reserve = std::max(ALIGN_MEMORY_RESERVE_BYTES,
                size_t(double(status.ullTotalPhys) * ALIGN_MEMORY_RESERVE_SHARE));
            const size_t taken = reserve + BUTTERAUGLI_IMAGE_CACHE_BYTES;
            // Memory is allocated against the commit limit (the memory and
            // the page file together), which other programs can have taken
            // more of than they use: what is free there can be less.
            const size_t free = size_t(std::min(status.ullAvailPhys, status.ullAvailPageFile));
            room = free > taken ? free - taken : 0;
        }
        const TInt32 setting = m_pOptions->compare.alignedLevel;
        const size_t wanted = setting >= 1 && size_t(setting) <= ALIGN_LEVEL_COUNT ? size_t(setting) : ALIGN_LEVEL_COUNT;
        size_t used = wanted;
        if (setting == 0)
            while (used > 1 && LevelBytes(ALIGN_LEVELS[used - 1], imageCount) > room)
                --used;
        m_pLevel = &ALIGN_LEVELS[used - 1];
        m_alignedMemory.wanted = adInt32(wanted);
        m_alignedMemory.used = adInt32(used);
        m_alignedMemory.wantedBytes = LevelBytes(ALIGN_LEVELS[wanted - 1], imageCount);
        m_alignedMemory.usedBytes = LevelBytes(*m_pLevel, imageCount);
        m_alignedMemory.freeBytes = room;
        m_alignedMemory.imageCount = imageCount;
    }

    TButteraugliStore::~TButteraugliStore()
    {
        if (m_changed)
            Save();
    }

    double TButteraugliStore::Score(TImageDataPtr pFirst, TImageDataPtr pSecond, adTransformType transform)
    {
        if (Before(pSecond, pFirst))
        {
            std::swap(pFirst, pSecond);
            transform = Inverse(transform);
        }
        TKey key = { pFirst->size, pSecond->size, pFirst->crc32c, pSecond->crc32c, transform,
            m_pOptions->compare.butteraugliSize, m_pOptions->advanced.ignoreFrameWidth, 0 };
        // 0 is a CRC not computed, and -1 one the file couldn't be read for.
        const bool keyed = pFirst->crc32c != 0 && pFirst->crc32c != TUInt32(-1) &&
            pSecond->crc32c != 0 && pSecond->crc32c != TUInt32(-1);
        if (keyed)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_scores.find(key);
            if (it != m_scores.end())
                return it->second;
        }

        TWorkImagePtr first = Image(pFirst), second = Image(pSecond);
        if (!first->bgr || !second->bgr)
            return -1;
        const double score = Compute(*first->bgr, *second->bgr, transform);

        if (keyed && score >= 0)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_scores[key] = score;
            m_changed = true;
        }
        return score;
    }

    void TButteraugliStore::SetKeypoints(TImageDataPtr pImageData, const TView & gray)
    {
        std::shared_ptr<TKeypoints> keypoints = std::make_shared<TKeypoints>();
        ImageKeypoints(gray, *m_pLevel, *keypoints);
        std::lock_guard<std::mutex> lock(m_keypointMutex);
        m_keypoints[pImageData] = keypoints;
    }

    std::shared_ptr<const TKeypoints> TButteraugliStore::Keypoints(TImageDataPtr pImageData)
    {
        std::lock_guard<std::mutex> lock(m_keypointMutex);
        auto it = m_keypoints.find(pImageData);
        return it != m_keypoints.end() ? it->second : NULL;
    }

    void TButteraugliStore::Join()
    {
        std::lock_guard<std::mutex> lock(m_indexMutex);
        ++m_participants;
    }

    void TButteraugliStore::Queue(TImageDataPtr pImageData)
    {
        const std::shared_ptr<const TKeypoints> keypoints = Keypoints(pImageData);
        if (!keypoints || keypoints->Size() == 0 || !m_pIndex)
            return;
        std::lock_guard<std::mutex> lock(m_indexMutex);
        m_indexed.push_back(pImageData);
    }

    bool TButteraugliStore::Gather()
    {
        std::unique_lock<std::mutex> lock(m_indexMutex);
        if (!m_pIndex)
            return false;
        if (++m_arrived == m_participants)
        {
            // A key full of images stops counting, so the images in the
            // index when one is looked up decide its candidates: all of
            // them, added in an order that doesn't depend on the threads.
            std::sort(m_indexed.begin(), m_indexed.end(), [](TImageDataPtr a, TImageDataPtr b)
                { return TPath::LesserByPath(a->path, b->path); });
            std::vector<std::shared_ptr<const TKeypoints>> keypoints(m_indexed.size());
            for (size_t i = 0; i < m_indexed.size(); ++i)
            {
                keypoints[i] = Keypoints(m_indexed[i]);
                m_number[m_indexed[i]] = uint32_t(i);
            }
            m_pIndex->Build(keypoints.size(), [&](size_t i) -> const TKeypoints & { return *keypoints[i]; });
            m_indexBuilt = true;
            m_gathered.notify_all();
            return true;
        }
        while (!m_indexBuilt)
        {
            if (m_pEngine->Status()->Stopped())
                return false;
            m_gathered.wait_for(lock, std::chrono::milliseconds(200));
        }
        return true;
    }

    void TButteraugliStore::Candidates(TImageDataPtr pImageData, std::vector<TImageDataPtr> & candidates)
    {
        candidates.clear();
        const std::shared_ptr<const TKeypoints> keypoints = Keypoints(pImageData);
        if (!keypoints || keypoints->Size() == 0 || !m_pIndex)
            return;
        std::vector<uint32_t> found;
        std::lock_guard<std::mutex> lock(m_indexMutex);
        auto self = m_number.find(pImageData);
        if (!m_indexBuilt || self == m_number.end())
            return;
        m_pIndex->Find(*keypoints, found);
        for (const uint32_t image : found)
            if (image != self->second)
                candidates.push_back(m_indexed[image]);
    }

    bool TButteraugliStore::Claim(TImageDataPtr pFirst, TImageDataPtr pSecond)
    {
        std::lock_guard<std::mutex> lock(m_indexMutex);
        const uint64_t a = m_number[pFirst], b = m_number[pSecond];
        return m_claimed.insert(a < b ? (a << 32) | b : (b << 32) | a).second;
    }

    // From pixels of a picture of one size to those of another: pixel
    // centres correspond, not pixel corners.
    static TAffine Rescale(size_t fromWidth, size_t fromHeight, size_t toWidth, size_t toHeight)
    {
        const double sx = double(toWidth) / fromWidth, sy = double(toHeight) / fromHeight;
        TAffine t = { sx, 0, 0.5 * sx - 0.5, 0, sy, 0.5 * sy - 0.5 };
        return t;
    }

    double TButteraugliStore::AlignedScore(TImageDataPtr pFirst, TImageDataPtr pSecond)
    {
        if (Before(pSecond, pFirst))
            std::swap(pFirst, pSecond);
        TKey key = { pFirst->size, pSecond->size, pFirst->crc32c, pSecond->crc32c, AD_TRANSFORM_TURN_0,
            m_pOptions->compare.butteraugliSize, m_pOptions->advanced.ignoreFrameWidth, ALIGNED_SCORE_VERSION };
        const bool keyed = pFirst->crc32c != 0 && pFirst->crc32c != TUInt32(-1) &&
            pSecond->crc32c != 0 && pSecond->crc32c != TUInt32(-1);
        if (keyed)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_scores.find(key);
            if (it != m_scores.end())
                return it->second;
        }

        // Most candidates end here, and matching them again costs less than
        // keeping them all in the scores' file.
        const std::shared_ptr<const TKeypoints> firstKeypoints = Keypoints(pFirst), secondKeypoints = Keypoints(pSecond);
        TAffine transform;
        if (!firstKeypoints || !secondKeypoints || !FindTransform(*secondKeypoints, *firstKeypoints, transform, NULL))
            return -1;
        const TKeypoints & a = *firstKeypoints, & b = *secondKeypoints;
        if (!EnoughOverlap(a, b, transform))
            return -1;

        TWorkImagePtr first = Image(pFirst), second = Image(pSecond);
        if (!first->bgr || !second->bgr)
            return -1;
        const double score = ComputeAligned(*first->bgr, *second->bgr,
            Rescale(a.width, a.height, first->bgr->width, first->bgr->height).After(transform).After(
                Rescale(second->bgr->width, second->bgr->height, b.width, b.height)));

        if (keyed)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_scores[key] = score;
            m_changed = true;
        }
        return score;
    }

    TButteraugliStore::TWorkImagePtr TButteraugliStore::Image(TImageDataPtr pImageData)
    {
        TWorkImagePtr image;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_images.find(pImageData);
            if (it != m_images.end())
            {
                image = it->second.image;
                m_uses.splice(m_uses.begin(), m_uses, it->second.use);
            }
            else
            {
                image = std::make_shared<TWorkImage>();
                m_uses.push_front(pImageData);
                m_images[pImageData] = TCached{ image, m_uses.begin() };
            }
        }

        // Another thread asking for the same image waits for this decode.
        std::call_once(image->decoded, [&] { Decode(pImageData, *image); });

        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_images.find(pImageData);
        if (image->bgr && !image->counted && it != m_images.end() && it->second.image == image)
        {
            image->counted = true;
            m_imageBytes += image->bgr->stride * image->bgr->height;
        }
        // The two images of the pair being scored stay; the shared pointers
        // keep an evicted image alive while a thread still scores with it.
        while (m_imageBytes > BUTTERAUGLI_IMAGE_CACHE_BYTES && m_uses.size() > 2)
        {
            auto victim = m_images.find(m_uses.back());
            if (victim->second.image->counted)
                m_imageBytes -= victim->second.image->bgr->stride * victim->second.image->bgr->height;
            m_images.erase(victim);
            m_uses.pop_back();
        }
        return image;
    }

    void TButteraugliStore::Decode(TImageDataPtr pImageData, TWorkImage & image) const
    {
        std::unique_ptr<TImage> pImage(TImage::Load(pImageData->path.Original().c_str(), m_pOptions));
        if (!pImage || !pImage->View())
            return;
        const TView & view = *pImage->View();
        if (view.format != TView::Bgra32 && view.format != TView::Rgba32 && view.format != TView::Rgb24 &&
            view.format != TView::Bgr24 && view.format != TView::Gray8)
            return;

        size_t w = view.width, h = view.height;
        const size_t side = (size_t)m_pOptions->compare.butteraugliSize;
        TView reduced;
        const TView * pSrc = &view;
        if (std::max(w, h) > side)
        {
            const double scale = double(side) / std::max(w, h);
            w = std::max<size_t>(1, size_t(w * scale + 0.5));
            h = std::max<size_t>(1, size_t(h * scale + 0.5));
            reduced.Recreate(w, h, view.format);
            AreaResize(view, reduced);
            pSrc = &reduced;
        }

        image.bgr.reset(new TView(w, h, TView::Bgr24));
        TView & bgr = *image.bgr;
        const TView & src = *pSrc;
        switch (src.format)
        {
        case TView::Bgra32:
            SimdBgraToBgr(src.data, w, h, src.stride, bgr.data, bgr.stride);
            break;
        case TView::Rgba32:
            SimdBgraToRgb(src.data, w, h, src.stride, bgr.data, bgr.stride); // swaps red and blue
            break;
        case TView::Rgb24:
            SimdBgrToRgb(src.data, w, h, src.stride, bgr.data, bgr.stride); // swaps red and blue
            break;
        case TView::Bgr24:
            Simd::Copy(src, bgr);
            break;
        default:
            SimdGrayToBgr(src.data, w, h, src.stride, bgr.data, bgr.stride);
            break;
        }
    }

    // Both images at the size of the one with fewer pixels, the other one
    // area-resized to it and taken as the reference, and both cut to what
    // Ignore frame width leaves: compared without the frame, not only
    // scored without it, since Butteraugli's wide filters carry a change in
    // the frame well into the picture. The score is the largest value of
    // the difference map.
    double TButteraugliStore::Compute(const TView & first, const TView & second, adTransformType transform) const
    {
        TView turned;
        const TView * pFirst = &first;
        if (transform != AD_TRANSFORM_TURN_0)
        {
            Transform(first, turned, transform);
            pFirst = &turned;
        }
        const bool firstIsReference = pFirst->width * pFirst->height >= second.width * second.height;
        const TView & reference = firstIsReference ? *pFirst : second;
        const TView & other = firstIsReference ? second : *pFirst;
        const size_t w = other.width, h = other.height;

        TView resized;
        const TView * pReference = &reference;
        if (reference.width != w || reference.height != h)
        {
            resized.Recreate(w, h, TView::Bgr24);
            AreaResize(reference, resized);
            pReference = &resized;
        }

        const double frame = m_pOptions->advanced.ignoreFrameWidth / double(DENOMINATOR);
        size_t fx = (size_t)ceil(w * frame), fy = (size_t)ceil(h * frame);
        if (2 * fx >= w)
            fx = 0;
        if (2 * fy >= h)
            fy = 0;
        const TView a = pReference->Region(fx, fy, w - fx, h - fy);
        const TView b = other.Region(fx, fy, w - fx, h - fy);

        jxl::ImageF diffmap;
        if (!Diffmap(a, b, diffmap))
            return -1;
        return jxl::ButteraugliScoreFromDiffmap(diffmap);
    }

    // The local colour difference of two views, of any size, in percent: as
    // TImageComparer_LocalColor on thumbnails made from them.
    static double LocalColor(const TView & first, const TView & second)
    {
        const size_t n = COLOR_IMAGE_SIZE, block = COLOR_BLOCK_SIZE;
        TView a(n, n, TView::Bgr24), b(n, n, TView::Bgr24);
        AreaResize(first, a);
        AreaResize(second, b);
        uint64_t worst = 0;
        for (size_t by = 0; by < n; by += block)
        {
            for (size_t bx = 0; bx < n; bx += block)
            {
                uint64_t sum = 0;
                for (size_t y = by; y < by + block; ++y)
                {
                    const TUInt8 * p = a.data + y * a.stride + bx * 3, * q = b.data + y * b.stride + bx * 3;
                    for (size_t i = 0; i < block * 3; ++i)
                    {
                        const int d = int(p[i]) - int(q[i]);
                        sum += uint64_t(d * d);
                    }
                }
                worst = std::max(worst, sum);
            }
        }
        return sqrt(double(worst) / (block * block * 3)) * DENOMINATOR / PIXEL_MAX_DIFFERENCE;
    }

    // Two pictures laid over each other.
    struct TOverlay
    {
        const TView * pOther; // the one that stays as it is
        TView warped; // the other one, on its pixels
        TBox box; // what both show, in other's pixels
    };

    // The image that shows the picture smaller stays as it is and the other
    // is reduced and laid over it, as in Compute: blowing an image up blurs
    // it, which Butteraugli would score. That isn't always the one with fewer
    // pixels: a crop can have as many as the whole. Transform: second's
    // pixels to first's; frame: the share of each edge to leave out. False
    // when too little is left.
    static bool Overlay(const TView & first, const TView & second, const TAffine & transform, double frame, TOverlay & overlay)
    {
        const bool firstIsLarger = transform.Scale() >= 1;
        const TView & larger = firstIsLarger ? first : second;
        const TView & other = firstIsLarger ? second : first;
        overlay.pOther = &other;
        TAffine t = firstIsLarger ? transform : transform.Inverse();

        TView reduced;
        const TView * pReference = &larger;
        const double scale = t.Scale();
        if (scale > ALIGN_REDUCE_SCALE)
        {
            const size_t w = std::max<size_t>(8, size_t(larger.width / scale + 0.5));
            const size_t h = std::max<size_t>(8, size_t(larger.height / scale + 0.5));
            reduced.Recreate(w, h, TView::Bgr24);
            AreaResize(larger, reduced);
            t = Rescale(larger.width, larger.height, w, h).After(t);
            pReference = &reduced;
        }
        const TView & reference = *pReference;

        TView otherGray(other.width, other.height, TView::Gray8), referenceGray(reference.width, reference.height, TView::Gray8);
        Simd::BgrToGray(other, otherGray);
        Simd::BgrToGray(reference, referenceGray);
        RefineTransform(otherGray.data, other.width, other.height, otherGray.stride,
            referenceGray.data, reference.width, reference.height, referenceGray.stride, t);

        TBox & box = overlay.box;
        box = OverlapBox(t, other.width, other.height, reference.width, reference.height);
        const ptrdiff_t fx = (ptrdiff_t)ceil(other.width * frame), fy = (ptrdiff_t)ceil(other.height * frame);
        box.left = std::max(box.left, fx);
        box.top = std::max(box.top, fy);
        box.right = std::min(box.right, ptrdiff_t(other.width) - fx);
        box.bottom = std::min(box.bottom, ptrdiff_t(other.height) - fy);
        if (box.Width() < ptrdiff_t(ALIGN_BOX_SIDE_MIN) || box.Height() < ptrdiff_t(ALIGN_BOX_SIDE_MIN))
            return false;

        overlay.warped.Recreate(other.width, other.height, TView::Bgr24);
        WarpBicubic(reference.data, reference.width, reference.height, reference.stride, 3, t,
            overlay.warped.data, overlay.warped.width, overlay.warped.height, overlay.warped.stride);
        return true;
    }

    double TButteraugliStore::ComputeAligned(const TView & first, const TView & second, const TAffine & transform) const
    {
        TOverlay overlay;
        if (!Overlay(first, second, transform, m_pOptions->advanced.ignoreFrameWidth / double(DENOMINATOR), overlay))
            return -1;
        const TBox & box = overlay.box;
        const TView a = overlay.warped.Region(box.left, box.top, box.right, box.bottom);
        const TView b = overlay.pOther->Region(box.left, box.top, box.right, box.bottom);
        if (LocalColor(a, b) > BUTTERAUGLI_CANDIDATE_THRESHOLD)
            return -1;
        jxl::ImageF diffmap;
        if (!Diffmap(a, b, diffmap))
            return -1;
        return jxl::ButteraugliScoreFromDiffmap(diffmap);
    }

    void TButteraugliStore::Load()
    {
        std::ifstream file(m_path.c_str(), std::ios::binary);
        if (!file)
            return;
        TUInt32 header[3] = { 0 };
        TUInt64 count = 0;
        file.read((char*)header, sizeof(header));
        file.read((char*)&count, sizeof(count));
        if (!file || header[0] != SCORE_FILE_SIGNATURE || header[1] != SCORE_FILE_FORMAT ||
            header[2] != BUTTERAUGLI_SCORE_VERSION)
            return;
        m_scores.reserve((size_t)count);
        for (TUInt64 i = 0; i < count; ++i)
        {
            TKey key;
            double score;
            file.read((char*)&key, sizeof(key));
            file.read((char*)&score, sizeof(score));
            if (!file)
                break;
            m_scores[key] = score;
        }
    }

    // To a temporary file first, which then replaces the old one: a search
    // stopped while saving keeps the scores saved before.
    void TButteraugliStore::Save() const
    {
        const TString temporary = m_path + TEXT(".tmp");
        {
            std::ofstream file(temporary.c_str(), std::ios::binary | std::ios::trunc);
            if (!file)
                return;
            const TUInt32 header[3] = { SCORE_FILE_SIGNATURE, SCORE_FILE_FORMAT, BUTTERAUGLI_SCORE_VERSION };
            const TUInt64 count = m_scores.size();
            file.write((const char*)header, sizeof(header));
            file.write((const char*)&count, sizeof(count));
            for (auto & entry : m_scores)
            {
                file.write((const char*)&entry.first, sizeof(entry.first));
                file.write((const char*)&entry.second, sizeof(entry.second));
            }
            if (!file)
                return;
        }
        ::MoveFileEx(temporary.c_str(), m_path.c_str(), MOVEFILE_REPLACE_EXISTING);
    }
    //-------------------------------------------------------------------------
}
