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

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;
		void setupResources() override;

	private:
		void createDefaultScene();
		void notifyFrameStart(float _time);

	private:
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::SceneNode* mNode{nullptr};
#endif // MYGUI_OGRE_PLATFORM
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
