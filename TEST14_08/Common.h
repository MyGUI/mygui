#pragma once

#include <map>
#include <string>
#include <vector>
#include "TRect.h"
#include "TSize.h"
#include "TPoint.h"
#include <assert.h>
#include "LoggingOut.h"
#include "RenderOut.h"


namespace widget
{

	#define null 0

	//Bit flags done easy
	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))

	#define REGISTER_VALUE(_map,_value) _map[#_value]=_value;

	#if _DEBUG
		#define ASSERT(_exp) assert(_exp)
		#define TYPE(_type,_ptr) assert(dynamic_cast<_type>(_ptr))
	#else
		#define ASSERT(_exp) ((void)0)
		#define TYPE(_type,_ptr) ((void)0)
	#endif

} // namespace widget