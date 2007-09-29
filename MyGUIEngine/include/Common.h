#ifndef _COMMON_H_
#define _COMMON_H_

#include <map>
#include <string>
#include <vector>
#include "Types.h"
//#include "TRect.h"
//#include "TSize.h"
//#include "TPoint.h"
#include <assert.h>
#include "LoggingOut.h"
#include "RenderOut.h"
#include "StringUtil.h"
//#include "ParseUtil.h"
#include "AlignInfo.h"
#include "ResourcePath.h"

#include "Prerequest.h"


namespace MyGUI
{

	#define null 0

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


#endif