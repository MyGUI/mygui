#include "Precompiled.h"
#include "BaseManager.h"

namespace base
{
	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		return true;
	}

	void BaseManager::destroyRender()
	{
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::DummyPlatform();
		mPlatform->initialise();
		setupResources();
	}

	void BaseManager::destroyGuiPlatform()
	{
		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::drawOneFrame()
	{
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
	}

}
