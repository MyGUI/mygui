/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _2e0cd43f_7d3e_4e45_abd7_8a1a47c55275_
#define _2e0cd43f_7d3e_4e45_abd7_8a1a47c55275_

#include "IPropertyInitialisator.h"
#include "Property.h"

namespace tools
{

	class GenerateFontNamePropertyInitialisator :
		public IPropertyInitialisator
	{
	public:
		GenerateFontNamePropertyInitialisator();
		virtual ~GenerateFontNamePropertyInitialisator();

		virtual void initialise(PropertyPtr _property);
	};

}

#endif
