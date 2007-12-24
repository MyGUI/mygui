/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_COMMON_H__
#define __MYGUI_COMMON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Loging.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Types.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_AlignInfo.h"
#include "MyGUI_ResourcePath.h"
#include "OgreException.h"

namespace MyGUI
{

	#define MYGUI_ERROR(_dest) MyGUI::MYGUI_LOG("ERROR : ", _dest, "  at  ", __FILE__, " (line ", __LINE__, ")")
	#define MYGUI_EXCEPT(_dest) {MyGUI::MYGUI_LOG("EXCEPT : ", _dest,  "  at  ", __FILE__, " (line ", __LINE__, ")"); OGRE_EXCEPT(0, _dest, "");}
	#define MYGUI_ASSERT(_exp) {if (!(_exp)) {MyGUI::MYGUI_LOG("ASSERT : '", #_exp,  "'  at  ", __FILE__, " (line ", __LINE__, ")"); OGRE_EXCEPT(0, util::toString("expression = '", #_exp, "'"), "");}}

	#if MYGUI_DEBUG_MODE == 1
		#define MYGUI_TYPE(_type,_ptr) MYGUI_ASSERT(null != dynamic_cast<_type>(_ptr))
		#define MYGUI_REGISTER_VALUE(_map,_value) {MYGUI_LOG("Register value : '", #_value, "' = ", _value);_map[#_value]=_value;}
		#define MYGUI_DEBUG_ASSERT(_exp) MYGUI_ASSERT(_exp)
	#else // MYGUI_DEBUG_MODE == 1
		#define MYGUI_TYPE(_type,_ptr) ((void)0)
		#define MYGUI_REGISTER_VALUE(_map,_value) _map[#_value]=_value;
		#define MYGUI_DEBUG_ASSERT(_exp) ((void)0)
	#endif // MYGUI_DEBUG_MODE == 1


	// шаблонный класс для проверки типа виджета
	class Widget;

	#if MYGUI_DEBUG_MODE == 1
		#define castWidget(_type,_widget) castWidgetTemplate<_type>(_widget, __FILE__, __LINE__)
		template <class T> inline T* castWidgetTemplate(Widget * _widget , const char * _file, int _line)
		{
			T * widget = dynamic_cast<T*>(_widget);
			if (null == widget) {
				MYGUI_LOG("ASSERT : 'null == widget'  at  ", _file, " (line ", _line, ")");
				throw Ogre::ExceptionFactory::create(Ogre::ExceptionCodeType<0>(), util::toString("expression = 'null == widget'"), "", _file, _line);
			}
			return static_cast<T*>(widget);
		}
	#else // MYGUI_DEBUG_MODE == 1
		#define castWidget(_type,_widget) static_cast<_type*>(_widget)
	#endif // MYGUI_DEBUG_MODE == 1

} // namespace MyGUI


#endif // __MYGUI_COMMON_H__