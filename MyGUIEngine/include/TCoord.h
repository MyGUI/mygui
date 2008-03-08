/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __TCOORD_H__
#define __TCOORD_H__

#include "TPoint.h"
#include "TSize.h"
#include <string>
#include <sstream>

namespace types
{

	template< typename T > struct TCoord
	{
		T left, top, width, height;

		TCoord() : left( 0 ), top( 0 ), width( 0 ), height( 0 ) { }
		TCoord( T const & _left, T const & _top, T const & _width, T const & _height ) : left( _left ), top( _top ), width( _width ), height( _height ) { }
		TCoord( TCoord const & _obj ) : left( _obj.left ), top( _obj.top ), width( _obj.width ), height( _obj.height ) { }
		TCoord( TPoint<T> const & _point, TSize<T> const & _size ) : left( _point.left ), top( _point.top ), width( _size.width ), height( _size.height ) { }
		explicit TCoord(const std::string& _value) {*this = parse(_value);}

		inline TCoord & operator-=( TCoord const & _obj )
		{
			left -= _obj.left;
			top -= _obj.top;
			width -= _obj.width;
			height -= _obj.height;
			return *this;
		}

		inline TCoord & operator+=( TCoord const & _obj )
		{
			left += _obj.left;
			top += _obj.top;
			width += _obj.width;
			height += _obj.height;
			return *this;
		}

		inline TCoord operator-( TCoord const & _obj ) const
		{
			return TCoord(left - _obj.left, top - _obj.top, width - _obj.width, height - _obj.height);
		}

		inline TCoord operator-( TPoint<T> const & _obj ) const
		{
			return TCoord(left - _obj.left, top - _obj.top, width, height);
		}

		inline TCoord operator-( TSize<T> const & _obj ) const
		{
			return TCoord(left, top, width - _obj.width, height - _obj.height);
		}

		inline TCoord operator+( TCoord const & _obj ) const
		{
			return TCoord(left + _obj.left, top + _obj.top, width + _obj.width, height + _obj.height);
		}

		inline TCoord operator+( TPoint<T> const & _obj ) const
		{
			return TCoord(left + _obj.left, top + _obj.top, width, height);
		}

		inline TCoord operator+( TSize<T> const & _obj ) const
		{
			return TCoord(left, top, width + _obj.width, height + _obj.height);
		}

		inline TCoord & operator=( TCoord const & _obj )
		{
			left = _obj.left;
			top = _obj.top;
			width = _obj.width;
			height = _obj.height;
			return *this;
		}

		inline TCoord & operator=( TPoint<T> const & _obj )
		{
			left = _obj.left;
			top = _obj.top;
			return *this;
		}

		inline TCoord & operator=( TSize<T> const & _obj )
		{
			width = _obj.width;
			height = _obj.height;
			return *this;
		}


		inline bool operator==( TCoord const & _obj ) const
		{
			return ((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
		}

		inline bool operator!=( TCoord const & _obj ) const
		{
			return ! ((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
		}

		inline T right() const
		{
			return left + width;
		}

		inline T bottom() const
		{
			return top + height;
		}

		inline void clear()
		{
			left = top = width = height = 0;
		}

		inline void set( T const & _left, T const & _top, T const & _width, T const & _height )
		{
			left = _left;
			top = _top;
			width = _width;
			height = _height;
		}

		inline void swap(TCoord& _value)
		{
			TCoord tmp = _value;
			_value = *this;
			*this = tmp;
		}

		inline bool empty() const
		{
			return ((left == 0) && (top == 0) && (width == 0) && (height == 0));
		}

		inline TPoint<T> point() const
		{
			return TPoint<T>(left, top);
		}

		inline TSize<T> size() const
		{
			return TSize<T>(width, height);
		}

		inline std::string print() const
		{
	        std::ostringstream stream;
	        stream << left << " " << top << " " << width << " " << height;
		    return stream.str();
		}

		static TCoord<T> parse(const std::string& _value)
		{
			TCoord<T> ret;
			std::istringstream str(_value);
			str >> ret.left >> ret.top >> ret.width >> ret.height;
			if (str.fail()) return TCoord<T>();
			return ret;
		}

	};

} // namespace types

#endif // __TCOORD_H__
