/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL_PLATFORM_H_
#define MYGUI_OPENGL_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDataManager.h"
#include "MyGUI_OpenGLImageLoader.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class OpenGLPlatform
	{
	public:
		OpenGLPlatform();
		~OpenGLPlatform();

		void initialise(OpenGLImageLoader* _loader, std::string_view _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		OpenGLRenderManager* getRenderManagerPtr() const;
		OpenGLDataManager* getDataManagerPtr() const;

	private:
		bool mIsInitialise{false};
		OpenGLRenderManager* mRenderManager;
		OpenGLDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL_PLATFORM_H_
