#ifndef _TSIZE_H_
#define _TSIZE_H_

namespace types
{

	template< typename T > struct TSize
	{
		T width, height;

		TSize()
		: width( 0 ), height( 0 )
		{
		}

		TSize( T const & w, T const & h)
		: width( w ), height( h )
		{
		}

		TSize( TSize const & o )
		: width( o.width ), height( o.height )
		{
		}

		TSize & operator=( TSize const & o )
		{
			width = o.width;
			height = o.height;
			return *this;
		}

		inline void clear()
		{
			width = height = 0;
		}

		inline void set( T const & w, T const & h)
		{
			width = w;
			height = h;
		}

	};

} // namespace types

#endif
