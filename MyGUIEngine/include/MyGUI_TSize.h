/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TSIZE_H_
#define MYGUI_TSIZE_H_

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

#endif // MYGUI_TSIZE_H_
