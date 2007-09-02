#pragma once

#include "WidgetDefines.h"

namespace widget
{
	class UserData
	{
	public:

		// ���������������� ������ ������� ������
		inline void setUserString(const std::string & _key, const std::string & _value)
		{
			m_mapUserString[_key] = _value;
		}

		const std::string & getUserString(const std::string & _key)
		{
			MapString::iterator iter = m_mapUserString.find(_key);
			if (iter == m_mapUserString.end()) {
				static std::string empty;
				return empty;
			}
			return iter->second;
		}

		inline void clearUserString(const std::string & _key)
		{
			MapString::iterator iter = m_mapUserString.find(_key);
			if (iter != m_mapUserString.end()) m_mapUserString.erase(iter);
		}

		inline void clearUserString()
		{
			m_mapUserString.clear();
		}

		// ���������������� ������ ������� int
		inline void setUserData(const std::string & _key, int _value)
		{
			m_mapUserInt[_key] = _value;
		}

		const int getUserData(const std::string & _key)
		{
			MapInt::iterator iter = m_mapUserInt.find(_key);
			if (iter == m_mapUserInt.end()) return 0;
			return iter->second;
		}

		inline void clearUserData(const std::string & _key)
		{
			MapInt::iterator iter = m_mapUserInt.find(_key);
			if (iter != m_mapUserInt.end()) m_mapUserInt.erase(iter);
		}

		inline void clearUserData()
		{
			m_mapUserInt.clear();
		}

	private:
		// ���������������� ������
		MapString m_mapUserString;
		MapInt m_mapUserInt;
	};
} // namespace widget