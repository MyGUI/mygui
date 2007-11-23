#ifndef __COMMON_H__
#define __COMMON_H__

#include "Prerequest.h"
#include "Types.h"
#include "RenderOut.h"
#include "utility.h"
#include "AlignInfo.h"
#include "ResourcePath.h"

namespace MyGUI
{

	#define null 0
	#define ITEM_NONE SIZE_MAX

	#define REGISTER_VALUE(_map,_value) _map[#_value]=_value;

	#if _DEBUG
		#define ASSERT(_exp) assert(_exp)
//		#define ASSERT_DEBUG(_exp) assert(_exp)
		#define TYPE(_type,_ptr) assert(dynamic_cast<_type>(_ptr))
	#else
		#define ASSERT(_exp) ((void)0)
		#define TYPE(_type,_ptr) ((void)0)
	#endif

} // namespace MyGUI


#endif // __COMMON_H__