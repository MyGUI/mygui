/*!
	@file
	@author		Albert Semenov
	@date		09/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_OpenGLPlatform.h"

namespace MyGUI
{

	OpenGLPlatform::OpenGLPlatform() :
		mIsInitialise(false)
	{
		mRenderManager = new OpenGLRenderManager();
		mDataManager = new OpenGLDataManager();
	}

	OpenGLPlatform::~OpenGLPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
	}

	void OpenGLPlatform::initialise(OpenGLImageLoader* _loader, const std::string& _logname)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		LogManager::registerSection(MYGUI_PLATFORM_LOG_SECTION, _logname);

		mRenderManager->initialise(_loader);
		mDataManager->initialise();
	}

	void OpenGLPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();

		// last platform log
		LogManager::unregisterSection(MYGUI_PLATFORM_LOG_SECTION);
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
