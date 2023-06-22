/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#ifndef MYGUI_SDL2_PLATFORM_H_
#define MYGUI_SDL2_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_SDL2RenderManager.h"
#include "MyGUI_SDL2DataManager.h"
#include "MyGUI_SDL2Diagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class SDL2Platform
	{
	public:
		SDL2Platform() :
			mLogManager(nullptr),
			mRenderManager(nullptr),
			mDataManager(nullptr)
		{
			mLogManager = new LogManager();
			mRenderManager = new SDL2RenderManager();
			mDataManager = new SDL2DataManager();
		}

		~SDL2Platform()
		{
			delete mRenderManager;
			mRenderManager = nullptr;
			delete mDataManager;
			mDataManager = nullptr;
			delete mLogManager;
			mLogManager = nullptr;
		}

		void initialise(SDL2ImageLoader *loader, SDL_Renderer *renderer, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(loader, renderer);
			mDataManager->initialise();
		}

		void shutdown()
		{
			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		SDL2RenderManager* getRenderManagerPtr()
		{
			return mRenderManager;
		}

		SDL2DataManager* getDataManagerPtr()
		{
			return mDataManager;
		}

	private:
		SDL2RenderManager* mRenderManager;
		SDL2DataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_PLATFORM_H_
