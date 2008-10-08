/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_USER_DATA_H__
#define __MYGUI_WIDGET_USER_DATA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_Any.h"

namespace MyGUI
{
	class _MyGUIExport UserData
	{
	public:
		UserData() { }
		virtual ~UserData() { }

		// пользовательские данные виджета строки
		void setUserString(const std::string & _key, const std::string & _value)
		{
			mMapUserString[_key] = _value;
		}

		const std::string & getUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter == mMapUserString.end()) {
				static std::string empty;
				return empty;
			}
			return iter->second;
		}

		bool clearUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter != mMapUserString.end()) {
				mMapUserString.erase(iter);
				return true;
			}
			return false;
		}

		bool isUserString(const std::string & _key)
		{
			return mMapUserString.find(_key) != mMapUserString.end();
		}

		void clearUserStrings()
		{
			mMapUserString.clear();
		}

		void setUserData(Any _data) { mUserData = _data; }

		template <typename ValueType>
		ValueType * getUserData(bool _throw = true)
		{
			return mUserData.castType<ValueType>(_throw);
		}


		void _setInternalData(Any _data) { mInternalData = _data; }

		template <typename ValueType>
		ValueType * _getInternalData(bool _throw = true)
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
