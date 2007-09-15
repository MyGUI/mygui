#ifndef _TRECT_H_
#define _TRECT_H_

#include "Prerequest.h"
#include <vector>
#include <string>
#include "stringUtil.h"


namespace MyGUI
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

		static TRect parse(const std::string & _value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 4) return TRect(parseValue<T>(vec[0]), parseValue<T>(vec[1]), parseValue<T>(vec[2]), parseValue<T>(vec[3]));
			return TRect();
		}

	};

	typedef TRect<int> intRect;
	typedef TRect<float> floatRect;

} // namespace MyGUI

#endif
