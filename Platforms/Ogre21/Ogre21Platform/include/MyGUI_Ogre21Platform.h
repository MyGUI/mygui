/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE21_PLATFORM_H_
#define MYGUI_OGRE21_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Ogre21Texture.h"
#include "MyGUI_Ogre21VertexBuffer.h"
#include "MyGUI_Ogre21RenderManager.h"
#include "MyGUI_Ogre21DataManager.h"
#include "MyGUI_Ogre21Diagnostic.h"
#include "MyGUI_Ogre21Texture.h"
#include "MyGUI_LogManager.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class Ogre21Platform
	{
	public:
		Ogre21Platform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new Ogre21RenderManager();
			mDataManager = new Ogre21DataManager();
		}

		~Ogre21Platform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene, const std::string& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(_window, _scene);
			mDataManager->initialise(_group);
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		Ogre21RenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		Ogre21DataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		Ogre21RenderManager* mRenderManager;
		Ogre21DataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_PLATFORM_H_
