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
		typedef std::map<std::string, int> MapInt;

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

		inline void clearUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter != mMapUserString.end()) mMapUserString.erase(iter);
		}

		inline void clearUserString()
		{
			mMapUserString.clear();
		}

		// пользовательские данные виджета int
		inline void setUserData(const std::string & _key, int _value)
		{
			mMapUserInt[_key] = _value;
		}

		int getUserData(const std::string & _key)
		{
			MapInt::iterator iter = mMapUserInt.find(_key);
			if (iter == mMapUserInt.end()) return 0;
			return iter->second;
		}

		inline void clearUserData(const std::string & _key)
		{
			MapInt::iterator iter = mMapUserInt.find(_key);
			if (iter != mMapUserInt.end()) mMapUserInt.erase(iter);
		}

		inline void clearUserData()
		{
			mMapUserInt.clear();
		}

		inline int getInternalData() {return mInternalData;}
		inline void setInternalData(int _data) {mInternalData = _data;}

	private:
		// пользовательские данные
		MapString mMapUserString;
		MapInt mMapUserInt;
		// номер для внутренниего или внешнего использования
		int mInternalData;
	};
} // namespace MyGUI

#endif // __MYGUI_WIDGET_USER_DATA_H__