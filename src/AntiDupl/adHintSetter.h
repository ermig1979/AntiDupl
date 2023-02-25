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
#ifndef __adHintSetter_h__
#define __adHintSetter_h__

#include "adConfig.h"


namespace ad
{
    struct TOptions;
    struct TResult;
    //-------------------------------------------------------------------------
	// Общий класс установшика подсказок
    class THintSetter
    {
	protected:
        TOptions *m_pOptions;
	public:
        THintSetter(TOptions *pOptions);
        virtual void Execute(TResult *pResult, bool canRename) const; //можно переопределить в наследнике
    };

	//-------------------------------------------------------------------------
    class THintSetter_Algorithm : public THintSetter
    {
	private:
        double m_autoDeleteThresholdDifference;
		adInt32 m_blockinessThreshold;
    public:
		THintSetter_Algorithm(TOptions *pOptions);
		virtual void Execute(TResult *pResult, bool canRename) const; //переопределяем
	};

    //-------------------------------------------------------------------------
	class THintSetterStorage
    {
	private:
		static THintSetter* m_hintSetter_pointer; //its just a declaration, not a definition!
    public:
		static THintSetter* GetHintSetterPointer(TOptions *pOptions);
	};

    //-------------------------------------------------------------------------
}

#endif/*__adHintSetter_h__*/