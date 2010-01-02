/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		//base::BaseManager::getInstance().addResourceLocation("../../Media/Wrapper");
		getGUI()->eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::destroyScene()
	{
		getGUI()->eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::notifyFrameStart(float _time)
	{
		if (mFrameStartDelegate  != nullptr)
			mFrameStartDelegate(_time);
	}

} // namespace demo
