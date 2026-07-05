#ifndef MYGUI_OGRE_NEXT_PLATFORM_H_
#define MYGUI_OGRE_NEXT_PLATFORM_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_OgreNextTexture.h"
#include "MyGUI_OgreNextVertexBuffer.h"
#include "MyGUI_OgreNextRenderManager.h"
#include "MyGUI_OgreNextDataManager.h"
#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_LogManager.h"

#include <OgreResourceGroupManager.h>

#include "MyGUI_LastHeader.h"

#include <cassert>

namespace MyGUI
{
	// Top-level facade for the OgreNext MyGUI platform. Owns the LogManager,
	// DataManager, and RenderManager. Users:
	//
	//   platform.initialise(sceneManager);
	//   // Option A: quick demo path - build a default workspace ourselves.
	//   platform.createDefaultWorkspace(window, camera);
	//   // Option B: user compositor - add `pass custom mygui { }` to your own
	//   //   workspace definition. Do not call createDefaultWorkspace.
	class OgreNextPlatform
	{
	public:
		OgreNextPlatform()
		{
			mLogManager = new LogManager();
			mDataManager = new OgreNextDataManager();
			mRenderManager = new OgreNextRenderManager();
		}

		~OgreNextPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(
			Ogre::SceneManager* scene,
			const std::string& resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			std::string_view logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!logName.empty())
				LogManager::getInstance().createDefaultSource(logName);

			mDataManager->initialise(resourceGroup);
			mRenderManager->initialise(scene);
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			destroyDefaultWorkspace();

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		// Convenience helper: create a single-node workspace that clears the
		// window and runs the MyGUI custom pass over it. Returns the workspace
		// pointer for callers who want to remove it manually; otherwise
		// destroyDefaultWorkspace / shutdown handle cleanup.
		Ogre::CompositorWorkspace* createDefaultWorkspace(Ogre::Window* window, Ogre::Camera* camera);
		void destroyDefaultWorkspace();

		OgreNextRenderManager* getRenderManagerPtr()
		{
			return mRenderManager;
		}
		OgreNextDataManager* getDataManagerPtr()
		{
			return mDataManager;
		}

	private:
		bool mIsInitialise{false};
		OgreNextRenderManager* mRenderManager{nullptr};
		OgreNextDataManager* mDataManager{nullptr};
		LogManager* mLogManager{nullptr};

		Ogre::CompositorWorkspace* mDefaultWorkspace{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_PLATFORM_H_
