/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_ENUMERATOR_H__
#define __MYGUI_ENUMERATOR_H__

#include "assert.h"

namespace MyGUI
{

	/** Enumerator usage
	example :

		typedef std::vector<std::string> VectorString;
		typedef Enumerator<VectorString> EnumeratorVectorString;

		VectorString vec;
		vec.push_back("value");
		EnumeratorVectorString enum_vec(vec.begin(), vec.end());
		while (enum_vec.next()) {
			std::string value = enum_vec.current();
		};

		typedef std::pair<std::string, std::string> PairString;
		typedef std::map<PairString> MapString;

		MapString map;
		map["key"] = "value";
		EnumeratorMapString enum_map(map.begin(), map.end());
		while (enum_map.next()) {
			std::string key = enum_map.current().first;
			std::string value = enum_map.current().second;
		};

	*/

	template<class T>
	class Enumerator
	{
	private:
		Enumerator() { }

	public:
		Enumerator(typename T::iterator _first, typename T::iterator _end) :
			m_first(true),
			m_current(_first),
			m_end(_end)
		{
		}

		bool next()
		{
			if (m_current == m_end) return false;
			else if (m_first) {
				m_first = false;
				return true;
			}
			++ m_current;
			if (m_current == m_end) return false;
			return true;
		}

		inline typename T::value_type operator->() const {assert(m_current != m_end); return (*m_current);}
		inline typename T::value_type current() {assert(m_current != m_end); return (*m_current);}

	private:
		bool m_first;
		typename T::iterator m_current;
		typename T::iterator m_end;
	};

} // namespace MyGUI

#endif // __MYGUI_ENUMERATOR_H__
