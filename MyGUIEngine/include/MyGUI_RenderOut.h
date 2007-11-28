/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_RENDER_OUT_H__
#define __MYGUI_RENDER_OUT_H__

#include "MyGUI_Prerequest.h"
#include "utility.h"

namespace MyGUI
{
	void _MyGUIExport MYGUI_OUT(const std::string & _value);

	template< class T1,  class T2 >
	inline void MYGUI_OUT (T1 p1, T2 p2)	{MYGUI_OUT(util::toString(p1, p2));}

	template< class T1,  class T2,  class T3 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3) {MYGUI_OUT(util::toString(p1, p2, p3));}

	template< class T1,  class T2,  class T3, class T4 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3, T4 p4) {MYGUI_OUT(util::toString(p1, p2, p3, p4));}

	template< class T1,  class T2,  class T3, class T4, class T5 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)	{MYGUI_OUT(util::toString(p1, p2, p3, p4, p5));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {MYGUI_OUT(util::toString(p1, p2, p3, p4, p5, p6));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {MYGUI_OUT(util::toString(p1, p2, p3, p4, p5, p6, p7));}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
	inline void MYGUI_OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {MYGUI_OUT(util::toString(p1, p2, p3, p4, p5, p6, p7, p8));}

} // namespace MyGUI

#endif // __MYGUI_RENDER_OUT_H__