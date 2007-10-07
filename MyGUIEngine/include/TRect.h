#ifndef _TRECT_H_
#define _TRECT_H_

namespace types
{

	template< typename T > struct TRect
	{
		T left, top, right, bottom;

		TRect()
		: left( 0 ), top( 0 ), right( 0 ), bottom( 0 )
		{
		}

		TRect( T const & l, T const & t, T const & r, T const & b )
		: left( l ), top( t ), right( r ), bottom( b )
		{
		}

		TRect( TRect const & o )
		: left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom )
		{
		}

		TRect & operator=( TRect const & o )
		{
			left = o.left;
			top = o.top;
			right = o.right;
			bottom = o.bottom;
			return *this;
		}

		T width() const
		{
			return right - left;
		}

		T height() const
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
	};

} // namespace types

#endif
