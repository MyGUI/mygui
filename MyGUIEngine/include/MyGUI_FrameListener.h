/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_FRAME_LISTENER_H__
#define __MYGUI_FRAME_LISTENER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class FrameListener
	{
	public:
		virtual void _frameEntered(float _time) = 0;
	};

	typedef std::list<FrameListener*> ListFrameListener;

}

#endif // __MYGUI_FRAME_LISTENER_H__
