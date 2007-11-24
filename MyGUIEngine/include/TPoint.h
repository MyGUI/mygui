#ifndef __TPONT_H__
#define __TPONT_H__

namespace types
{
	template< typename T > struct TPoint
	{
		T left, top;

		TPoint()
		: left( 0 ), top( 0 )
		{
		}

		TPoint( T const & l, T const & t)
		: left( l ), top( t )
		{
		}

		TPoint( TPoint const & o )
		: left( o.left ), top( o.top )
		{
		}

		TPoint & operator=( TPoint const & o )
		{
			left = o.left;
			top = o.top;
			return *this;
		}

		bool operator==( TPoint const & o )
		{
			return ( (left == o.left) && (top == o.top) );
		}

		bool operator!=( TPoint const & o )
		{
			return ! ( (left == o.left) && (top == o.top) );
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

	};

} // namespace types

#endif // __TPONT_H__