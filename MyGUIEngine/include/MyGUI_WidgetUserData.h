/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_WIDGET_USER_DATA_H__
#define __MYGUI_WIDGET_USER_DATA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	/** UserData is parent of Widget class. Used to store any user data and strings inside widget */
	class MYGUI_EXPORT UserData
	{
	public:
		UserData();
		virtual ~UserData();

		/** Set user string */
		void setUserString(const std::string& _key, const std::string& _value);

		/** Get user string or "" if not found */
		const std::string& getUserString(const std::string& _key) const;

		/** Get map of all user strings */
		const MapString& getUserStrings() const;

		/** Delete user string */
		bool clearUserString(const std::string& _key);

		/** Return true if user string with such key exist */
		bool isUserString(const std::string& _key) const;

		/** Delete all user strings */
		void clearUserStrings();

		/** Set any user data to store inside widget */
		void setUserData(Any _data);

		/** Get user data and cast it to ValueType */
		template <typename ValueType>
		ValueType* getUserData(bool _throw = true)
		{
			return mUserData.castType<ValueType>(_throw);
		}

		/*internal:*/
		void _setInternalData(Any _data);

		template <typename ValueType>
		ValueType* _getInternalData(bool _throw = true)
		{
			return mInternalData.castType<ValueType>(_throw);
		}

	private:
		// пользовательские данные
		MapString mMapUserString;
		Any mUserData;

		// для внутренниего использования
		Any mInternalData;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_USER_DATA_H__
