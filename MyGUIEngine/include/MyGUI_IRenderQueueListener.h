/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_I_RENDER_QUEUE_LISTENER_H__
#define __MYGUI_I_RENDER_QUEUE_LISTENER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class MYGUI_EXPORT IRenderQueueListener 
	{
	public:
		virtual ~IRenderQueueListener() = 0 { }

		virtual void doRender(bool _update) = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_I_RENDER_QUEUE_LISTENER_H__
