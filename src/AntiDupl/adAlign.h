/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
#ifndef __adAlign_h__
#define __adAlign_h__

#include <cstddef>
#include <cstdint>
#include <vector>

// Finding the same picture in two images whatever their framing: a crop, a
// wider canvas, a slightly different scale. Keypoints (corners described by
// the brightness around them) give a rough transform, patches compared
// between the two images make it exact to a fraction of a pixel. Depends on
// nothing else in the core: plain byte planes in and out.
namespace ad
{
    const size_t KEYPOINT_WORDS = 4; // 256 bits per keypoint

    //-------------------------------------------------------------------------
    struct TKeypoints
    {
        size_t width, height; // of the picture they were found on
        // Positions in steps of unit, from the picture's corner (-0.5 in
        // pixel centres): an image keeps its keypoints for the whole
        // search, and 16 bits hold them to a sixteenth of a pixel on any
        // copy up to 4096 pixels long, coarser on longer ones only.
        std::vector<uint16_t> x, y;
        float unit;
        std::vector<uint64_t> bits; // KEYPOINT_WORDS per keypoint

        TKeypoints() : width(0), height(0), unit(1) {}
        size_t Size() const { return x.size(); }
        float X(size_t i) const { return x[i] * unit - 0.5f; }
        float Y(size_t i) const { return y[i] * unit - 0.5f; }

        // The memory each keypoint takes, for estimates.
        static const size_t BYTES_PER_POINT = 2 * sizeof(uint16_t) + KEYPOINT_WORDS * sizeof(uint64_t);
    };

    // x' = a*x + b*y + c, y' = d*x + e*y + f
    struct TAffine
    {
        double a, b, c, d, e, f;

        // This one after other: x -> this(other(x)).
        TAffine After(const TAffine & other) const;
        TAffine Inverse() const;
        double Scale() const; // the square root of the area's change
    };

    struct TBox
    {
        ptrdiff_t left, top, right, bottom;

        ptrdiff_t Width() const { return right - left; }
        ptrdiff_t Height() const { return bottom - top; }
    };
    //-------------------------------------------------------------------------
    // Up to maxCount keypoints on a pyramid of levels scales, each 1.2 times
    // smaller than the last, from the picture's own, the strongest of each.
    // Two copies' keypoints match across as many levels' difference in
    // scale. Upright: a turned copy's keypoints don't match, which the
    // turned search's reduced images cover.
    void FindKeypoints(const uint8_t * gray, size_t width, size_t height, ptrdiff_t stride,
        size_t maxCount, size_t levels, TKeypoints & keypoints);

    // Picks the images worth aligning with one, without looking at every
    // pair: an image's keypoints are filed under bits of their descriptions,
    // and an image that shares many of them is a candidate. Hits on a
    // keypoint's exact bits are few for a changed copy and yet far more than
    // for an unrelated picture. Built once from all the images, numbered in
    // the order given.
    class TKeypointIndex
    {
    public:
        TKeypointIndex(size_t images, size_t entriesPerKey = 16, size_t hitsMin = 6, size_t tables = 4);

        // keypoints(i) gives image i's.
        template<class TGet> void Build(size_t images, TGet keypoints);
        void Find(const TKeypoints & keypoints, std::vector<uint32_t> & images);

        // The most memory an index of so many images, with up to so many
        // keypoints each, takes once built.
        static size_t Bytes(size_t images, size_t keypointsPerImage, size_t tables = 4);

    private:
        static size_t KeyBits(size_t images);
        size_t Key(const uint64_t * bits, size_t table) const;
        // An image's keys, each once.
        void Keys(const TKeypoints & keypoints, std::vector<uint32_t> & keys) const;

        size_t m_tables, m_keyBits, m_entriesPerKey, m_hitsMin;
        // A key's images are m_entries[m_starts[key]] up to the next key's
        // start. A key that reached entriesPerKey images holds none and is
        // marked full instead, so that it stops counting: what many images
        // share says nothing about any of them.
        std::vector<uint32_t> m_starts; // per table and key, plus one
        std::vector<bool> m_full;
        std::vector<uint32_t> m_entries;
        std::vector<uint16_t> m_hits; // per image, 0 between searches
        std::vector<uint32_t> m_hit; // the images with hits
    };

    template<class TGet> void TKeypointIndex::Build(size_t images, TGet keypoints)
    {
        // Two passes, counting then filling, so that the entries take
        // exactly their room: one by one, the vector's growth would hold
        // up to twice as much at its peak.
        std::vector<uint32_t> keys;
        for (size_t i = 0; i < images; ++i)
        {
            Keys(keypoints(i), keys);
            for (const uint32_t key : keys)
                ++m_starts[key];
        }
        size_t total = 0;
        for (size_t key = 0; key + 1 < m_starts.size(); ++key)
        {
            const size_t count = m_starts[key];
            if (count >= m_entriesPerKey)
                m_full[key] = true;
            else
                total += count;
            m_starts[key] = uint32_t(total); // the key's end, for now
        }
        m_starts.back() = uint32_t(total);
        m_entries.resize(total);
        // Filled from each key's end back: each start then ends where it
        // belongs, and a key's images come out in the order given.
        for (size_t i = images; i-- > 0;)
        {
            Keys(keypoints(i), keys);
            for (const uint32_t key : keys)
                if (!m_full[key])
                    m_entries[--m_starts[key]] = uint32_t(i);
        }
        m_hits.assign(images, 0);
    }

    // The transform from one set's pictures to the other's: scale, a small
    // turn and a shift. False when too few matches agree on one.
    bool FindTransform(const TKeypoints & from, const TKeypoints & to, TAffine & transform, size_t * pInliers);

    // Makes a transform from fixed's pixels to moving's exact: patches with
    // texture in fixed are sought in moving, and those that agree give the
    // correction, so what differs between the two doesn't pull it aside.
    void RefineTransform(const uint8_t * fixed, size_t fixedWidth, size_t fixedHeight, ptrdiff_t fixedStride,
        const uint8_t * moving, size_t movingWidth, size_t movingHeight, ptrdiff_t movingStride, TAffine & transform);

    // The largest box of fixedWidth x fixedHeight pixels the transform takes
    // inside movingWidth x movingHeight; empty when there's none.
    TBox OverlapBox(const TAffine & transform, size_t fixedWidth, size_t fixedHeight,
        size_t movingWidth, size_t movingHeight);

    // Each pixel (x, y) of dst takes moving's value at transform(x, y),
    // bicubic: bilinear sampling blurs, and Butteraugli sees that as a
    // difference. Byte channels, interleaved; the edges repeat.
    void WarpBicubic(const uint8_t * moving, size_t movingWidth, size_t movingHeight, ptrdiff_t movingStride,
        size_t channels, const TAffine & transform, uint8_t * dst, size_t dstWidth, size_t dstHeight, ptrdiff_t dstStride);
    //-------------------------------------------------------------------------
}
#endif//__adAlign_h__
