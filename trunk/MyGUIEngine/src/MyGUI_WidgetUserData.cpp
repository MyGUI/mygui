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

	UserData::UserData()
	{
	}

	UserData::~UserData()
	{
	}

	void UserData::setUserString(const std::string& _key, const std::string& _value)
	{
		mMapUserString[_key] = _value;
	}

	/** Get user string or "" if not found */
	const std::string& UserData::getUserString(const std::string& _key) const
	{
		MapString::const_iterator iter = mMapUserString.find(_key);
		if (iter != mMapUserString.end())
			return iter->second;
		return Constants::getEmptyString();
	}

	const MapString& UserData::getUserStrings() const
	{
		return mMapUserString;
	}

	bool UserData::clearUserString(const std::string& _key)
	{
		MapString::iterator iter = mMapUserString.find(_key);
		if (iter != mMapUserString.end())
		{
			mMapUserString.erase(iter);
			return true;
		}
		return false;
	}

	bool UserData::isUserString(const std::string& _key) const
	{
		return mMapUserString.find(_key) != mMapUserString.end();
	}

	void UserData::clearUserStrings()
	{
		mMapUserString.clear();
	}

	void UserData::setUserData(Any _data)
	{
		mUserData = _data;
	}

	void UserData::_setInternalData(Any _data)
	{
		mInternalData = _data;
	}

} // namespace MyGUI
