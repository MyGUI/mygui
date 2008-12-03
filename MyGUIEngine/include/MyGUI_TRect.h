/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TRECT_H__
#define __MYGUI_TRECT_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace types
	{

		template< typename T > struct TRect
		{
			T left, top, right, bottom;

			TRect() : left( 0 ), top( 0 ), right( 0 ), bottom( 0 ) { }
			TRect( T const & l, T const & t, T const & r, T const & b ) : left( l ), top( t ), right( r ), bottom( b ) { }
			TRect( TRect const & o ) : left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom ) { }
			explicit TRect(const std::string& _value) {*this = parse(_value);}

			TRect & operator-=( TRect const & o )
			{
				left -= o.left;
				top -= o.top;
				right -= o.right;
				bottom -= o.bottom;
				return *this;
			}

			TRect & operator+=( TRect const & o )
			{
				left += o.left;
				top += o.top;
				right += o.right;
				bottom += o.bottom;
				return *this;
			}

			TRect operator-( TRect const & o ) const
			{
				return TRect(left - o.left, top - o.top, right - o.right, bottom - o.bottom);
			}

			TRect operator+( TRect const & o ) const
			{
				return TRect(left + o.left, top + o.top, right + o.right, bottom + o.bottom);
			}

			TRect & operator=( TRect const & o )
			{
				left = o.left;
				top = o.top;
				right = o.right;
				bottom = o.bottom;
				return *this;
			}

			template< typename U >
			TRect & operator=( TRect<U> const & o )
			{
				left = o.left;
				top = o.top;
				right = o.right;
				bottom = o.bottom;
				return *this;
			}

			bool operator==( TRect const & o ) const
			{
				return ((left == o.left) && (top == o.top) && (right == o.right) && (bottom == o.bottom));
			}

			bool operator!=( TRect const & o ) const
			{
				return ! ((left == o.left) && (top == o.top) && (right == o.right) && (bottom == o.bottom));
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

			void set( T const & l, T const & t, T const & r, T const & b )
			{
				left = l;
				top = t;
				right = r;
				bottom = b;
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
				return ( (_value.left >= left) && (_value.right <= right) && (_value.top >= top) && (_value.bottom <= bottom) );
			}

			bool intersect(const TRect<T>&  _value) const
			{
				return ( (_value.left <= right) && (_value.right >= left) && (_value.top <= bottom) && (_value.bottom >= top) );
			}

			bool inside(const TPoint<T>&  _value) const
			{
				return ( (_value.left >= left) && (_value.left <= right) && (_value.top >= top) && (_value.top <= bottom) );
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
				if (stream.fail()) return TRect<T>();
				else {
					int item = stream.get();
					while (item != -1) {
						if (item != ' ' && item != '\t') return TRect<T>();
						item = stream.get();
					};
				}
				return result;
			}

			friend std::ostream& operator << ( std::ostream& _stream, const TRect<T>&  _value )
			{
				_stream << _value.left << " " << _value.top << " " << _value.right << " " << _value.bottom;
				return _stream;
			}

			friend std::istream& operator >> ( std::istream& _stream, TRect<T>&  _value )
			{
				_stream >> _value.left >> _value.top >> _value.right >> _value.bottom;
				if (_stream.fail()) _value.clear();
				return _stream;
			}

		};

	} // namespace types
} // namespace MyGUI

#endif // __MYGUI_TRECT_H__
