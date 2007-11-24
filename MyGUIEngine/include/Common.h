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

	#if _DEBUG
		#define ASSERT(_exp) assert(_exp)
		#define ASSERT_DEBUG(_exp) assert(_exp)
		#define TYPE(_type,_ptr) assert(dynamic_cast<_type>(_ptr))
		#define MYGUI_EXCEPT(_num,_dest,_src) {LOG(_dest,_src);  OGRE_EXCEPT(_num,_dest,_src);}
		#define REGISTER_VALUE(_map,_value) {LOG("REGISTER_VALUE : '", #_value, "' = ", _value);_map[#_value]=_value;}
	#else
		#define ASSERT(_exp) assert(_exp)
		#define ASSERT_DEBUG(_exp) ((void)0)
		#define TYPE(_type,_ptr) ((void)0)
		#define MYGUI_EXCEPT(_num,_dest,_src) {LOG(_dest,"  ",_src);  OGRE_EXCEPT(_num,_dest,_src);}
		#define REGISTER_VALUE(_map,_value) _map[#_value]=_value;
	#endif

} // namespace MyGUI


#endif // __COMMON_H__