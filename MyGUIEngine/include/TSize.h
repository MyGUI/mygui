#ifndef __TSIZE_H__
#define __TSIZE_H__

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

		bool operator==( TSize const & o )
		{
			return ( (width == o.width) && (height == o.height) );
		}

		bool operator!=( TSize const & o )
		{
			return ! ( (width == o.width) && (height == o.height) );
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

		inline bool empty()
		{
			return ((width == 0) && (height == 0));
		}

	};

} // namespace types

#endif // __TSIZE_H__