/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/
/*
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
#ifndef __MYGUI_OPENGL_PLATFORM_H__
#define __MYGUI_OPENGL_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDataManager.h"
#include "MyGUI_OpenGLImageLoader.h"

namespace MyGUI
{

	class OpenGLPlatform
	{
	public:
		OpenGLPlatform();
		~OpenGLPlatform();

		void initialise(OpenGLImageLoader* _loader = 0, const std::string& _logname = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		OpenGLRenderManager* getRenderManagerPtr();
		OpenGLDataManager* getDataManagerPtr();

	private:
		bool mIsInitialise;
		OpenGLRenderManager* mRenderManager;
		OpenGLDataManager* mDataManager;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_PLATFORM_H__
