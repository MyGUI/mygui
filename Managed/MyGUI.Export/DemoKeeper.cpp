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
	}

	void DemoKeeper::destroyScene()
	{
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		bool ok = BaseManager::frameStarted(evt);
		if (ok && mFrameStartDelegate  != nullptr) mFrameStartDelegate(evt.timeSinceLastFrame);
		return ok;
	}

	bool DemoKeeper::frameEnded(const Ogre::FrameEvent& evt)
	{
		return BaseManager::frameEnded(evt);
	}

} // namespace demo
