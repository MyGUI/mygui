/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ENUMERATOR_H_
#define MYGUI_ENUMERATOR_H_

#include <assert.h>

namespace MyGUI
{

	/** Class for comfortable using of vectors with small while loop
	instead iterators. Performance is same as with iterators.
	Enumerator usage
	@example Enumerator
	@code
		typedef std::vector<std::string> VectorString;
		typedef Enumerator<VectorString> EnumeratorVectorString;

		VectorString vec;
		vec.push_back("value");
		//EnumeratorVectorString enum_vec(vec.begin(), vec.end());
		EnumeratorVectorString enum_vec(vec);
		while (enum_vec.next())
		{
			std::string value = enum_vec.current();
		}

		typedef std::pair<std::string, std::string> PairString;
		typedef std::map<PairString> MapString;

		MapString map;
		map["key"] = "value";
		//EnumeratorMapString enum_map(map.begin(), map.end());
		EnumeratorMapString enum_map(map);
		while (enum_map.next())
		{
			std::string key = enum_map.current().first;
			std::string value = enum_map.current().second;
		}
	@endcode
	*/

	template<typename T>
	class Enumerator
	{
	private:
		Enumerator()
		{
		}

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
			if (m_current == m_end)
				return false;
			else if (m_first)
			{
				m_first = false;
				return true;
			}
			++ m_current;
			if (m_current == m_end)
				return false;
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

} // namespace MyGUI

#endif // MYGUI_ENUMERATOR_H_
