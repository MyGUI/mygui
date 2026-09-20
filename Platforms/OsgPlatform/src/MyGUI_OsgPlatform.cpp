/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgPlatform.h"
#include <cassert>
#include <osgDB/Registry>

#ifdef OSG_LIBRARY_STATIC
USE_OSGPLUGIN(png)
#endif

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

		// Use the PNG plugin on macOS too, through normal osgDB dispatch.
		osgDB::Registry::instance()->addFileExtensionAlias("png", "png");
		mRenderManager->initialise(_sceneRoot, _width, _height);
	}

	void OsgPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
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
