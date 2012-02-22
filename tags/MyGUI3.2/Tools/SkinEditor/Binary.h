/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __BINARY_H__
#define __BINARY_H__

namespace tools
{

	template <unsigned long N>
	struct Binary
	{
		static unsigned const value
			= Binary < N / 10 >::value << 1 // prepend higher bits
			| N % 10;                     // to lowest bit
	};

	template <>                           // specialization
	struct Binary<0>                      // terminates recursion
	{
		static unsigned const value = 0;
	};

} // namespace tools

#endif // __BINARY_H__
