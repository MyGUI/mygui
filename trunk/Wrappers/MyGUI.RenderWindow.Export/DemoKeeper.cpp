/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "DemoKeeper.h"

namespace demo
{

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

} // namespace demo
