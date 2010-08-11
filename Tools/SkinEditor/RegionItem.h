/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_ITEM_H__
#define __REGION_ITEM_H__

#include <MyGUI.h>
#include "PropertySet.h"

namespace tools
{

	class RegionItem
	{
	public:
		RegionItem();
		~RegionItem();

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		PropertySet* getPropertySet();

	private:
		MyGUI::UString mName;
		PropertySet* mPropertySet;
	};

} // namespace tools

#endif // __REGION_ITEM_H__
