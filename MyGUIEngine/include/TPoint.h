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

		inline TPoint & operator=( TPoint const & o )
		{
			left = o.left;
			top = o.top;
			return *this;
		}

		inline bool operator==( TPoint const & o )
		{
			return ( (left == o.left) && (top == o.top) );
		}

		inline bool operator!=( TPoint const & o )
		{
			return ! (*this == o);
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

		inline bool empty()
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