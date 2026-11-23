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
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <thread>
#include <immintrin.h>
#include <intrin.h>

#include "adAlign.h"

namespace ad
{
    // Keypoints: FAST corners (Rosten and Drummond) on a pyramid, ranked by
    // Harris' measure as ORB does (Rublee et al.), with BRIEF's comparisons
    // of smoothed pixels as the description (Calonder et al.).
    static const double LEVEL_SCALE = 1.2;
    static const int FAST_THRESHOLD = 20;
    static const int FAST_THRESHOLD_LOW = 8; // for a picture with few corners
    static const int FAST_ARC = 9;
    static const int PATCH_RADIUS = 15; // of the description's comparisons
    static const int MARGIN = PATCH_RADIUS + 1;
    static const int HARRIS_RADIUS = 3;
    static const double HARRIS_K = 0.04;

    // Matching: Lowe's ratio test, then RANSAC.
    static const int RATIO_NUMERATOR = 4, RATIO_DENOMINATOR = 5; // best under 0.8 of the second best
    static const size_t MATCHES_MIN = 8;
    static const size_t INLIERS_MIN = 12;
    static const double INLIER_DISTANCE = 2.0; // pixels of the keypoints' picture
    static const int RANSAC_ROUNDS_MAX = 2000;
    static const double RANSAC_CONFIDENCE = 0.995;
    // A small file against a crop of a large one, both copied to 3 million
    // pixels, can differ 11 times in scale; 8 missed such pairs.
    static const double SCALE_MIN = 1.0 / 16, SCALE_MAX = 16.0;
    static const double TURN_MAX = 0.35; // radians; upright descriptions don't match past it

    // The index.
    static const size_t KEYPOINT_INDEX_ENTRIES_PER_IMAGE = 500;

    // Refinement.
    static const int REFINE_PATCH = 24;
    static const int REFINE_STEP = 40;
    static const int REFINE_RADII[] = { 6, 3, 2 };
    static const double REFINE_TEXTURE_MIN = 500.0; // smaller eigenvalue of the gradients' moments, per pixel
    static const double REFINE_CORRELATION_MIN = 0.7;
    static const size_t REFINE_PATCHES_MIN = 8;
    static const double REFINE_OUTLIER_MIN = 0.25; // pixels
    static const double REFINE_REACH = 3.0; // times the radius searched

    //-------------------------------------------------------------------------
    TAffine TAffine::After(const TAffine & o) const
    {
        TAffine r;
        r.a = a * o.a + b * o.d;
        r.b = a * o.b + b * o.e;
        r.c = a * o.c + b * o.f + c;
        r.d = d * o.a + e * o.d;
        r.e = d * o.b + e * o.e;
        r.f = d * o.c + e * o.f + f;
        return r;
    }

    TAffine TAffine::Inverse() const
    {
        const double det = a * e - b * d;
        TAffine r;
        r.a = e / det;
        r.b = -b / det;
        r.d = -d / det;
        r.e = a / det;
        r.c = -(r.a * c + r.b * f);
        r.f = -(r.d * c + r.e * f);
        return r;
    }

    double TAffine::Scale() const
    {
        return sqrt(fabs(a * e - b * d));
    }
    //-------------------------------------------------------------------------
    struct TPlane
    {
        int width, height;
        std::vector<uint8_t> data;

        void Resize(int w, int h)
        {
            width = w;
            height = h;
            data.resize(size_t(w) * h);
        }
        uint8_t * Row(int y) { return data.data() + size_t(y) * width; }
        const uint8_t * Row(int y) const { return data.data() + size_t(y) * width; }
    };

    struct TCorner
    {
        int x, y;
        float response;
    };

    // For one axis: where each output's input range starts, and the weights
    // of its inputs.
    struct TBoxTaps
    {
        std::vector<int> first, count;
        std::vector<float> weights;
        std::vector<size_t> offset;
    };

    static void BoxTaps(int srcSize, int dstSize, TBoxTaps & taps)
    {
        const double scale = double(srcSize) / dstSize;
        taps.first.resize(dstSize);
        taps.count.resize(dstSize);
        taps.offset.resize(dstSize);
        taps.weights.clear();
        for (int i = 0; i < dstSize; ++i)
        {
            const double from = i * scale, to = (i + 1) * scale;
            const int first = int(from), last = std::min(srcSize - 1, int(ceil(to)) - 1);
            taps.first[i] = first;
            taps.count[i] = last - first + 1;
            taps.offset[i] = taps.weights.size();
            for (int j = first; j <= last; ++j)
            {
                const double covered = std::min(to, j + 1.0) - std::max(from, double(j));
                taps.weights.push_back(float(covered / scale));
            }
        }
    }

    // Area averaging to dst's size.
    static void Reduce(const uint8_t * src, int srcWidth, int srcHeight, ptrdiff_t srcStride, TPlane & dst)
    {
        TBoxTaps cols, rows;
        BoxTaps(srcWidth, dst.width, cols);
        BoxTaps(srcHeight, dst.height, rows);
        std::vector<float> narrowed(size_t(dst.width) * srcHeight);
        for (int y = 0; y < srcHeight; ++y)
        {
            const uint8_t * p = src + y * srcStride;
            float * q = narrowed.data() + size_t(y) * dst.width;
            for (int x = 0; x < dst.width; ++x)
            {
                const float * w = cols.weights.data() + cols.offset[x];
                const uint8_t * s = p + cols.first[x];
                float sum = 0;
                for (int i = 0; i < cols.count[x]; ++i)
                    sum += w[i] * s[i];
                q[x] = sum;
            }
        }
        std::vector<float> sums(dst.width);
        for (int y = 0; y < dst.height; ++y)
        {
            std::fill(sums.begin(), sums.end(), 0.0f);
            const float * w = rows.weights.data() + rows.offset[y];
            for (int i = 0; i < rows.count[y]; ++i)
            {
                const float * s = narrowed.data() + size_t(rows.first[y] + i) * dst.width;
                for (int x = 0; x < dst.width; ++x)
                    sums[x] += w[i] * s[x];
            }
            uint8_t * q = dst.Row(y);
            for (int x = 0; x < dst.width; ++x)
                q[x] = uint8_t(std::min(255.0f, sums[x] + 0.5f));
        }
    }

    // A Gaussian of sigma 2 in seven taps, in 256ths.
    static void Smooth(const TPlane & src, TPlane & dst)
    {
        static const int KERNEL[7] = { 18, 34, 49, 54, 49, 34, 18 };
        const int w = src.width, h = src.height;
        std::vector<uint16_t> across(size_t(w) * h);
        for (int y = 0; y < h; ++y)
        {
            const uint8_t * p = src.Row(y);
            uint16_t * q = across.data() + size_t(y) * w;
            for (int x = 0; x < w; ++x)
            {
                int sum = 0;
                for (int i = -3; i <= 3; ++i)
                    sum += KERNEL[i + 3] * p[std::min(w - 1, std::max(0, x + i))];
                q[x] = uint16_t(sum >> 4); // 16 x the value: fits, and keeps a fraction
            }
        }
        dst.Resize(w, h);
        for (int y = 0; y < h; ++y)
        {
            const uint16_t * rows[7];
            for (int i = -3; i <= 3; ++i)
                rows[i + 3] = across.data() + size_t(std::min(h - 1, std::max(0, y + i))) * w;
            uint8_t * q = dst.Row(y);
            for (int x = 0; x < w; ++x)
            {
                int sum = 0;
                for (int i = 0; i < 7; ++i)
                    sum += KERNEL[i] * rows[i][x];
                q[x] = uint8_t(std::min(255, (sum + (1 << 11)) >> 12));
            }
        }
    }

    // Where a FAST corner has an arc of FAST_ARC pixels of the circle around
    // it brighter or darker than itself by the threshold: the sum of what
    // they exceed it by, else 0.
    static int FastScore(const uint8_t * p, const int * circle, int threshold)
    {
        const int v = p[0], darker = v - threshold, brighter = v + threshold;
        // Two pixels opposite each other can't both be outside an arc of 9.
        int kinds = 3;
        for (int i = 0; i < 8; ++i)
        {
            const int s = p[circle[i]], t = p[circle[i + 8]];
            int here = 0;
            if (s < darker || t < darker)
                here |= 1;
            if (s > brighter || t > brighter)
                here |= 2;
            kinds &= here;
            if (kinds == 0)
                return 0;
        }
        int best = 0;
        for (int kind = 1; kind <= 2; ++kind)
        {
            if (!(kinds & kind))
                continue;
            int run = 0, longest = 0, sum = 0;
            for (int i = 0; i < 16 + FAST_ARC - 1; ++i)
            {
                const int s = p[circle[i & 15]];
                if (kind == 1 ? s < darker : s > brighter)
                    longest = std::max(longest, ++run);
                else
                    run = 0;
            }
            if (longest < FAST_ARC)
                continue;
            for (int i = 0; i < 16; ++i)
            {
                const int s = p[circle[i]];
                sum += std::max(0, kind == 1 ? darker - s : s - brighter);
            }
            best = std::max(best, sum + 1);
        }
        return best;
    }

    static float HarrisResponse(const TPlane & plane, int cx, int cy)
    {
        int xx = 0, yy = 0, xy = 0;
        const int w = plane.width;
        for (int y = cy - HARRIS_RADIUS; y <= cy + HARRIS_RADIUS; ++y)
        {
            const uint8_t * p = plane.Row(y);
            for (int x = cx - HARRIS_RADIUS; x <= cx + HARRIS_RADIUS; ++x)
            {
                const uint8_t * q = p + x;
                const int dx = (q[1 - w] - q[-1 - w]) + 2 * (q[1] - q[-1]) + (q[1 + w] - q[-1 + w]);
                const int dy = (q[w - 1] - q[-w - 1]) + 2 * (q[w] - q[-w]) + (q[w + 1] - q[-w + 1]);
                xx += dx * dx;
                yy += dy * dy;
                xy += dx * dy;
            }
        }
        const double a = xx, b = yy, c = xy, scale = 1.0 / (49.0 * 4 * 255);
        return float(((a * b - c * c) - HARRIS_K * (a + b) * (a + b)) * scale * scale * scale * scale);
    }

    // The corners of a plane, the strongest wanted ones.
    static void FindCorners(const TPlane & plane, int threshold, size_t wanted, std::vector<TCorner> & corners)
    {
        static const int CIRCLE[16][2] = { { 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1 }, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 },
            { 0, -3 }, { -1, -3 }, { -2, -2 }, { -3, -1 }, { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3 } };
        const int w = plane.width, h = plane.height;
        int circle[16];
        for (int i = 0; i < 16; ++i)
            circle[i] = CIRCLE[i][1] * w + CIRCLE[i][0];

        corners.clear();
        std::vector<int> scores(size_t(w) * h, 0);
        for (int y = MARGIN; y < h - MARGIN; ++y)
        {
            const uint8_t * p = plane.Row(y);
            int * s = scores.data() + size_t(y) * w;
            for (int x = MARGIN; x < w - MARGIN; ++x)
                s[x] = FastScore(p + x, circle, threshold);
        }
        for (int y = MARGIN; y < h - MARGIN; ++y)
        {
            const int * s = scores.data() + size_t(y) * w;
            for (int x = MARGIN; x < w - MARGIN; ++x)
            {
                const int v = s[x];
                if (v == 0 || v < s[x - 1] || v <= s[x + 1] || v < s[x - w - 1] || v < s[x - w] || v < s[x - w + 1] ||
                    v <= s[x + w - 1] || v <= s[x + w] || v <= s[x + w + 1])
                    continue;
                TCorner corner = { x, y, HarrisResponse(plane, x, y) };
                corners.push_back(corner);
            }
        }
        if (corners.size() > wanted)
        {
            std::nth_element(corners.begin(), corners.begin() + wanted, corners.end(),
                [](const TCorner & a, const TCorner & b) { return a.response > b.response; });
            corners.resize(wanted);
        }
    }

    // BRIEF's comparisons, x1, y1, x2, y2 around the keypoint, chosen as ORB's
    // authors chose theirs: from thousands of random ones on patches around
    // corners of drawings, the ones nearest an even split that correlate
    // least. Random ones leave descriptions so alike that an index can't
    // tell pictures apart by them.
    static const int8_t PATTERN[256][4] = {
        {-11,2,-9,9}, {4,-1,3,3}, {-2,5,-10,-5}, {-11,3,-5,12}, {-13,-1,-5,9}, {7,-5,-11,-9}, {-14,1,-8,-11}, {10,0,-9,-5},
        {0,6,14,-1}, {7,-3,14,3}, {-4,-8,-6,11}, {-9,-5,8,7}, {14,1,-8,-9}, {-2,-7,-1,-11}, {0,6,-4,-11}, {4,-7,3,-13},
        {9,0,6,-8}, {8,-3,10,-7}, {-5,4,-12,-9}, {9,-2,7,11}, {-2,-5,-3,4}, {5,4,14,-4}, {-7,1,6,-3}, {-12,1,-1,-9},
        {8,-2,0,9}, {-12,-3,2,-11}, {10,0,4,6}, {-2,8,2,-14}, {-7,2,-7,6}, {-12,3,11,-4}, {-8,-1,-9,5}, {-1,-9,7,10},
        {14,-2,-4,-14}, {4,-5,6,3}, {5,-8,-7,7}, {6,-3,13,-3}, {-8,-1,-4,5}, {9,4,-8,6}, {-6,1,-9,-4}, {-2,4,-13,2},
        {8,-2,-1,-9}, {-1,-5,3,3}, {-13,3,0,15}, {-1,12,8,-12}, {-4,-7,-7,-11}, {5,-1,-11,-1}, {6,-6,12,6}, {-9,-5,7,-11},
        {-3,-4,9,-3}, {-7,4,4,6}, {-9,2,-11,5}, {12,1,3,-11}, {13,-2,-11,9}, {-6,-5,1,12}, {-4,-7,-10,5}, {0,-5,-12,-2},
        {6,-3,7,-6}, {-6,1,2,-7}, {7,-4,4,-11}, {9,0,-14,0}, {-8,-2,3,4}, {-2,-11,12,5}, {0,-9,4,-14}, {0,6,6,13},
        {-6,2,-2,5}, {7,3,-6,-8}, {-5,4,-5,11}, {4,-3,7,-2}, {4,6,7,-13}, {-13,3,5,4}, {6,-7,-7,-12}, {-6,-3,-10,-4},
        {-2,5,5,8}, {3,5,-10,9}, {-6,-3,4,-6}, {9,0,12,-6}, {-6,-1,-5,-6}, {5,2,12,2}, {-11,-3,-5,14}, {-6,5,-1,-14},
        {6,4,11,10}, {12,1,-2,11}, {-5,3,-14,-2}, {-7,4,-2,14}, {-2,-7,13,-7}, {3,-5,14,0}, {10,2,11,10}, {-8,1,-14,2},
        {-12,-3,3,11}, {10,-2,9,-10}, {7,-5,-2,-12}, {-11,-1,-14,-5}, {-12,-2,15,0}, {2,-10,-3,14}, {-1,-7,8,-9}, {0,-8,-3,-12},
        {2,7,10,-6}, {-8,-4,-2,-11}, {-2,-7,-10,-9}, {5,-1,8,1}, {-10,-1,-14,5}, {-8,-5,-9,-12}, {-2,6,-6,11}, {13,1,-6,4},
        {-7,-1,-12,-2}, {7,4,-2,13}, {-3,-1,3,1}, {-9,0,-13,-3}, {7,3,5,14}, {2,5,14,4}, {4,-4,9,-4}, {-3,5,3,13},
        {-1,1,2,-1}, {2,6,11,10}, {5,-6,6,10}, {-9,-3,-14,-4}, {0,-7,3,-10}, {3,3,6,1}, {-3,-4,-9,0}, {7,3,-1,6},
        {-13,-3,10,-6}, {-5,-5,-4,-12}, {-6,4,-11,10}, {1,6,-3,14}, {-7,-4,-7,11}, {4,5,-8,-12}, {2,-4,5,-1}, {8,-5,-4,14},
        {-6,2,-9,3}, {-11,4,9,10}, {1,-10,-13,7}, {-5,-1,-13,0}, {-6,-6,2,-13}, {0,5,4,-9}, {8,3,6,-12}, {-10,2,-10,-9},
        {6,-4,9,-6}, {-1,7,-11,10}, {-5,-4,-7,-7}, {2,-7,-8,-9}, {1,-6,-1,-9}, {-3,3,8,-1}, {1,-5,8,-4}, {5,2,-6,2},
        {11,-2,0,15}, {-3,5,-3,14}, {-5,-2,-5,4}, {10,-1,13,7}, {-4,-5,11,-8}, {7,-6,9,-12}, {-9,5,10,-11}, {-1,-5,-4,-5},
        {9,2,13,4}, {-2,-5,4,-8}, {-14,-2,-5,-14}, {-1,5,0,8}, {-14,4,5,-13}, {14,2,10,-11}, {-2,5,-6,6}, {3,-7,11,-9},
        {11,2,5,11}, {6,3,7,6}, {6,0,10,-1}, {8,0,14,-3}, {-7,-4,-13,7}, {-1,-7,12,1}, {5,-6,-14,-5}, {5,-8,0,-15},
        {7,3,4,8}, {6,1,9,-5}, {5,-6,8,-9}, {-10,-3,-12,-9}, {-6,4,-13,5}, {5,1,8,-1}, {1,5,8,5}, {0,11,-7,-13},
        {-10,4,-6,-13}, {-6,-5,-12,-9}, {-4,4,-1,7}, {-2,-9,-4,-14}, {-9,-3,-7,-11}, {6,-3,7,6}, {-3,-5,-14,-5}, {1,5,1,10},
        {-2,7,14,5}, {-3,-5,-6,-6}, {-7,-3,-10,-7}, {5,5,0,-15}, {5,3,2,-10}, {-4,-4,1,-8}, {-11,-1,-11,8}, {2,-8,7,-11},
        {5,-4,-3,-7}, {-4,8,12,-5}, {10,-3,0,-15}, {-1,7,10,8}, {1,5,-4,7}, {-8,-2,-8,-7}, {6,3,-14,-5}, {-10,2,1,8},
        {-4,5,-10,8}, {10,1,9,7}, {-10,3,-12,8}, {-9,-6,-5,-14}, {6,4,12,5}, {-1,-8,-1,-13}, {0,-8,-12,-7}, {-7,5,13,7},
        {1,6,-14,5}, {-5,-1,-8,-2}, {-3,-9,0,-15}, {13,-3,-13,-6}, {6,-3,4,-7}, {5,4,6,10}, {-4,-10,0,15}, {8,-4,11,10},
        {4,-6,4,-10}, {-2,5,-10,5}, {-12,-8,9,12}, {12,0,12,-8}, {3,5,6,7}, {-2,5,-3,10}, {-5,-4,-14,2}, {-5,2,-6,-7},
        {3,12,2,-14}, {-10,0,-7,-6}, {14,0,6,8}, {7,4,11,-9}, {-1,-6,-4,-8}, {1,-8,13,-5}, {-8,-3,13,-3}, {4,-5,1,-9},
        {1,-7,0,10}, {-10,3,-14,4}, {0,13,-13,-7}, {1,9,12,8}, {-2,3,-5,-1}, {2,13,12,-7}, {2,6,3,14}, {-6,-8,9,-12},
        {0,-9,-8,-12}, {-14,1,-13,-7}, {6,1,5,6}, {8,-5,12,-8}, {-3,-6,11,8}, {7,12,-4,-14}, {0,-4,3,-4}, {2,13,14,5},
        {2,4,-1,5}, {0,1,1,-3}, {4,2,8,2}, {-12,4,13,4}, {-12,-7,4,-14}, {-8,2,-9,11}, {7,2,14,2}, {-5,-1,12,1}
    };

    static void Describe(const TPlane & smooth, int x, int y, uint64_t * bits)
    {
        const uint8_t * center = smooth.Row(y) + x;
        const int w = smooth.width;
        for (int word = 0; word < 4; ++word)
        {
            uint64_t value = 0;
            for (int i = 0; i < 64; ++i)
            {
                const int8_t * test = PATTERN[word * 64 + i];
                const int a = center[test[1] * w + test[0]];
                const int b = center[test[3] * w + test[2]];
                value |= uint64_t(a < b) << i;
            }
            bits[word] = value;
        }
    }

    void FindKeypoints(const uint8_t * gray, size_t width, size_t height, ptrdiff_t stride,
        size_t maxCount, size_t levels, TKeypoints & keypoints)
    {
        keypoints.width = width;
        keypoints.height = height;
        keypoints.unit = float(std::max<size_t>(std::max(width, height), 1)) / 65535;
        keypoints.x.clear();
        keypoints.y.clear();
        keypoints.x.reserve(maxCount);
        keypoints.y.reserve(maxCount);
        std::vector<uint64_t> bits;
        bits.reserve(maxCount * KEYPOINT_WORDS);

        // Each level's share of the keypoints follows its area's root, as
        // in ORB, so that a smaller copy's keypoints find theirs.
        std::vector<double> shares(levels);
        double total = 0;
        for (size_t i = 0; i < levels; ++i)
            total += shares[i] = pow(1 / LEVEL_SCALE, double(i));

        TPlane plane, smooth;
        std::vector<TCorner> corners;
        for (size_t level = 0; level < levels; ++level)
        {
            const double scale = pow(LEVEL_SCALE, double(level));
            const int w = int(width / scale + 0.5), h = int(height / scale + 0.5);
            if (w < 2 * MARGIN + 8 || h < 2 * MARGIN + 8)
                break;
            plane.Resize(w, h);
            Reduce(gray, int(width), int(height), stride, plane);
            const size_t wanted = size_t(maxCount * shares[level] / total + 0.5);
            FindCorners(plane, FAST_THRESHOLD, wanted, corners);
            if (corners.size() < wanted / 2)
                FindCorners(plane, FAST_THRESHOLD_LOW, wanted, corners);
            if (corners.empty())
                continue;
            Smooth(plane, smooth);
            const double sx = double(width) / w, sy = double(height) / h;
            for (const TCorner & c : corners)
            {
                keypoints.x.push_back(uint16_t((c.x + 0.5) * sx / keypoints.unit + 0.5));
                keypoints.y.push_back(uint16_t((c.y + 0.5) * sy / keypoints.unit + 0.5));
                bits.resize(bits.size() + KEYPOINT_WORDS);
                Describe(smooth, c.x, c.y, bits.data() + bits.size() - KEYPOINT_WORDS);
            }
        }
        keypoints.x.shrink_to_fit();
        keypoints.y.shrink_to_fit();
        keypoints.SetDescriptions(bits);
    }

    // A description's 32-bit word w is the low (even w) or high half of its
    // 64-bit word w / 2, as both lie in memory on a little-endian machine.
    void TKeypoints::Description(size_t i, uint64_t * bits) const
    {
        const uint32_t * block = words.data() + (i / KEYPOINT_BLOCK) * KEYPOINT_BLOCK * 2 * KEYPOINT_WORDS;
        const size_t lane = i % KEYPOINT_BLOCK;
        for (size_t k = 0; k < KEYPOINT_WORDS; ++k)
            bits[k] = uint64_t(block[(2 * k) * KEYPOINT_BLOCK + lane]) |
                uint64_t(block[(2 * k + 1) * KEYPOINT_BLOCK + lane]) << 32;
    }

    void TKeypoints::SetDescriptions(const std::vector<uint64_t> & bits)
    {
        const size_t count = bits.size() / KEYPOINT_WORDS;
        const size_t blocks = (count + KEYPOINT_BLOCK - 1) / KEYPOINT_BLOCK;
        words.assign(blocks * KEYPOINT_BLOCK * 2 * KEYPOINT_WORDS, 0);
        for (size_t i = 0; i < count; ++i)
        {
            uint32_t * block = words.data() + (i / KEYPOINT_BLOCK) * KEYPOINT_BLOCK * 2 * KEYPOINT_WORDS;
            const size_t lane = i % KEYPOINT_BLOCK;
            for (size_t k = 0; k < KEYPOINT_WORDS; ++k)
            {
                block[(2 * k) * KEYPOINT_BLOCK + lane] = uint32_t(bits[i * KEYPOINT_WORDS + k]);
                block[(2 * k + 1) * KEYPOINT_BLOCK + lane] = uint32_t(bits[i * KEYPOINT_WORDS + k] >> 32);
            }
        }
    }
    //-------------------------------------------------------------------------
    TKeypointIndex::TKeypointIndex(size_t images, size_t entriesPerKey, size_t hitsMin, size_t tables)
        : m_tables(tables)
        , m_keyBits(KeyBits(images))
        , m_entriesPerKey(entriesPerKey)
        , m_hitsMin(hitsMin)
    {
        m_starts.assign((m_tables << m_keyBits) + 1, 0);
        m_full.assign(m_tables << m_keyBits, false);
    }

    size_t TKeypointIndex::KeyBits(size_t images)
    {
        // About one entry per key at most in a table, whose keys are bits
        // the description's words give without any hashing.
        size_t bits = 16;
        while (bits < 24 && (size_t(1) << bits) < images * KEYPOINT_INDEX_ENTRIES_PER_IMAGE)
            ++bits;
        return bits;
    }

    size_t TKeypointIndex::Bytes(size_t images, size_t keypointsPerImage, size_t tables)
    {
        const size_t keys = tables << KeyBits(images);
        return keys * sizeof(uint32_t) + keys / 8 + images * keypointsPerImage * tables * sizeof(uint32_t) +
            images * (sizeof(uint16_t) + sizeof(uint32_t));
    }

    size_t TKeypointIndex::Key(const uint64_t * bits, size_t table) const
    {
        const size_t word = table % KEYPOINT_WORDS, part = table / KEYPOINT_WORDS;
        return (table << m_keyBits) + size_t((bits[word] >> (part * m_keyBits)) & ((uint64_t(1) << m_keyBits) - 1));
    }

    void TKeypointIndex::Keys(const TKeypoints & keypoints, std::vector<uint32_t> & keys) const
    {
        keys.clear();
        uint64_t bits[KEYPOINT_WORDS];
        for (size_t i = 0, n = keypoints.Size(); i < n; ++i)
        {
            keypoints.Description(i, bits);
            for (size_t table = 0; table < m_tables; ++table)
                keys.push_back(uint32_t(Key(bits, table)));
        }
        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
    }

    void TKeypointIndex::Find(const TKeypoints & keypoints, std::vector<uint32_t> & images)
    {
        images.clear();
        m_hit.clear();
        uint64_t bits[KEYPOINT_WORDS];
        for (size_t i = 0, n = keypoints.Size(); i < n; ++i)
        {
            keypoints.Description(i, bits);
            for (size_t table = 0; table < m_tables; ++table)
            {
                const size_t key = Key(bits, table);
                for (uint32_t e = m_starts[key], end = m_starts[key + 1]; e < end; ++e)
                {
                    const uint32_t image = m_entries[e];
                    if (m_hits[image]++ == 0)
                        m_hit.push_back(image);
                }
            }
        }
        for (const uint32_t image : m_hit)
        {
            if (m_hits[image] >= m_hitsMin)
                images.push_back(image);
            m_hits[image] = 0;
        }
    }
    //-------------------------------------------------------------------------
    // The nearest and second nearest of to's descriptions to one (its eight
    // 32-bit words given), by bits that differ: the nearest's earliest
    // index on ties, and the second counting every other, an equal one too.
    struct TNearest
    {
        int best, second;
        size_t bestIndex;
    };

    static TNearest NearestScalar(const uint32_t * query, const TKeypoints & to)
    {
        TNearest n = { INT32_MAX, INT32_MAX, 0 };
        for (size_t j = 0, m = to.Size(); j < m; ++j)
        {
            const uint32_t * block = to.words.data() + (j / KEYPOINT_BLOCK) * KEYPOINT_BLOCK * 2 * KEYPOINT_WORDS;
            const size_t lane = j % KEYPOINT_BLOCK;
            int d = 0;
            for (size_t w = 0; w < 2 * KEYPOINT_WORDS; ++w)
                d += int(__popcnt(query[w] ^ block[w * KEYPOINT_BLOCK + lane]));
            if (d < n.best)
            {
                n.second = n.best;
                n.best = d;
                n.bestIndex = j;
            }
            else if (d < n.second)
                n.second = d;
        }
        return n;
    }

    // The same, a block at a time: each lane keeps its own nearest (the
    // earliest on ties) and second, and the lanes merge into exactly what
    // the scalar loop finds. Bits are counted by a nibble table per byte.
    static TNearest NearestAvx2(const uint32_t * query, const TKeypoints & to)
    {
        static_assert(KEYPOINT_BLOCK == 8 && KEYPOINT_WORDS == 4, "a block is one register of 32-bit words");
        const __m256i table = _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
        const __m256i low = _mm256_set1_epi8(0x0f), ones8 = _mm256_set1_epi8(1), ones16 = _mm256_set1_epi16(1);
        const __m256i step = _mm256_set1_epi32(KEYPOINT_BLOCK), none = _mm256_set1_epi32(INT32_MAX);
        const __m256i count = _mm256_set1_epi32(int(to.Size()));
        __m256i q[8];
        for (int w = 0; w < 8; ++w)
            q[w] = _mm256_set1_epi32(int(query[w]));
        __m256i best = none, second = none, bestIndex = _mm256_setzero_si256();
        __m256i index = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        const size_t blocks = (to.Size() + KEYPOINT_BLOCK - 1) / KEYPOINT_BLOCK;
        for (size_t b = 0; b < blocks; ++b)
        {
            const __m256i * t = (const __m256i *)(to.words.data() + b * 64);
            // At most 8 bits per byte from each of the 8 words: a byte holds the sum.
            __m256i sum = _mm256_setzero_si256();
            for (int w = 0; w < 8; ++w)
            {
                const __m256i x = _mm256_xor_si256(q[w], _mm256_loadu_si256(t + w));
                sum = _mm256_add_epi8(sum, _mm256_add_epi8(_mm256_shuffle_epi8(table, _mm256_and_si256(x, low)),
                    _mm256_shuffle_epi8(table, _mm256_and_si256(_mm256_srli_epi16(x, 4), low))));
            }
            __m256i d = _mm256_madd_epi16(_mm256_maddubs_epi16(sum, ones8), ones16);
            // The padding past the last keypoint never comes near.
            d = _mm256_blendv_epi8(none, d, _mm256_cmpgt_epi32(count, index));
            const __m256i nearer = _mm256_cmpgt_epi32(best, d), nearerSecond = _mm256_cmpgt_epi32(second, d);
            second = _mm256_blendv_epi8(second, _mm256_blendv_epi8(d, best, nearer), nearerSecond);
            best = _mm256_blendv_epi8(best, d, nearer);
            bestIndex = _mm256_blendv_epi8(bestIndex, index, nearer);
            index = _mm256_add_epi32(index, step);
        }
        alignas(32) int32_t bests[8], seconds[8], indices[8];
        _mm256_store_si256((__m256i *)bests, best);
        _mm256_store_si256((__m256i *)seconds, second);
        _mm256_store_si256((__m256i *)indices, bestIndex);
        TNearest n = { INT32_MAX, INT32_MAX, 0 };
        int lane = -1;
        for (int l = 0; l < 8; ++l)
            if (bests[l] < n.best || (bests[l] == n.best && size_t(indices[l]) < n.bestIndex))
            {
                n.best = bests[l];
                n.bestIndex = size_t(indices[l]);
                lane = l;
            }
        for (int l = 0; l < 8; ++l)
        {
            n.second = std::min(n.second, seconds[l]);
            if (l != lane)
                n.second = std::min(n.second, bests[l]);
        }
        return n;
    }

    static bool HasAvx2()
    {
        int info[4];
        __cpuid(info, 0);
        if (info[0] < 7)
            return false;
        __cpuid(info, 1);
        const bool osSaves = (info[2] & (1 << 27)) != 0, avx = (info[2] & (1 << 28)) != 0;
        if (!osSaves || !avx || (_xgetbv(0) & 6) != 6)
            return false;
        __cpuidex(info, 7, 0);
        return (info[1] & (1 << 5)) != 0;
    }

    struct TMatch
    {
        float fromX, fromY, toX, toY;
    };

    // The similarity (scale, turn, shift) nearest to the matches given.
    static bool FitSimilarity(const std::vector<TMatch> & matches, const std::vector<size_t> & chosen, TAffine & t)
    {
        double mx = 0, my = 0, nx = 0, ny = 0;
        for (size_t i : chosen)
        {
            mx += matches[i].fromX;
            my += matches[i].fromY;
            nx += matches[i].toX;
            ny += matches[i].toY;
        }
        const double n = double(chosen.size());
        mx /= n; my /= n; nx /= n; ny /= n;
        double dot = 0, cross = 0, norm = 0;
        for (size_t i : chosen)
        {
            const double ux = matches[i].fromX - mx, uy = matches[i].fromY - my;
            const double vx = matches[i].toX - nx, vy = matches[i].toY - ny;
            dot += ux * vx + uy * vy;
            cross += ux * vy - uy * vx;
            norm += ux * ux + uy * uy;
        }
        if (norm < 1e-6)
            return false;
        const double a = dot / norm, b = cross / norm;
        t.a = a;
        t.b = -b;
        t.d = b;
        t.e = a;
        t.c = nx - (a * mx - b * my);
        t.f = ny - (b * mx + a * my);
        return true;
    }

    static void Inliers(const std::vector<TMatch> & matches, const TAffine & t, std::vector<size_t> & inliers)
    {
        inliers.clear();
        const double limit = INLIER_DISTANCE * INLIER_DISTANCE;
        for (size_t i = 0; i < matches.size(); ++i)
        {
            const TMatch & m = matches[i];
            const double dx = t.a * m.fromX + t.b * m.fromY + t.c - m.toX;
            const double dy = t.d * m.fromX + t.e * m.fromY + t.f - m.toY;
            if (dx * dx + dy * dy <= limit)
                inliers.push_back(i);
        }
    }

    static bool Plausible(const TAffine & t)
    {
        const double scale = sqrt(t.a * t.a + t.d * t.d);
        return scale >= SCALE_MIN && scale <= SCALE_MAX && fabs(atan2(t.d, t.a)) <= TURN_MAX;
    }

    bool FindTransform(const TKeypoints & from, const TKeypoints & to, TAffine & transform, size_t * pInliers)
    {
        static const bool avx2 = HasAvx2();
        std::vector<TMatch> matches;
        uint64_t bits[KEYPOINT_WORDS];
        for (size_t i = 0, n = from.Size(), m = to.Size(); i < n; ++i)
        {
            from.Description(i, bits);
            const uint32_t * query = (const uint32_t *)bits;
            const TNearest nearest = avx2 ? NearestAvx2(query, to) : NearestScalar(query, to);
            if (m >= 2 && nearest.best * RATIO_DENOMINATOR < nearest.second * RATIO_NUMERATOR)
            {
                TMatch match = { from.X(i), from.Y(i), to.X(nearest.bestIndex), to.Y(nearest.bestIndex) };
                matches.push_back(match);
            }
        }
        if (matches.size() < MATCHES_MIN)
            return false;

        // The same pairs for the same images: a fixed seed.
        uint32_t state = 0x9E3779B9u;
        auto next = [&state](size_t range)
        {
            state = state * 1664525u + 1013904223u;
            return size_t((uint64_t(state) * range) >> 32);
        };
        std::vector<size_t> sample(2), inliers, best;
        TAffine t;
        int rounds = RANSAC_ROUNDS_MAX;
        for (int round = 0; round < rounds; ++round)
        {
            sample[0] = next(matches.size());
            sample[1] = next(matches.size());
            if (sample[0] == sample[1] || !FitSimilarity(matches, sample, t) || !Plausible(t))
                continue;
            Inliers(matches, t, inliers);
            if (inliers.size() > best.size())
            {
                best.swap(inliers);
                const double share = double(best.size()) / matches.size();
                const double miss = 1 - share * share;
                if (miss <= 0)
                    break;
                rounds = std::min(rounds, int(ceil(log(1 - RANSAC_CONFIDENCE) / log(miss))));
            }
        }
        if (best.size() < INLIERS_MIN)
            return false;
        // Twice: the fit to all inliers finds a few more.
        for (int i = 0; i < 2; ++i)
        {
            if (!FitSimilarity(matches, best, t))
                return false;
            Inliers(matches, t, inliers);
            if (inliers.size() < INLIERS_MIN)
                return false;
            best.swap(inliers);
        }
        if (!Plausible(t))
            return false;
        transform = t;
        if (pInliers)
            *pInliers = best.size();
        return true;
    }

    double MatchingSeconds(size_t keypoints, size_t threads)
    {
        // Two sets of random descriptions, the same for every call.
        TKeypoints sets[2];
        uint64_t state = 0x9E3779B97F4A7C15ull;
        for (TKeypoints & set : sets)
        {
            std::vector<uint64_t> bits(keypoints * KEYPOINT_WORDS);
            for (uint64_t & word : bits)
            {
                state ^= state << 13;
                state ^= state >> 7;
                state ^= state << 17;
                word = state;
            }
            set.width = set.height = 1000;
            set.x.assign(keypoints, 0);
            set.y.assign(keypoints, 0);
            set.SetDescriptions(bits);
        }
        threads = std::max<size_t>(threads, 1);
        // Enough pairs per thread that starting the threads costs little
        // beside them: a small level's pair takes under a millisecond.
        const size_t repeats = std::max<size_t>(1, (size_t(16) << 20) / (keypoints * keypoints + 1));
        // The faster of two rounds: the first can meet the threads' start.
        double best = 0;
        for (int round = 0; round < 2; ++round)
        {
            const auto start = std::chrono::steady_clock::now();
            std::vector<std::thread> workers;
            for (size_t i = 0; i < threads; ++i)
                workers.emplace_back([&sets, repeats]()
                {
                    TAffine t;
                    for (size_t r = 0; r < repeats; ++r)
                        FindTransform(sets[0], sets[1], t, NULL);
                });
            for (std::thread & worker : workers)
                worker.join();
            const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
            best = round == 0 ? seconds : std::min(best, seconds);
        }
        return best / (threads * repeats);
    }
    //-------------------------------------------------------------------------
    TBox OverlapBox(const TAffine & transform, size_t fixedWidth, size_t fixedHeight, size_t movingWidth, size_t movingHeight)
    {
        // Moving's corners in fixed's pixels; the second smallest and second
        // largest of each axis bound what a small turn leaves covered.
        const TAffine inverse = transform.Inverse();
        const double cornersX[4] = { 0, double(movingWidth), double(movingWidth), 0 };
        const double cornersY[4] = { 0, 0, double(movingHeight), double(movingHeight) };
        double xs[4], ys[4];
        for (int i = 0; i < 4; ++i)
        {
            xs[i] = inverse.a * cornersX[i] + inverse.b * cornersY[i] + inverse.c;
            ys[i] = inverse.d * cornersX[i] + inverse.e * cornersY[i] + inverse.f;
        }
        std::sort(xs, xs + 4);
        std::sort(ys, ys + 4);
        TBox box;
        box.left = std::max<ptrdiff_t>(0, ptrdiff_t(ceil(xs[1])) + 1);
        box.top = std::max<ptrdiff_t>(0, ptrdiff_t(ceil(ys[1])) + 1);
        box.right = std::min<ptrdiff_t>(fixedWidth, ptrdiff_t(floor(xs[2])) - 1);
        box.bottom = std::min<ptrdiff_t>(fixedHeight, ptrdiff_t(floor(ys[2])) - 1);
        if (box.right < box.left)
            box.right = box.left;
        if (box.bottom < box.top)
            box.bottom = box.top;
        return box;
    }

    static void WarpBilinear(const uint8_t * src, int srcWidth, int srcHeight, ptrdiff_t srcStride,
        const TAffine & t, TPlane & dst)
    {
        for (int y = 0; y < dst.height; ++y)
        {
            uint8_t * q = dst.Row(y);
            for (int x = 0; x < dst.width; ++x)
            {
                const double sx = std::min(srcWidth - 1.001, std::max(0.0, t.a * x + t.b * y + t.c));
                const double sy = std::min(srcHeight - 1.001, std::max(0.0, t.d * x + t.e * y + t.f));
                const int ix = int(sx), iy = int(sy);
                const double fx = sx - ix, fy = sy - iy;
                const uint8_t * p = src + iy * srcStride + ix;
                const double top = p[0] + fx * (p[1] - p[0]);
                const double bottom = p[srcStride] + fx * (p[srcStride + 1] - p[srcStride]);
                q[x] = uint8_t(top + fy * (bottom - top) + 0.5);
            }
        }
    }

    // A patch's shift between two planes of the same size, to a fraction of
    // a pixel: the normalised correlation's peak, a parabola through it.
    static bool PatchShift(const uint8_t * fixed, ptrdiff_t fixedStride, const TPlane & warped,
        int x, int y, int radius, double * pShiftX, double * pShiftY)
    {
        const int n = REFINE_PATCH, side = 2 * radius + 1;
        double sum = 0, sumSquares = 0;
        for (int j = 0; j < n; ++j)
        {
            const uint8_t * a = fixed + (y + j) * fixedStride + x;
            for (int i = 0; i < n; ++i)
            {
                sum += a[i];
                sumSquares += double(a[i]) * a[i];
            }
        }
        const double count = double(n) * n;
        const double fixedVariance = sumSquares - sum * sum / count;
        if (fixedVariance <= 0)
            return false;

        std::vector<double> correlation(size_t(side) * side);
        int peakX = 0, peakY = 0;
        double peak = -2;
        for (int dy = -radius; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx)
            {
                int cross = 0, s = 0, ss = 0;
                for (int j = 0; j < n; ++j)
                {
                    const uint8_t * a = fixed + (y + j) * fixedStride + x;
                    const uint8_t * b = warped.Row(y + j + dy) + x + dx;
                    int rowCross = 0, rowSum = 0, rowSquares = 0;
                    for (int i = 0; i < n; ++i)
                    {
                        rowCross += a[i] * b[i];
                        rowSum += b[i];
                        rowSquares += b[i] * b[i];
                    }
                    cross += rowCross;
                    s += rowSum;
                    ss += rowSquares;
                }
                const double variance = double(ss) - double(s) * s / count;
                const double covariance = double(cross) - sum * s / count;
                const double value = variance > 0 ? covariance / sqrt(fixedVariance * variance) : -1;
                correlation[size_t(dy + radius) * side + dx + radius] = value;
                if (value > peak)
                {
                    peak = value;
                    peakX = dx + radius;
                    peakY = dy + radius;
                }
            }
        }
        if (peak < REFINE_CORRELATION_MIN || peakX == 0 || peakY == 0 || peakX == side - 1 || peakY == side - 1)
            return false;
        const double * c = correlation.data() + size_t(peakY) * side + peakX;
        const double curveX = c[-1] - 2 * c[0] + c[1], curveY = c[-side] - 2 * c[0] + c[side];
        if (curveX >= 0 || curveY >= 0)
            return false;
        *pShiftX = peakX - radius + (c[-1] - c[1]) / (2 * curveX);
        *pShiftY = peakY - radius + (c[-side] - c[side]) / (2 * curveY);
        return true;
    }

    // Whether a patch has texture in both directions: the smaller eigenvalue
    // of its gradients' moment matrix. An edge alone can slide along itself.
    static bool Textured(const uint8_t * fixed, ptrdiff_t stride, int x, int y)
    {
        double xx = 0, yy = 0, xy = 0;
        for (int j = 0; j < REFINE_PATCH; ++j)
        {
            const uint8_t * p = fixed + (y + j) * stride + x;
            for (int i = 0; i < REFINE_PATCH; ++i, ++p)
            {
                const int dx = (p[1 - stride] - p[-1 - stride]) + 2 * (p[1] - p[-1]) + (p[1 + stride] - p[-1 + stride]);
                const int dy = (p[stride - 1] - p[-stride - 1]) + 2 * (p[stride] - p[-stride]) + (p[stride + 1] - p[-stride + 1]);
                xx += double(dx) * dx;
                yy += double(dy) * dy;
                xy += double(dx) * dy;
            }
        }
        const double half = (xx + yy) / 2, root = sqrt(std::max(0.0, half * half - (xx * yy - xy * xy)));
        return (half - root) / (REFINE_PATCH * REFINE_PATCH) >= REFINE_TEXTURE_MIN;
    }

    struct TPatchShift
    {
        double x, y, shiftX, shiftY;
    };

    // Solves the three normal equations of an affine fit of one coordinate.
    static bool Solve3(double m[3][4], double out[3])
    {
        for (int col = 0; col < 3; ++col)
        {
            int pivot = col;
            for (int row = col + 1; row < 3; ++row)
                if (fabs(m[row][col]) > fabs(m[pivot][col]))
                    pivot = row;
            if (fabs(m[pivot][col]) < 1e-9)
                return false;
            if (pivot != col)
                for (int i = 0; i < 4; ++i)
                    std::swap(m[pivot][i], m[col][i]);
            for (int row = 0; row < 3; ++row)
            {
                if (row == col)
                    continue;
                const double factor = m[row][col] / m[col][col];
                for (int i = col; i < 4; ++i)
                    m[row][i] -= factor * m[col][i];
            }
        }
        for (int i = 0; i < 3; ++i)
            out[i] = m[i][3] / m[i][i];
        return true;
    }

    // The affine map x -> x + shift(x) that the patches agree on, the ones
    // far off dropped: those lie where the two pictures differ.
    static bool FitShifts(const std::vector<TPatchShift> & patches, TAffine & t)
    {
        std::vector<char> keep(patches.size(), 1);
        std::vector<double> errors;
        double cx[3] = { 0 }, cy[3] = { 0 };
        for (int round = 0; round < 3; ++round)
        {
            double ax[3][4] = { { 0 } }, ay[3][4] = { { 0 } };
            size_t kept = 0;
            for (size_t i = 0; i < patches.size(); ++i)
            {
                if (!keep[i])
                    continue;
                ++kept;
                const double v[3] = { patches[i].x, patches[i].y, 1.0 };
                for (int r = 0; r < 3; ++r)
                {
                    for (int c = 0; c < 3; ++c)
                        ax[r][c] += v[r] * v[c];
                    ax[r][3] += v[r] * patches[i].shiftX;
                    ay[r][3] += v[r] * patches[i].shiftY;
                }
            }
            if (kept < REFINE_PATCHES_MIN)
                return false;
            for (int r = 0; r < 3; ++r)
                for (int c = 0; c < 3; ++c)
                    ay[r][c] = ax[r][c];
            if (!Solve3(ax, cx) || !Solve3(ay, cy))
                return false;

            errors.clear();
            std::vector<double> all(patches.size());
            for (size_t i = 0; i < patches.size(); ++i)
            {
                const TPatchShift & p = patches[i];
                all[i] = hypot(cx[0] * p.x + cx[1] * p.y + cx[2] - p.shiftX, cy[0] * p.x + cy[1] * p.y + cy[2] - p.shiftY);
                if (keep[i])
                    errors.push_back(all[i]);
            }
            std::nth_element(errors.begin(), errors.begin() + errors.size() / 2, errors.end());
            const double limit = std::max(REFINE_OUTLIER_MIN, 2.5 * errors[errors.size() / 2]);
            for (size_t i = 0; i < patches.size(); ++i)
                keep[i] = all[i] < limit;
        }
        t.a = 1 + cx[0];
        t.b = cx[1];
        t.c = cx[2];
        t.d = cy[0];
        t.e = 1 + cy[1];
        t.f = cy[2];
        return true;
    }

    // Whether a correction moves the picture's corners about as far as the
    // patches could have seen: patches all in one corner fit the others badly.
    static bool WithinReach(const TAffine & t, size_t width, size_t height, int radius)
    {
        for (int i = 0; i < 4; ++i)
        {
            const double x = (i & 1) ? double(width) : 0, y = (i & 2) ? double(height) : 0;
            if (hypot(t.a * x + t.b * y + t.c - x, t.d * x + t.e * y + t.f - y) > REFINE_REACH * radius)
                return false;
        }
        return true;
    }

    void RefineTransform(const uint8_t * fixed, size_t fixedWidth, size_t fixedHeight, ptrdiff_t fixedStride,
        const uint8_t * moving, size_t movingWidth, size_t movingHeight, ptrdiff_t movingStride, TAffine & transform)
    {
        TPlane warped;
        warped.Resize(int(fixedWidth), int(fixedHeight));
        std::vector<TPatchShift> patches;
        for (const int radius : REFINE_RADII)
        {
            const TBox box = OverlapBox(transform, fixedWidth, fixedHeight, movingWidth, movingHeight);
            WarpBilinear(moving, int(movingWidth), int(movingHeight), movingStride, transform, warped);
            patches.clear();
            const int margin = radius + 1;
            for (int y = int(box.top) + margin; y + REFINE_PATCH + margin < box.bottom; y += REFINE_STEP)
            {
                for (int x = int(box.left) + margin; x + REFINE_PATCH + margin < box.right; x += REFINE_STEP)
                {
                    if (!Textured(fixed, fixedStride, x, y))
                        continue;
                    TPatchShift patch = { x + REFINE_PATCH / 2.0, y + REFINE_PATCH / 2.0, 0, 0 };
                    if (PatchShift(fixed, fixedStride, warped, x, y, radius, &patch.shiftX, &patch.shiftY))
                        patches.push_back(patch);
                }
            }
            TAffine correction;
            if (patches.size() < REFINE_PATCHES_MIN || !FitShifts(patches, correction) ||
                !WithinReach(correction, fixedWidth, fixedHeight, radius))
                return;
            transform = transform.After(correction);
        }
    }
    //-------------------------------------------------------------------------
    // Keys' cubic with a = -0.75, the one OpenCV's bicubic uses.
    static inline void CubicWeights(double t, float * w)
    {
        const double a = -0.75;
        const double t1 = t + 1, t2 = 1 - t, t3 = 2 - t;
        w[0] = float(((a * t1 - 5 * a) * t1 + 8 * a) * t1 - 4 * a);
        w[1] = float(((a + 2) * t - (a + 3)) * t * t + 1);
        w[2] = float(((a + 2) * t2 - (a + 3)) * t2 * t2 + 1);
        w[3] = float(((a * t3 - 5 * a) * t3 + 8 * a) * t3 - 4 * a);
    }

    void WarpBicubic(const uint8_t * moving, size_t movingWidth, size_t movingHeight, ptrdiff_t movingStride,
        size_t channels, const TAffine & t, uint8_t * dst, size_t dstWidth, size_t dstHeight, ptrdiff_t dstStride)
    {
        const int w = int(movingWidth), h = int(movingHeight), n = int(channels);
        for (size_t y = 0; y < dstHeight; ++y)
        {
            uint8_t * q = dst + y * dstStride;
            for (size_t x = 0; x < dstWidth; ++x, q += n)
            {
                const double sx = t.a * x + t.b * y + t.c, sy = t.d * x + t.e * y + t.f;
                const int ix = int(floor(sx)), iy = int(floor(sy));
                float wx[4], wy[4];
                CubicWeights(sx - ix, wx);
                CubicWeights(sy - iy, wy);
                int columns[4];
                for (int i = 0; i < 4; ++i)
                    columns[i] = std::min(w - 1, std::max(0, ix - 1 + i)) * n;
                for (int c = 0; c < n; ++c)
                {
                    float sum = 0;
                    for (int j = 0; j < 4; ++j)
                    {
                        const uint8_t * row = moving + std::min(h - 1, std::max(0, iy - 1 + j)) * movingStride + c;
                        sum += wy[j] * (wx[0] * row[columns[0]] + wx[1] * row[columns[1]] + wx[2] * row[columns[2]] + wx[3] * row[columns[3]]);
                    }
                    q[c] = uint8_t(std::min(255.0f, std::max(0.0f, sum + 0.5f)));
                }
            }
        }
    }
    //-------------------------------------------------------------------------
}
