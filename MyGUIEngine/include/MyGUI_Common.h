/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_COMMON_H__
#define __MYGUI_COMMON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_AlignInfo.h"
#include "MyGUI_ResourcePath.h"
#include "MyGUI_LogManager.h"
#include "utility.h"

namespace MyGUI
{

	#define null 0
	#define ITEM_NONE SIZE_MAX

	#if _DEBUG
		#define ASSERT(_exp) assert(_exp)
		#define ASSERT_DEBUG(_exp) assert(_exp)
		#define TYPE(_type,_ptr) assert(dynamic_cast<_type>(_ptr))
		#define MYGUI_EXCEPT(_dest,_src) {LogManager::getInstance().out(_dest," | ",_src);  OGRE_EXCEPT(0,_dest,_src);}
		#define REGISTER_VALUE(_map,_value) {LogManager::getInstance().out("REGISTER_VALUE : '", #_value, "' = ", _value);_map[#_value]=_value;}
	#else
		#define ASSERT(_exp) assert(_exp)
		#define ASSERT_DEBUG(_exp) ((void)0)
		#define TYPE(_type,_ptr) ((void)0)
		#define MYGUI_EXCEPT(_dest,_src) {LogManager::getInstance().out(_dest," | ",_src);  OGRE_EXCEPT(0,_dest,_src);}
		#define REGISTER_VALUE(_map,_value) _map[#_value]=_value;
	#endif

} // namespace MyGUI


#endif // __MYGUI_COMMON_H__