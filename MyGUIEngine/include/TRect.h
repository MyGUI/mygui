/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __TRECT_H__
#define __TRECT_H__

#include <string>
#include <sstream>

namespace types
{

	template< typename T > struct TRect
	{
		T left, top, right, bottom;

		TRect() : left( 0 ), top( 0 ), right( 0 ), bottom( 0 ) { }
		TRect( T const & l, T const & t, T const & r, T const & b ) : left( l ), top( t ), right( r ), bottom( b ) { }
		TRect( TRect const & o ) : left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom ) { }
		explicit TRect(const std::string& _value) {*this = parse(_value);}

		inline TRect & operator-=( TRect const & o )
		{
			left -= o.left;
			top -= o.top;
			right -= o.right;
			bottom -= o.bottom;
			return *this;
		}

		inline TRect & operator+=( TRect const & o )
		{
			left += o.left;
			top += o.top;
			right += o.right;
			bottom += o.bottom;
			return *this;
		}

		inline TRect operator-( TRect const & o ) const
		{
			return TRect(left - o.left, top - o.top, right - o.right, bottom - o.bottom);
		}

		inline TRect operator+( TRect const & o ) const
		{
			return TRect(left + o.left, top + o.top, right + o.right, bottom + o.bottom);
		}

		inline TRect & operator=( TRect const & o )
		{
			left = o.left;
			top = o.top;
			right = o.right;
			bottom = o.bottom;
			return *this;
		}

		inline bool operator==( TRect const & o ) const
		{
			return ((left == o.left) && (top == o.top) && (right == o.right) && (bottom == o.bottom));
		}

		inline bool operator!=( TRect const & o ) const
		{
			return ! ((left == o.left) && (top == o.top) && (right == o.right) && (bottom == o.bottom));
		}

		inline T width() const
		{
			return right - left;
		}

		inline T height() const
		{
			return bottom - top;
		}

		inline void clear()
		{
			left = top = right = bottom = 0;
		}

		inline void set( T const & l, T const & t, T const & r, T const & b )
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}

		inline void swap(TRect& _value)
		{
			TRect tmp = _value;
			_value = *this;
			*this = tmp;
		}

		inline bool empty() const
		{
			return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
		}

		inline std::string print() const
		{
	        std::ostringstream stream;
	        stream << left << " " << top << " " << right << " " << bottom;
		    return stream.str();
		}

		static TRect<T> parse(const std::string& _value)
		{
			TRect<T> ret;
			std::istringstream str(_value);
			str >> ret.left >> ret.top >> ret.right >> ret.bottom;
			if (str.fail()) return TRect<T>();
			return ret;
		}

	};

} // namespace types

#endif // __TRECT_H__
