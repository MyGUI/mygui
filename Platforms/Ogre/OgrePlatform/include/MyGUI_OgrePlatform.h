/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef __MYGUI_OGRE_PLATFORM_H__
#define __MYGUI_OGRE_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_OgreVertexBuffer.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_LogManager.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgrePlatform
	{
	public:
		OgrePlatform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new OgreRenderManager();
			mDataManager = new OgreDataManager();
		}

		~OgrePlatform()
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

		OgreRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		OgreDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		OgreRenderManager* mRenderManager;
		OgreDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_PLATFORM_H__
