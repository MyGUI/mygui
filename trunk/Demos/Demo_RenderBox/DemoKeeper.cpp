/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "RenderBox/RenderBoxWrap.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		setDescriptionText("Demo of rendering camera view into widget and mesh into widget (you can drag or zoom it using mouse).");

		Ogre::Entity* entity = this->mSceneMgr->createEntity("axes.mesh", "axes.mesh");
		mNode = this->mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mNode->attachObject(entity);
		mCamera->setPosition(20, 20, 20);

		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, 300, 300), MyGUI::Align::Default, "Overlapped");
		window->setCaption("Camera view");
		window->setMinSize(100, 100);
		MyGUI::RenderBoxPtr box = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);
		box->setViewport(getMainViewport());
		box->setBackgroundColour(MyGUI::Colour::Zero);

		MyGUI::WindowPtr window2 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(400, 10, 300, 300), MyGUI::Align::Default, "Overlapped");
		window2->setCaption("Model view");
		window2->setMinSize(100, 100);
		MyGUI::RenderBoxPtr box2 = window2->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(0, 0, window2->getClientCoord().width, window2->getClientCoord().height), MyGUI::Align::Stretch);
		box2->setBackgroundColour(MyGUI::Colour::Zero);
		box2->setPointer("hand");
		wraps::RenderBoxWrap * boxwrap = new wraps::RenderBoxWrap(box2);
		boxwrap->injectObject("axes.mesh");
		boxwrap->setMouseRotation(true);
		boxwrap->setViewScale(true);
		
		mGUI->eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::notifyFrameStart(float _time)
	{
		if (mNode) mNode->yaw(Ogre::Radian(Ogre::Degree(_time * 10)));
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
