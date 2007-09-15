#ifndef _TSIZE_H_
#define _TSIZE_H_

#include "Prerequest.h"
#include <vector>
#include <string>
#include "stringUtil.h"


namespace MyGUI
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

		static TSize parse(const std::string & _value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 2) return TSize( parseValue<T>(vec[0]), parseValue<T>(vec[1]) );
			return TSize();
		}

	};

	typedef TSize<int> intSize;
	typedef TSize<float> floatSize;

} // namespace MyGUI

#endif
