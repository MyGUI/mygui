/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

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

#endif // DEMO_KEEPER_H_
