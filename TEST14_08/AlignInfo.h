
#pragma once

namespace widget
{

	//Bit flags done easy
	#ifndef FLAG_NONE
		#define FLAG_NONE  0
		#define FLAG(num)  (1<<(num))
	#endif

	enum ALIGN_INFO {

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

} // namespace widget