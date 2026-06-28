/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "MyGUI_OpenGLPlatform.h"
#include <cassert>

namespace MyGUI
{

	OpenGLPlatform::OpenGLPlatform()
	{
		mRenderManager = new OpenGLRenderManager();
		mDataManager = new OpenGLDataManager();
		mLogManager = new LogManager();
	}

	OpenGLPlatform::~OpenGLPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
		delete mLogManager;
	}

	void OpenGLPlatform::initialise(OpenGLImageLoader* _loader, std::string_view _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise(_loader);
		mDataManager->initialise();
	}

	void OpenGLPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	OpenGLRenderManager* OpenGLPlatform::getRenderManagerPtr() const
	{
		return mRenderManager;
	}

	OpenGLDataManager* OpenGLPlatform::getDataManagerPtr() const
	{
		return mDataManager;
	}

} // namespace MyGUI
