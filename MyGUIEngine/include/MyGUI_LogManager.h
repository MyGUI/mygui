/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LOG_MANAGER_H__
#define __MYGUI_LOG_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "utility.h"

namespace MyGUI
{

	class _MyGUIExport LogManager
	{
		INSTANCE_HEADER(LogManager);

	public:
		void out(const std::string _value);

		template< class T1,  class T2 >
		inline void out (T1 p1, T2 p2) {out(util::toString(p1, p2));}

		template< class T1,  class T2,  class T3 >
		inline void out (T1 p1, T2 p2, T3 p3) {out(util::toString(p1, p2, p3));}

		template< class T1,  class T2,  class T3, class T4 >
		inline void out (T1 p1, T2 p2, T3 p3, T4 p4) {out(util::toString(p1, p2, p3, p4));}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void out (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)	{out(util::toString(p1, p2, p3, p4, p5));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void out (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {out(util::toString(p1, p2, p3, p4, p5, p6));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
		inline void out (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {out(util::toString(p1, p2, p3, p4, p5, p6, p7));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
		inline void out (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {out(util::toString(p1, p2, p3, p4, p5, p6, p7, p8));}

	}; // class LogManager

} // namespace MyGUI

#endif // __MYGUI_LOG_MANAGER_H__