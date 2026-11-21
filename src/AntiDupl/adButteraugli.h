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
#include <mutex>
#include <unordered_map>

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
    class TButteraugliStore
    {
    public:
        TButteraugliStore(TEngine *pEngine);
        ~TButteraugliStore(); // saves the scores

        // Butteraugli's score for the pair, the first image turned as
        // TImageComparer::Accept turns its reduced images; negative when
        // either file can't be decoded. The same for (a, b, t) and
        // (b, a, inverse of t).
        double Score(TImageDataPtr pFirst, TImageDataPtr pSecond, adTransformType transform);

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
    };
    //-------------------------------------------------------------------------
}
#endif//__adButteraugli_h__
