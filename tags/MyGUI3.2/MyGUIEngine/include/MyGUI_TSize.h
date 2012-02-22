/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_TSIZE_H__
#define __MYGUI_TSIZE_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace types
	{

		template<typename T>
		struct TSize
		{
			T width;
			T height;

			TSize() :
				width(0),
				height(0)
			{
			}

			TSize(T const& _width, T const& _height) :
				width(_width),
				height(_height)
			{
			}

			TSize(TSize const& _obj) :
				width(_obj.width),
				height(_obj.height)
			{
			}

			TSize& operator -= (TSize const& _obj)
			{
				width -= _obj.width;
				height -= _obj.height;
				return *this;
			}

			TSize& operator += (TSize const& _obj)
			{
				width += _obj.width;
				height += _obj.height;
				return *this;
			}

			TSize operator - (TSize const& _obj) const
			{
				return TSize(width - _obj.width, height - _obj.height);
			}

			TSize operator + (TSize const& _obj) const
			{
				return TSize(width + _obj.width, height + _obj.height);
			}

			TSize& operator = (TSize const& _obj)
			{
				width = _obj.width;
				height = _obj.height;
				return *this;
			}

			template<typename U>
			TSize& operator = (TSize<U> const& _obj)
			{
				width = _obj.width;
				height = _obj.height;
				return *this;
			}

			bool operator == (TSize const& _obj) const
			{
				return ((width == _obj.width) && (height == _obj.height));
			}

			bool operator != (TSize const& _obj) const
			{
				return !((width == _obj.width) && (height == _obj.height));
			}

			void clear()
			{
				width = height = 0;
			}

			void set(T const& _width, T const& _height)
			{
				width = _width;
				height = _height;
			}

			void swap(TSize& _value)
			{
				TSize tmp = _value;
				_value = *this;
				*this = tmp;
			}

			bool empty() const
			{
				return ((width == 0) && (height == 0));
			}

			std::string print() const
			{
				std::ostringstream stream;
				stream << *this;
				return stream.str();
			}

			static TSize<T> parse(const std::string& _value)
			{
				TSize<T> result;
				std::istringstream stream(_value);
				stream >> result.width >> result.height;
				if (stream.fail())
				{
					return TSize<T>();
				}
				else
				{
					int item = stream.get();
					while (item != -1)
					{
						if (item != ' ' && item != '\t')
							return TSize<T>();
						item = stream.get();
					}
				}
				return result;
			}

			friend std::ostream& operator << (std::ostream& _stream, const TSize<T>&  _value)
			{
				_stream << _value.width << " " << _value.height;
				return _stream;
			}

			friend std::istream& operator >> (std::istream& _stream, TSize<T>&  _value)
			{
				_stream >> _value.width >> _value.height;
				if (_stream.fail())
					_value.clear();
				return _stream;
			}
		};

	} // namespace types

} // namespace MyGUI

#endif // __MYGUI_TSIZE_H__
