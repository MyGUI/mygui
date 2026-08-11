/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgPlatform.h"
#include <cassert>

namespace MyGUI
{

	OsgPlatform::OsgPlatform()
	{
		mRenderManager = new OsgRenderManager();
		mDataManager = new OsgDataManager();
		mLogManager = new LogManager();
	}

	OsgPlatform::~OsgPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
		delete mLogManager;
	}

	void OsgPlatform::initialise(osg::Group* _sceneRoot, int _width, int _height, std::string_view _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise(_sceneRoot, _width, _height);
		mDataManager->initialise();
	}

	void OsgPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	OsgRenderManager* OsgPlatform::getRenderManagerPtr() const
	{
		return mRenderManager;
	}

	OsgDataManager* OsgPlatform::getDataManagerPtr() const
	{
		return mDataManager;
	}

} // namespace MyGUI
