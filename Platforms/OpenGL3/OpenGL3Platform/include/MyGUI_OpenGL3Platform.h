/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_PLATFORM_H_
#define MYGUI_OPENGL3_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_OpenGL3Diagnostic.h"
#include "MyGUI_OpenGL3RenderManager.h"
#include "MyGUI_OpenGL3DataManager.h"
#include "MyGUI_OpenGL3ImageLoader.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class OpenGL3Platform
	{
	public:
		OpenGL3Platform();
		~OpenGL3Platform();

		void initialise(OpenGL3ImageLoader* _loader, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		OpenGL3RenderManager* getRenderManagerPtr();
		OpenGL3DataManager* getDataManagerPtr();

	private:
		bool mIsInitialise;
		OpenGL3RenderManager* mRenderManager;
		OpenGL3DataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_PLATFORM_H_
