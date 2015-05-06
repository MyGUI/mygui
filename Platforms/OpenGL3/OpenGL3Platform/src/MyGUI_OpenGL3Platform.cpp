/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "MyGUI_OpenGL3Platform.h"
#include <assert.h>

namespace MyGUI
{

	OpenGL3Platform::OpenGL3Platform() :
		mIsInitialise(false)
	{
		mRenderManager = new OpenGL3RenderManager();
		mDataManager = new OpenGL3DataManager();
		mLogManager = new LogManager();
	}

	OpenGL3Platform::~OpenGL3Platform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
		delete mLogManager;
	}

	void OpenGL3Platform::initialise(OpenGL3ImageLoader* _loader, const std::string& _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise(_loader);
		mDataManager->initialise();
	}

	void OpenGL3Platform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	OpenGL3RenderManager* OpenGL3Platform::getRenderManagerPtr()
	{
		assert(mIsInitialise);
		return mRenderManager;
	}

	OpenGL3DataManager* OpenGL3Platform::getDataManagerPtr()
	{
		assert(mIsInitialise);
		return mDataManager;
	}

} // namespace MyGUI
