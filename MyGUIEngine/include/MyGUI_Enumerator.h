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
