/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TPONT_H_
#define MYGUI_TPONT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_StringUtility.h"

namespace MyGUI::types
{

	template<typename T>
	struct TPoint
	{
		T left{};
		T top{};

		TPoint() = default;

		TPoint(T const& _left, T const& _top) :
			left(_left),
			top(_top)
		{
		}

		TPoint& operator-=(TPoint const& _obj)
		{
			left -= _obj.left;
			top -= _obj.top;
			return *this;
		}

		TPoint& operator+=(TPoint const& _obj)
		{
			left += _obj.left;
			top += _obj.top;
			return *this;
		}

		TPoint operator-(TPoint const& _obj) const
		{
			return TPoint(left - _obj.left, top - _obj.top);
		}

		TPoint operator+(TPoint const& _obj) const
		{
			return TPoint(left + _obj.left, top + _obj.top);
		}

		template<typename U>
		TPoint& operator=(TPoint<U> const& _obj)
		{
			left = _obj.left;
			top = _obj.top;
			return *this;
		}

		bool operator==(TPoint const& _obj) const
		{
			return ((left == _obj.left) && (top == _obj.top));
		}

		bool operator!=(TPoint const& _obj) const
		{
			return !((left == _obj.left) && (top == _obj.top));
		}

		void clear()
		{
			left = top = 0;
		}

		void set(T const& _left, T const& _top)
		{
			left = _left;
			top = _top;
		}

		void swap(TPoint& _value)
		{
			TPoint tmp = _value;
			_value = *this;
			*this = tmp;
		}

		bool empty() const
		{
			return ((left == 0) && (top == 0));
		}

		std::string print() const
		{
			std::ostringstream stream;
			stream << *this;
			return stream.str();
		}

		static TPoint<T> parse(std::string_view _value)
		{
			return utility::parseValue<TPoint<T>>(_value);
		}

		friend std::ostream& operator<<(std::ostream& _stream, const TPoint<T>& _value)
		{
			_stream << _value.left << " " << _value.top;
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, TPoint<T>& _value)
		{
			_stream >> _value.left >> _value.top;
			if (_stream.fail())
				_value.clear();
			return _stream;
		}
	};

} // namespace MyGUI

#endif // MYGUI_TPONT_H_
