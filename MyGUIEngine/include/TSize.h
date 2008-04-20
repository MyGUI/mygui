/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __TSIZE_H__
#define __TSIZE_H__

#include <string>
#include <sstream>

namespace types
{

	template< typename T > struct TSize
	{
		T width, height;

		TSize() : width( 0 ), height( 0 ) { }
		TSize( T const & w, T const & h) : width( w ), height( h ) { }
		TSize( TSize const & o ) : width( o.width ), height( o.height ) { }
		explicit TSize(const std::string& _value) {*this = parse(_value);}

		inline TSize & operator-=( TSize const & o )
		{
			width -= o.width;
			height -= o.height;
			return *this;
		}

		inline TSize & operator+=( TSize const & o )
		{
			width += o.width;
			height += o.height;
			return *this;
		}

		inline TSize operator-( TSize const & o ) const
		{
			return TSize(width - o.width, height - o.height);
		}

		inline TSize operator+( TSize const & o ) const
		{
			return TSize(width + o.width, height + o.height);
		}

		inline TSize & operator=( TSize const & o )
		{
			width = o.width;
			height = o.height;
			return *this;
		}

		template< typename U >
		inline TSize & operator=( TSize<U> const & o )
		{
			width = o.width;
			height = o.height;
			return *this;
		}

		inline bool operator==( TSize const & o ) const
		{
			return ((width == o.width) && (height == o.height));
		}

		inline bool operator!=( TSize const & o ) const
		{
			return ! ((width == o.width) && (height == o.height));
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

		inline void swap(TSize& _value)
		{
			TSize tmp = _value;
			_value = *this;
			*this = tmp;
		}

		inline bool empty() const
		{
			return ((width == 0) && (height == 0));
		}

		inline std::string print() const
		{
	        std::ostringstream stream;
	        stream << *this;
		    return stream.str();
		}

		inline static TSize<T> parse(const std::string& _value)
		{
			TSize<T> ret;
	        std::istringstream stream(_value);
	        stream >> ret;
		    return ret;
		}

        inline friend std::ostream& operator << ( std::ostream& _stream, const TSize<T>&  _value )
        {
            _stream << _value.width << " " << _value.height;
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, TSize<T>&  _value )
        {
            _stream >> _value.width >> _value.height;
			if (_stream.fail()) _value.clear();
            return _stream;
        }

	};

} // namespace types

#endif // __TSIZE_H__
