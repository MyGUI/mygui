/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "DemoKeeper.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mFrameStartDelegate(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Wrapper/WrapperBaseApp");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::notifyFrameStart(float _time)
	{
		if (mFrameStartDelegate  != nullptr)
			mFrameStartDelegate(_time);
	}

	void DemoKeeper::setFrameEvent(HandleFrameStart _delegate)
	{
		mFrameStartDelegate = _delegate;
	}

} // namespace demo
