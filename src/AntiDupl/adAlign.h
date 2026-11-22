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
        std::vector<float> x, y;
        std::vector<uint64_t> bits; // KEYPOINT_WORDS per keypoint

        TKeypoints() : width(0), height(0) {}
        size_t Size() const { return x.size(); }
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
    // Up to maxCount keypoints at scales from the picture's own down to about
    // a quarter of it, the strongest of each. Upright: a turned copy's
    // keypoints don't match, which the turned search's reduced images cover.
    void FindKeypoints(const uint8_t * gray, size_t width, size_t height, ptrdiff_t stride,
        size_t maxCount, TKeypoints & keypoints);

    // Picks the images worth aligning with one, without looking at every
    // pair: an image's keypoints are filed under bits of their descriptions,
    // and an image that shares many of them is a candidate. Hits on a
    // keypoint's exact bits are few for a changed copy and yet far more than
    // for an unrelated picture. Images are numbered in the order added.
    class TKeypointIndex
    {
    public:
        TKeypointIndex(size_t imagesExpected, size_t entriesPerKey = 16, size_t hitsMin = 6, size_t tables = 4);

        void Add(const TKeypoints & keypoints);
        void Find(const TKeypoints & keypoints, std::vector<uint32_t> & images);

    private:
        struct TEntry
        {
            uint32_t image, next; // next: an index into m_entries, plus 1
        };

        size_t Key(const uint64_t * bits, size_t table) const;

        size_t m_tables, m_keyBits, m_entriesPerKey, m_hitsMin;
        uint32_t m_imageCount;
        std::vector<uint32_t> m_heads; // per table and key, as TEntry::next
        std::vector<uint8_t> m_counts; // entries under each
        std::vector<TEntry> m_entries;
        std::vector<uint16_t> m_hits; // per image, 0 between searches
        std::vector<uint32_t> m_hit; // the images with hits
    };

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
