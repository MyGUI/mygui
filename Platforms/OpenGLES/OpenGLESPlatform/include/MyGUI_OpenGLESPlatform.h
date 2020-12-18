#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_OpenGLESDiagnostic.h"
#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESDataManager.h"
#include "MyGUI_OpenGLESImageLoader.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class OpenGLESPlatform
	{
	public:
		OpenGLESPlatform();
		~OpenGLESPlatform();

		void initialise(OpenGLESImageLoader* _loader, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		OpenGLESRenderManager* getRenderManagerPtr() const;
		OpenGLESDataManager* getDataManagerPtr() const;

	private:
		bool mIsInitialise;
		OpenGLESRenderManager* mRenderManager;
		OpenGLESDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI
