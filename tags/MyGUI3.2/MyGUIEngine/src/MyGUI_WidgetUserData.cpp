/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
