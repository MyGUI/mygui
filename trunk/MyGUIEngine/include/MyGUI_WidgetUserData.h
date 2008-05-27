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

namespace MyGUI
{
	class _MyGUIExport UserData
	{
	public:
		UserData() : mInternalData(0), mUserData(0) {}
		virtual ~UserData() {}

		// пользовательские данные виджета строки
		inline void setUserString(const std::string & _key, const std::string & _value)
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

		inline bool clearUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter != mMapUserString.end()) {
				mMapUserString.erase(iter);
				return true;
			}
			return false;
		}

		inline bool isUserString(const std::string & _key)
		{
			return mMapUserString.find(_key) != mMapUserString.end();
		}

		inline void clearUserStrings()
		{
			mMapUserString.clear();
		}

		inline int _getInternalData() {return mInternalData;}
		inline void _setInternalData(int _data) {mInternalData = _data;}

		inline const std::string& _getInternalString() {return mInternalString;}
		inline void _setInternalString(const std::string& _data) {mInternalString = _data;}

		inline void setUserData(void * _data) { mUserData = _data; }
		inline void * getUserData() { return mUserData; }

	private:
		// пользовательские данные
		MapString mMapUserString;
		// номер для внутренниего использования
		int mInternalData;
		std::string mInternalString;

		void * mUserData;
	};
} // namespace MyGUI

#endif // __MYGUI_WIDGET_USER_DATA_H__
