/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
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
