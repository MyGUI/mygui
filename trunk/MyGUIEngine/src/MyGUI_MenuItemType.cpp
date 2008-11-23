/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_MenuItemType.h"

namespace MyGUI
{

	/*MenuItemType::MapValue MenuItemType::mMapValue;

	MenuItemType MenuItemType::parse(const std::string & _value)
	{
		static bool first = true;
		if (first) {
			initialise();
			first = false;
		}

		MapValue::iterator iter = mMapValue.find(_value);
		if (iter != mMapValue.end()) return MenuItemType(iter->second);
		else {
			std::string value = _value;
			utility::trim(value);
			MapValue::iterator iter = mMapValue.find(value);
			if (iter != mMapValue.end()) return MenuItemType(iter->second);
			else {
				MYGUI_LOG(Warning, "Cannot parse '" << _value << "'");
			}
		}
		return MenuItemType();
	}

	void MenuItemType::initialise()
	{
		mMapValue.clear();

		MYGUI_REGISTER_VALUE(mMapValue, Normal);
		MYGUI_REGISTER_VALUE(mMapValue, Popup);
		MYGUI_REGISTER_VALUE(mMapValue, Separator);
	}

	std::string MenuItemType::print() const
	{
		if (value >= mMapValue.size()) return "";
		return (mMapValue.begin() + value)->first;
	}*/

} // namespace MyGUI
