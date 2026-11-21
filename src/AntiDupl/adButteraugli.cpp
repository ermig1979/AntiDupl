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
    TButteraugliStore::TButteraugliStore(TEngine *pEngine)
        : m_pOptions(pEngine->Options())
        , m_path(pEngine->UserPath() + TEXT("\\butteraugli.adp"))
        , m_imageBytes(0)
        , m_changed(false)
    {
        static_assert(sizeof(TKey) == 40, "TKey is written to the file as it is");
        Load();
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
}
