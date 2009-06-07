/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		virtual bool frameStarted(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);

		void exit() { m_exit = true; }

		virtual void createGui() { }
		virtual void destroyGui() { }

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
