/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		virtual bool frameStarted(const Ogre::FrameEvent& evt);

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
