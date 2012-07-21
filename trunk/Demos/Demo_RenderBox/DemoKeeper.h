/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();

	private:
		void createDefaultScene();
		void notifyFrameStart(float _time);

	private:
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::SceneNode* mNode;
#endif // MYGUI_OGRE_PLATFORM
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
