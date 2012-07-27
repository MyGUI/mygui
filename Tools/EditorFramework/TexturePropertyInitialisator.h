/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _1628df4a_ba95_4128_a0af_236fa36a402b_
#define _1628df4a_ba95_4128_a0af_236fa36a402b_

#include "IPropertyInitialisator.h"
#include "Property.h"

namespace tools
{

	class MYGUI_EXPORT_DLL TexturePropertyInitialisator :
		public IPropertyInitialisator
	{
	public:
		TexturePropertyInitialisator();
		virtual ~TexturePropertyInitialisator();

		virtual void initialise(PropertyPtr _property);
	};

}

#endif
