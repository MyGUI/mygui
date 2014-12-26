/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TRECT_H_
#define MYGUI_TRECT_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace types
	{

		template<typename T>
		struct TRect
		{
			T left;
			T top;
			T right;
			T bottom;

			TRect() :
				left(0),
				top(0),
				right(0),
				bottom(0)
			{
			}

			TRect(T const& _left, T const& _top, T const& _right, T const& _bottom) :
				left(_left),
				top(_top),
				right(_right),
				bottom(_bottom)
			{
			}

			TRect(TRect const& _obj) :
				left(_obj.left),
				top(_obj.top),
				right(_obj.right),
				bottom(_obj.bottom)
			{
			}

			TRect& operator -= (TRect const& _obj)
			{
				left -= _obj.left;
				top -= _obj.top;
				right -= _obj.right;
				bottom -= _obj.bottom;
				return *this;
			}

			TRect& operator += (TRect const& _obj)
			{
				left += _obj.left;
				top += _obj.top;
				right += _obj.right;
				bottom += _obj.bottom;
				return *this;
			}

			TRect operator - (TRect const& _obj) const
			{
				return TRect(left - _obj.left, top - _obj.top, right - _obj.right, bottom - _obj.bottom);
			}

			TRect operator + (TRect const& _obj) const
			{
				return TRect(left + _obj.left, top + _obj.top, right + _obj.right, bottom + _obj.bottom);
			}

			TRect& operator = (TRect const& _obj)
			{
				left = _obj.left;
				top = _obj.top;
				right = _obj.right;
				bottom = _obj.bottom;
				return *this;
			}

			template<typename U>
			TRect& operator = (TRect<U> const& _obj)
			{
				left = _obj.left;
				top = _obj.top;
				right = _obj.right;
				bottom = _obj.bottom;
				return *this;
			}

			bool operator == (TRect const& _obj) const
			{
				return ((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
			}

			bool operator != (TRect const& _obj) const
			{
				return !((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
			}

			T width() const
			{
				return right - left;
			}

			T height() const
			{
				return bottom - top;
			}

			void clear()
			{
				left = top = right = bottom = 0;
			}

			void set(T const& _left, T const& _top, T const& _right, T const& _bottom)
			{
				left = _left;
				top = _top;
				right = _right;
				bottom = _bottom;
			}

			void swap(TRect& _value)
			{
				TRect tmp = _value;
				_value = *this;
				*this = tmp;
			}

			bool empty() const
			{
				return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
			}

			bool inside(const TRect<T>&  _value) const
			{
				return ((_value.left >= left) && (_value.right <= right) && (_value.top >= top) && (_value.bottom <= bottom));
			}

			bool intersect(const TRect<T>&  _value) const
			{
				return ((_value.left <= right) && (_value.right >= left) && (_value.top <= bottom) && (_value.bottom >= top));
			}

			bool inside(const TPoint<T>&  _value) const
			{
				return ((_value.left >= left) && (_value.left <= right) && (_value.top >= top) && (_value.top <= bottom));
			}

			std::string print() const
			{
				std::ostringstream stream;
				stream << *this;
				return stream.str();
			}

			static TRect<T> parse(const std::string& _value)
			{
				TRect<T> result;
				std::istringstream stream(_value);
				stream >> result.left >> result.top >> result.right >> result.bottom;
				if (stream.fail())
				{
					return TRect<T>();
				}
				else
				{
					int item = stream.get();
					while (item != -1)
					{
						if (item != ' ' && item != '\t')
							return TRect<T>();
						item = stream.get();
					}
				}
				return result;
			}

			friend std::ostream& operator << (std::ostream& _stream, const TRect<T>&  _value)
			{
				_stream << _value.left << " " << _value.top << " " << _value.right << " " << _value.bottom;
				return _stream;
			}

			friend std::istream& operator >> (std::istream& _stream, TRect<T>&  _value)
			{
				_stream >> _value.left >> _value.top >> _value.right >> _value.bottom;
				if (_stream.fail())
					_value.clear();
				return _stream;
			}
		};

	} // namespace types

} // namespace MyGUI

#endif // MYGUI_TRECT_H_
