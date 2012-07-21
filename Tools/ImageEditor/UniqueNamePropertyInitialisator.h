/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _3757dc38_eb2a_4169_a6a6_a3284b230e20_
#define _3757dc38_eb2a_4169_a6a6_a3284b230e20_

#include "IPropertyInitialisator.h"
#include "Property.h"

namespace tools
{

	class UniqueNamePropertyInitialisator :
		public IPropertyInitialisator
	{
	public:
		UniqueNamePropertyInitialisator();
		virtual ~UniqueNamePropertyInitialisator();

		virtual void initialise(Property* _property);
	};

}

#endif
