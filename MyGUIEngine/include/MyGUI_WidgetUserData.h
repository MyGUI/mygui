/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
		ValueType* getUserData(bool _throw = true) const
		{
			return mUserData.castType<ValueType>(_throw);
		}

		/*internal:*/
		void _setInternalData(Any _data);

		template <typename ValueType>
		ValueType* _getInternalData(bool _throw = true) const
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
