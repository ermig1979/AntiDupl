#ifndef __adWebp_h__
#define __adWebp_h__

#include "adImage.h"

namespace ad
{
	class TWebp : public TImage
	{
	public:
		static TWebp* Load(HGLOBAL hGlobal);
		static bool Supported(HGLOBAL hGlobal);

		  private:
	};
}

#endif//__adWebp_h__
