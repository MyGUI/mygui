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
		ALIGN_HCENTER					= FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center
		ALIGN_CENTER_PARENT		    = FLAG(0),						// do not use it in your application это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения

		ALIGN_LEFT						= FLAG(1),						// align from the left (and center the window vertically)
		ALIGN_RIGHT					= FLAG(2),						// align from the left (and center the window vertically)
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch horizontally proportionate to parent window
		ALIGN_TOP						= FLAG(3),						// align from the top (and center the window horizontally)
		ALIGN_BOTTOM					= FLAG(4),						// align from the bottom (and center the window horizontally)
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch vertically proportionate to parent window
		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch proportionate to parent window
		ALIGN_DEFAULT					= ALIGN_LEFT | ALIGN_TOP,

		ALIGN_LEFT_TOP             = ALIGN_LEFT | ALIGN_TOP,
		ALIGN_RIGHT_TOP             = ALIGN_RIGHT | ALIGN_TOP,
		ALIGN_RIGHT_BOTTOM             = ALIGN_RIGHT | ALIGN_BOTTOM,
		ALIGN_LEFT_BOTTOM             = ALIGN_LEFT | ALIGN_BOTTOM,
	};

	#define IS_ALIGN_HCENTER(align) (ALIGN_HCENTER == (align & (ALIGN_LEFT | ALIGN_RIGHT)))
	#define IS_ALIGN_VCENTER(align) (ALIGN_VCENTER == (align & (ALIGN_TOP | ALIGN_BOTTOM)))
	#define IS_ALIGN_CENTER(align) (ALIGN_CENTER == (align & (ALIGN_LEFT | ALIGN_RIGHT | ALIGN_TOP | ALIGN_BOTTOM)))
	#define IS_ALIGN_CENTER_PARENT(align) (ALIGN_CENTER_PARENT == (align & ALIGN_CENTER_PARENT))

	#define IS_ALIGN_LEFT(align) (ALIGN_LEFT == (align & ALIGN_LEFT))
	#define IS_ALIGN_RIGHT(align) (ALIGN_RIGHT == (align & ALIGN_RIGHT))
	#define IS_ALIGN_HSTRETCH(align) (ALIGN_HSTRETCH == (align & ALIGN_HSTRETCH))

	#define IS_ALIGN_TOP(align) (ALIGN_TOP == (align & ALIGN_TOP))
	#define IS_ALIGN_BOTTOM(align) (ALIGN_BOTTOM == (align & ALIGN_BOTTOM))
	#define IS_ALIGN_VSTRETCH(align) (ALIGN_VSTRETCH == (align & ALIGN_VSTRETCH))

	#define IS_ALIGN_STRETCH(align) (ALIGN_STRETCH == (align & ALIGN_STRETCH))
	#define IS_ALIGN_DEFAULT(align) (ALIGN_DEFAULT == (align & ALIGN_DEFAULT))

	#define IS_ALIGN_LEFT_TOP(align) (ALIGN_LEFT_TOP == (align & ALIGN_LEFT_TOP))
	#define IS_ALIGN_RIGHT_TOP(align) (ALIGN_RIGHT_TOP == (align & ALIGN_RIGHT_TOP))
	#define IS_ALIGN_RIGHT_BOTTOM(align) (ALIGN_RIGHT_BOTTOM == (align & ALIGN_RIGHT_BOTTOM))
	#define IS_ALIGN_LEFT_BOTTOM(align) (ALIGN_LEFT_BOTTOM == (align & ALIGN_LEFT_BOTTOM))

} // namespace MyGUI

#endif // __MYGUI_ALIGN_INFO_H__
