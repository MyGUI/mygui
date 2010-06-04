/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "MyGUI_OpenGLPlatform.h"
#include <assert.h>

namespace MyGUI
{

	OpenGLPlatform::OpenGLPlatform() :
		mIsInitialise(false)
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

	void OpenGLPlatform::initialise(OpenGLImageLoader* _loader, const std::string& _logName)
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

	OpenGLRenderManager* OpenGLPlatform::getRenderManagerPtr()
	{
		assert(mIsInitialise);
		return mRenderManager;
	}

	OpenGLDataManager* OpenGLPlatform::getDataManagerPtr()
	{
		assert(mIsInitialise);
		return mDataManager;
	}

} // namespace MyGUI
