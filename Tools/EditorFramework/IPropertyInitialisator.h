/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _e1352c42_fc8c_43d0_90a9_b2bc8bb0deb6_
#define _e1352c42_fc8c_43d0_90a9_b2bc8bb0deb6_

#include <memory>
#include "IFactoryItem.h"

namespace tools
{

	class Property;

	class MYGUI_EXPORT_DLL IPropertyInitialisator : public components::IFactoryItem
	{
	public:
		virtual void initialise(std::shared_ptr<Property> _property) = 0;
	};

}

#endif
