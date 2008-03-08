/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __TPONT_H__
#define __TPONT_H__

#include <string>
#include <sstream>

namespace types
{
	template< typename T > struct TPoint
	{
		T left, top;

		TPoint() : left( 0 ), top( 0 ) { }
		TPoint( T const & l, T const & t) : left( l ), top( t ) { }
		TPoint( TPoint const & o ) : left( o.left ), top( o.top ) { }
		explicit TPoint(const std::string& _value) {*this = parse(_value);}

		inline TPoint & operator-=( TPoint const & o )
		{
			left -= o.left;
			top -= o.top;
			return *this;
		}

		inline TPoint & operator+=( TPoint const & o )
		{
			left += o.left;
			top += o.top;
			return *this;
		}

		inline TPoint operator-( TPoint const & o ) const
		{
			return TPoint(left - o.left, top - o.top);
		}

		inline TPoint operator+( TPoint const & o ) const
		{
			return TPoint(left + o.left, top + o.top);
		}

		inline TPoint & operator=( TPoint const & o )
		{
			left = o.left;
			top = o.top;
			return *this;
		}

		inline bool operator==( TPoint const & o ) const
		{
			return ((left == o.left) && (top == o.top));
		}

		inline bool operator!=( TPoint const & o ) const
		{
			return ! ((left == o.left) && (top == o.top));
		}

		inline void clear()
		{
			left = top = 0;
		}

		inline void set( T const & l, T const & t)
		{
			left = l;
			top = t;
		}

		inline void swap(TPoint& _value)
		{
			TPoint tmp = _value;
			_value = *this;
			*this = tmp;
		}

		inline bool empty() const
		{
			return ((left == 0) && (top == 0));
		}

		inline std::string print() const
		{
	        std::ostringstream stream;
	        stream << left << " " << top;
		    return stream.str();
		}

		static TPoint<T> parse(const std::string& _value)
		{
			TPoint<T> ret;
			std::istringstream str(_value);
			str >> ret.left >> ret.top;
			if (str.fail()) return TPoint<T>();
			return ret;
		}

	};

} // namespace types

#endif // __TPONT_H__
