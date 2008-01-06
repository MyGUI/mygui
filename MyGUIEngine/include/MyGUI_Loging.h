/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LOGING_H__
#define __MYGUI_LOGING_H__

#include "MyGUI_Prerequest.h"
#include "utility.h"

namespace MyGUI
{

	void _MyGUIExport _MYGUI_LOG(const std::string & _message);

	template< class T1,  class T2 >
	inline void _MYGUI_LOG (T1 p1, T2 p2) {_MYGUI_LOG(util::toString(p1, p2));}

	template< class T1,  class T2,  class T3 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3) {_MYGUI_LOG(util::toString(p1, p2, p3));}

	template< class T1,  class T2,  class T3, class T4 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3, T4 p4) {_MYGUI_LOG(util::toString(p1, p2, p3, p4));}

	template< class T1,  class T2,  class T3, class T4, class T5 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {_MYGUI_LOG(util::toString(p1, p2, p3, p4, p5));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {_MYGUI_LOG(util::toString(p1, p2, p3, p4, p5, p6));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {_MYGUI_LOG(util::toString(p1, p2, p3, p4, p5, p6, p7));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
	inline void _MYGUI_LOG (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {_MYGUI_LOG(util::toString(p1, p2, p3, p4, p5, p6, p7, p8));}

} // namespace MyGUI

#endif // __MYGUI_LOGING_H__
