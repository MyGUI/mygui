/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_WidgetUserData.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	void UserData::setUserString(std::string_view _key, std::string_view _value)
	{
		mapSet(mMapUserString, _key, _value);
	}

	/** Get user string or "" if not found */
	std::string_view UserData::getUserString(std::string_view _key) const
	{
		MapString::const_iterator iter = mMapUserString.find(_key);
		if (iter != mMapUserString.end())
			return iter->second;
		return {};
	}

	const MapString& UserData::getUserStrings() const
	{
		return mMapUserString;
	}

	bool UserData::clearUserString(std::string_view _key)
	{
		MapString::iterator iter = mMapUserString.find(_key);
		if (iter != mMapUserString.end())
		{
			mMapUserString.erase(iter);
			return true;
		}
		return false;
	}

	bool UserData::isUserString(std::string_view _key) const
	{
		return mMapUserString.find(_key) != mMapUserString.end();
	}

	void UserData::clearUserStrings()
	{
		mMapUserString.clear();
	}

	void UserData::setUserData(Any _data)
	{
		mUserData = std::move(_data);
	}

	void UserData::_setInternalData(Any _data)
	{
		mInternalData = std::move(_data);
	}

} // namespace MyGUI
