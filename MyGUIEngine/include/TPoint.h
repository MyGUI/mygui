#ifndef _TPONT_H_
#define _TPONT_H_

#include "Prerequest.h"
#include <vector>
#include <string>
#include "stringUtil.h"


namespace MyGUI
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

		static TPoint parse(const std::string & _value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 2) return TPoint( parseValue<T>(vec[0]), parseValue<T>(vec[1]) );
			return TPoint();
		}

	};

	typedef TPoint<int> intPoint;
	typedef TPoint<float> floatPoint;

} // namespace MyGUI

#endif
