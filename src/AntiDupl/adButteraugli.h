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
#ifndef __adButteraugli_h__
#define __adButteraugli_h__

#include <list>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

#include "adAlign.h"
#include "adConfig.h"
#include "adStrings.h"

namespace ad
{
    struct TImageData;
    struct TOptions;
    class TEngine;
    typedef TImageData* TImageDataPtr;
    //-------------------------------------------------------------------------
    // What the Butteraugli comparers of one search share: the images decoded
    // at the working size, and the scores of pairs, kept in the user folder
    // from one search to the next. The compare threads call Score at once.
    // The aligned search adds each image's keypoints and their index.
    class TButteraugliStore
    {
    public:
        TButteraugliStore(TEngine *pEngine, size_t imageCount);
        ~TButteraugliStore(); // saves the scores

        // Butteraugli's score for the pair, the first image turned as
        // TImageComparer::Accept turns its reduced images; negative when
        // either file can't be decoded. The same for (a, b, t) and
        // (b, a, inverse of t).
        double Score(TImageDataPtr pFirst, TImageDataPtr pSecond, adTransformType transform);

        // Whether this search wants every image's keypoints, which takes
        // decoding the images the image database would otherwise spare.
        bool NeedsKeypoints() const { return m_pIndex != NULL; }

        // An image's keypoints, from its grey pixels at full size.
        void SetKeypoints(TImageDataPtr pImageData, const TView & gray);

        // The aligned search's candidates, in three steps, so that they don't
        // depend on the order images arrive in: each compare thread joins
        // (its comparer, when made) and queues its images for the keypoint
        // index as they come; when its queue is done, it waits in Gather
        // until every thread has come, and the last one builds the index in
        // path order; then each looks its own images up in the whole index
        // (Candidates) and scores a pair only if it claims it first, as
        // both images of a pair may find each other.
        void Join();
        void Queue(TImageDataPtr pImageData);
        // False when the search was stopped while waiting.
        bool Gather();
        void Candidates(TImageDataPtr pImageData, std::vector<TImageDataPtr> & candidates);
        bool Claim(TImageDataPtr pFirst, TImageDataPtr pSecond);

        // Butteraugli's score for what the two images show in common, each
        // laid over the other; negative when they don't align, share too
        // little, or differ too much there for a score to be worth its time.
        double AlignedScore(TImageDataPtr pFirst, TImageDataPtr pSecond);

    private:
        // B, G, R bytes, the longest side at most the working size.
        struct TWorkImage
        {
            std::once_flag decoded;
            std::unique_ptr<TView> bgr; // NULL when the file didn't decode
            bool counted = false; // its bytes are in m_imageBytes
        };
        typedef std::shared_ptr<TWorkImage> TWorkImagePtr;
        struct TCached
        {
            TWorkImagePtr image;
            std::list<TImageDataPtr>::iterator use;
        };

        // A pair's identity on disk: both files' content and size, and what
        // else the score depends on. No padding: written to the file as it is.
        struct TKey
        {
            TUInt64 firstSize, secondSize;
            TUInt32 firstCrc, secondCrc;
            TInt32 transform, workingSize, frame, reserved;
            bool operator==(const TKey & key) const;
        };
        struct TKeyHash
        {
            size_t operator()(const TKey & key) const;
        };

        TWorkImagePtr Image(TImageDataPtr pImageData);
        void Decode(TImageDataPtr pImageData, TWorkImage & image) const;
        double Compute(const TView & first, const TView & second, adTransformType transform) const;
        double ComputeAligned(const TView & first, const TView & second, const TAffine & transform) const;
        std::shared_ptr<const TKeypoints> Keypoints(TImageDataPtr pImageData);
        void Load();
        void Save() const;

        const TOptions *m_pOptions;
        TString m_path;
        std::mutex m_mutex;
        std::unordered_map<TImageDataPtr, TCached> m_images;
        std::list<TImageDataPtr> m_uses; // the most recently used first
        size_t m_imageBytes;
        std::unordered_map<TKey, double, TKeyHash> m_scores;
        bool m_changed;

        std::mutex m_keypointMutex;
        std::unordered_map<TImageDataPtr, std::shared_ptr<const TKeypoints>> m_keypoints;
        TEngine *m_pEngine;
        std::mutex m_indexMutex;
        std::condition_variable m_gathered;
        size_t m_participants, m_arrived;
        bool m_indexBuilt;
        std::unique_ptr<TKeypointIndex> m_pIndex; // NULL but in the aligned search
        std::vector<TImageDataPtr> m_indexed; // by the index's image numbers, in path order once built
        std::unordered_map<TImageDataPtr, uint32_t> m_number; // the index's number of each image
        std::unordered_set<uint64_t> m_claimed; // pairs of index numbers, the smaller first
    };
    //-------------------------------------------------------------------------
}
#endif//__adButteraugli_h__
