/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_ALIGN_INFO_H__
#define __MYGUI_ALIGN_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"

namespace MyGUI
{

	typedef unsigned char Align;

	enum ALIGN_INFO
	{
		ALIGN_NONE						= FLAG_NONE,					// 
		ALIGN_HCENTER					= FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center
		ALIGN_CENTER_PARENT		    = FLAG(0),						// это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		ALIGN_LEFT						= FLAG(1),						// justify left
		ALIGN_RIGHT					= FLAG(2),						// justify right
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch to fill the entire parent window horizontally (?)
		ALIGN_TOP						= FLAG(3),						// align from the top
		ALIGN_BOTTOM					= FLAG(4),						// align from the bottom
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch to fill the entire parent window vertically (?)
		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch to fill the entire parent (?)
	};

	#define IS_ALIGN_NONE(align) (ALIGN_NONE == (align & ALIGN_NONE))
	#define IS_ALIGN_HCENTER(align) (ALIGN_HCENTER == (align & ALIGN_HCENTER))
	#define IS_ALIGN_VCENTER(align) (ALIGN_VCENTER == (align & ALIGN_VCENTER))
	#define IS_ALIGN_CENTER(align) (ALIGN_CENTER == (align & ALIGN_CENTER))
	#define IS_ALIGN_CENTER_PARENT(align) (ALIGN_CENTER_PARENT == (align & ALIGN_CENTER_PARENT))

	#define IS_ALIGN_LEFT(align) (ALIGN_LEFT == (align & ALIGN_LEFT))
	#define IS_ALIGN_RIGHT(align) (ALIGN_RIGHT == (align & ALIGN_RIGHT))
	#define IS_ALIGN_HSTRETCH(align) (ALIGN_HSTRETCH == (align & ALIGN_HSTRETCH))

	#define IS_ALIGN_TOP(align) (ALIGN_TOP == (align & ALIGN_TOP))
	#define IS_ALIGN_BOTTOM(align) (ALIGN_BOTTOM == (align & ALIGN_BOTTOM))
	#define IS_ALIGN_VSTRETCH(align) (ALIGN_VSTRETCH == (align & ALIGN_VSTRETCH))

	#define IS_ALIGN_STRETCH(align) (ALIGN_STRETCH == (align & ALIGN_STRETCH))

} // namespace MyGUI

#endif // __MYGUI_ALIGN_INFO_H__