/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TPONT_H__
#define __MYGUI_TPONT_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace types
	{
		template< typename T > struct TPoint
		{
			T left, top;

			TPoint() : left( 0 ), top( 0 ) { }
			TPoint( T const & l, T const & t) : left( l ), top( t ) { }
			TPoint( TPoint const & o ) : left( o.left ), top( o.top ) { }
			explicit TPoint(const std::string& _value) {*this = parse(_value);}

			TPoint & operator-=( TPoint const & o )
			{
				left -= o.left;
				top -= o.top;
				return *this;
			}

			TPoint & operator+=( TPoint const & o )
			{
				left += o.left;
				top += o.top;
				return *this;
			}

			TPoint operator-( TPoint const & o ) const
			{
				return TPoint(left - o.left, top - o.top);
			}

			TPoint operator+( TPoint const & o ) const
			{
				return TPoint(left + o.left, top + o.top);
			}

			TPoint & operator=( TPoint const & o )
			{
				left = o.left;
				top = o.top;
				return *this;
			}

			template< typename U >
			TPoint & operator=( TPoint<U> const & o )
			{
				left = o.left;
				top = o.top;
				return *this;
			}

			bool operator==( TPoint const & o ) const
			{
				return ((left == o.left) && (top == o.top));
			}

			bool operator!=( TPoint const & o ) const
			{
				return ! ((left == o.left) && (top == o.top));
			}

			void clear()
			{
				left = top = 0;
			}

			void set( T const & l, T const & t)
			{
				left = l;
				top = t;
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
				if (stream.fail()) return TPoint<T>();
				else {
					int item = stream.get();
					while (item != -1) {
						if (item != ' ' && item != '\t') return TPoint<T>();
						item = stream.get();
					};
				}
				return result;
			}

			friend std::ostream& operator << ( std::ostream& _stream, const TPoint<T>&  _value )
			{
				_stream << _value.left << " " << _value.top;
				return _stream;
			}

			friend std::istream& operator >> ( std::istream& _stream, TPoint<T>&  _value )
			{
				_stream >> _value.left >> _value.top;
				if (_stream.fail()) _value.clear();
				return _stream;
			}

		};

	} // namespace types
} // namespace MyGUI

#endif // __MYGUI_TPONT_H__
