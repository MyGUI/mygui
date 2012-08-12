/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportPlatform.h"

namespace MyGUI
{

	ExportPlatform::ExportPlatform() :
		mLogManager(nullptr),
		mRenderManager(nullptr),
		mDataManager(nullptr)
	{
		mLogManager = new LogManager();
		mRenderManager = new ExportRenderManager();
		mDataManager = new ExportDataManager();
	}

	ExportPlatform::~ExportPlatform()
	{
		delete mRenderManager;
		mRenderManager = nullptr;
		delete mDataManager;
		mDataManager = nullptr;

		MYGUI_PLATFORM_LOG(Info, "Shutdown logger");
		delete mLogManager;
		mLogManager = nullptr;
	}

	void ExportPlatform::initialise(const std::string& _logName)
	{
		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise();
		mDataManager->initialise();
	}

	void ExportPlatform::shutdown()
	{
		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	ExportRenderManager* ExportPlatform::getRenderManagerPtr()
	{
		return mRenderManager;
	}

	ExportDataManager* ExportPlatform::getDataManagerPtr()
	{
		return mDataManager;
	}

}
