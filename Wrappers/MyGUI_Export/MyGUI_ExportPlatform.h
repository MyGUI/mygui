/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef __MYGUI_EXPORT_PLATFORM_H__
#define __MYGUI_EXPORT_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDataManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class ExportPlatform
	{
	public:
		ExportPlatform() :
			mLogManager(nullptr),
			mRenderManager(nullptr),
			mDataManager(nullptr)
		{
			mLogManager = new LogManager();
			mRenderManager = new ExportRenderManager();
			mDataManager = new ExportDataManager();
		}

		~ExportPlatform()
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

		ExportRenderManager* getRenderManagerPtr()
		{
			return mRenderManager;
		}

		ExportDataManager* getDataManagerPtr()
		{
			return mDataManager;
		}

	private:
		ExportRenderManager* mRenderManager;
		ExportDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // __MYGUI_EXPORT_PLATFORM_H__
