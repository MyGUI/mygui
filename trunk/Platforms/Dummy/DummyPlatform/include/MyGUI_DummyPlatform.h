/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#ifndef __MYGUI_DUMMY_PLATFORM_H__
#define __MYGUI_DUMMY_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DummyRenderManager.h"
#include "MyGUI_DummyDataManager.h"
#include "MyGUI_DummyTexture.h"
#include "MyGUI_DummyVertexBuffer.h"
#include "MyGUI_DummyDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class DummyPlatform
	{
	public:
		DummyPlatform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new DummyRenderManager();
			mDataManager = new DummyDataManager();
		}

		~DummyPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(/*IDirect3DDevice9* _device, */const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(/*_device*/);
			mDataManager->initialise();
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		DummyRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		DummyDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		DummyRenderManager* mRenderManager;
		DummyDataManager* mDataManager;
		LogManager* mLogManager;

	};

} // namespace MyGUI

#endif // __MYGUI_DUMMY_PLATFORM_H__
