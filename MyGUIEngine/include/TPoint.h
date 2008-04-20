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

		template< typename U >
		inline TPoint & operator=( TPoint<U> const & o )
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
	        stream << *this;
		    return stream.str();
		}

		inline static TPoint<T> parse(const std::string& _value)
		{
			TPoint<T> ret;
	        std::istringstream stream(_value);
	        stream >> ret;
		    return ret;
		}

        inline friend std::ostream& operator << ( std::ostream& _stream, const TPoint<T>&  _value )
        {
            _stream << _value.left << " " << _value.top;
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, TPoint<T>&  _value )
        {
            _stream >> _value.left >> _value.top;
			if (_stream.fail()) _value.clear();
            return _stream;
        }

	};

} // namespace types

#endif // __TPONT_H__
