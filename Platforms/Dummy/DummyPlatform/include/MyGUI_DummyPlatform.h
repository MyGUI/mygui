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
#include "MyGUI_DummyDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class DummyPlatform
	{
	public:
		DummyPlatform() :
			mLogManager(nullptr),
			mRenderManager(nullptr),
			mDataManager(nullptr)
		{
			mLogManager = new LogManager();
			mRenderManager = new DummyRenderManager();
			mDataManager = new DummyDataManager();
		}

		~DummyPlatform()
		{
			delete mRenderManager;
			mRenderManager = nullptr;
			delete mDataManager;
			mDataManager = nullptr;
			delete mLogManager;
			mLogManager = nullptr;
		}

		void initialise(const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise();
			mDataManager->initialise();
		}

		void shutdown()
		{
			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		DummyRenderManager* getRenderManagerPtr()
		{
			return mRenderManager;
		}

		DummyDataManager* getDataManagerPtr()
		{
			return mDataManager;
		}

	private:
		DummyRenderManager* mRenderManager;
		DummyDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // __MYGUI_DUMMY_PLATFORM_H__
