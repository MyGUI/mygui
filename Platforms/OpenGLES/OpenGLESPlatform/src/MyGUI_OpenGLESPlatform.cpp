#include "MyGUI_OpenGLESPlatform.h"
#include <assert.h>

namespace MyGUI
{

	OpenGLESPlatform::OpenGLESPlatform() :
		mIsInitialise(false)
	{
		mRenderManager = new OpenGLESRenderManager();
		mDataManager = new OpenGLESDataManager();
		mLogManager = new LogManager();
	}

	OpenGLESPlatform::~OpenGLESPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
		delete mLogManager;
	}

	void OpenGLESPlatform::initialise(OpenGLESImageLoader* _loader, std::string_view _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise(_loader);
		mDataManager->initialise();
	}

	void OpenGLESPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	OpenGLESRenderManager* OpenGLESPlatform::getRenderManagerPtr() const
	{
		return mRenderManager;
	}

	OpenGLESDataManager* OpenGLESPlatform::getDataManagerPtr() const
	{
		return mDataManager;
	}

} // namespace MyGUI
