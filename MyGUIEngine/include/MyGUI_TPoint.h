/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TPONT_H_
#define MYGUI_TPONT_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace types
	{

		template<typename T>
		struct TPoint
		{
			T left;
			T top;

			TPoint() :
				left(0),
				top(0)
			{
			}

			TPoint(T const& _left, T const& _top) :
				left(_left),
				top(_top)
			{
			}

			TPoint(TPoint const& _obj) :
				left(_obj.left),
				top(_obj.top)
			{
			}

			TPoint& operator -= (TPoint const& _obj)
			{
				left -= _obj.left;
				top -= _obj.top;
				return *this;
			}

			TPoint& operator += (TPoint const& _obj)
			{
				left += _obj.left;
				top += _obj.top;
				return *this;
			}

			TPoint operator - (TPoint const& _obj) const
			{
				return TPoint(left - _obj.left, top - _obj.top);
			}

			TPoint operator + (TPoint const& _obj) const
			{
				return TPoint(left + _obj.left, top + _obj.top);
			}

			TPoint& operator = (TPoint const& _obj)
			{
				left = _obj.left;
				top = _obj.top;
				return *this;
			}

			template<typename U>
			TPoint& operator = (TPoint<U> const& _obj)
			{
				left = _obj.left;
				top = _obj.top;
				return *this;
			}

			bool operator == (TPoint const& _obj) const
			{
				return ((left == _obj.left) && (top == _obj.top));
			}

			bool operator != (TPoint const& _obj) const
			{
				return ! ((left == _obj.left) && (top == _obj.top));
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

			static TPoint<T> parse(const std::string& _value)
			{
				TPoint<T> result;
				std::istringstream stream(_value);
				stream >> result.left >> result.top;
				if (stream.fail())
				{
					return TPoint<T>();
				}
				else
				{
					int item = stream.get();
					while (item != -1)
					{
						if (item != ' ' && item != '\t')
							return TPoint<T>();
						item = stream.get();
					}
				}
				return result;
			}

			friend std::ostream& operator << (std::ostream& _stream, const TPoint<T>&  _value)
			{
				_stream << _value.left << " " << _value.top;
				return _stream;
			}

			friend std::istream& operator >> (std::istream& _stream, TPoint<T>&  _value)
			{
				_stream >> _value.left >> _value.top;
				if (_stream.fail())
					_value.clear();
				return _stream;
			}
		};

	} // namespace types

} // namespace MyGUI

#endif // MYGUI_TPONT_H_
