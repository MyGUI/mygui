#ifndef _TPONT_H_
#define _TPONT_H_

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

	};

} // namespace types

#endif
