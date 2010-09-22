/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__

#include "assert.h"

namespace utility
{

	template<typename T>
	class Enumerator
	{
	private:
		Enumerator() { }

	public:
		explicit Enumerator(const T& _container) :
			m_first(true),
			m_current(_container.begin()),
			m_end(_container.end())
		{
		}

		Enumerator(typename T::const_iterator _first, typename T::const_iterator _end) :
			m_first(true),
			m_current(_first),
			m_end(_end)
		{
		}

		bool next()
		{
			if (m_current == m_end) return false;
			else if (m_first)
			{
				m_first = false;
				return true;
			}
			++ m_current;
			if (m_current == m_end) return false;
			return true;
		}

		typename T::const_reference operator->() const
		{
			assert(m_current != m_end);
			return (*m_current);
		}
		typename T::const_reference current()
		{
			assert(m_current != m_end);
			return (*m_current);
		}

	private:
		bool m_first;
		typename T::const_iterator m_current;
		typename T::const_iterator m_end;
	};

} // namespace utility

#endif // __ENUMERATOR_H__
