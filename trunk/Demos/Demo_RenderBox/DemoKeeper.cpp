/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#ifdef MYGUI_OGRE_PLATFORM

#include "Ogre/RenderBox/RenderBox.h"
#include "Ogre/RenderBox/RenderBoxScene.h"

namespace demo
{
	static wraps::RenderBox gRenderBox;
	static wraps::RenderBoxScene gRenderBoxScene;
}

#endif // MYGUI_OGRE_PLATFORM

namespace demo
{

	DemoKeeper::DemoKeeper()
	{
#ifdef MYGUI_OGRE_PLATFORM
		mNode = nullptr;
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia());
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		createDefaultScene();
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Demo of rendering camera view into widget and mesh into widget (you can drag it using mouse).");

		const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, size.height - 10 - 230, 300, 230), MyGUI::Align::Default, "Overlapped");
		window->setCaption("Camera view");
		window->setMinSize(MyGUI::IntSize(100, 100));
		MyGUI::Canvas* canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

		MyGUI::Window* window2 = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(size.width - 10 - 300, 10, 300, 230), MyGUI::Align::Default, "Overlapped");
		window2->setCaption("Model view");
		window2->setMinSize(MyGUI::IntSize(100, 100));
		MyGUI::Canvas* canvas2 = window2->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window2->getClientCoord().width, window2->getClientCoord().height), MyGUI::Align::Stretch);
		canvas2->setPointer("hand");

#ifdef MYGUI_OGRE_PLATFORM

		gRenderBox.setCanvas(canvas);
		gRenderBox.setViewport(getCamera());
		gRenderBox.setBackgroundColour(MyGUI::Colour::Black);

		gRenderBoxScene.setCanvas(canvas2);
		gRenderBoxScene.injectObject("Robot.mesh");
		gRenderBoxScene.setAutoRotation(true);
		gRenderBoxScene.setMouseRotation(true);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::destroyScene()
	{
#ifdef MYGUI_OGRE_PLATFORM
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStart);

		gRenderBox.destroy();
		gRenderBoxScene.destroy();

#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::createDefaultScene()
	{
#ifdef MYGUI_OGRE_PLATFORM
		try
		{
			Ogre::Entity* entity = getSceneManager()->createEntity("Mikki.mesh", "Mikki.mesh");
			mNode = getSceneManager()->getRootSceneNode()->createChildSceneNode();
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
			return;
		}

		try
		{
			Ogre::MeshManager::getSingleton().createPlane(
				"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

			Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
			entity->setMaterialName("Ground");
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
		}
#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::notifyFrameStart(float _time)
	{
#ifdef MYGUI_OGRE_PLATFORM
		if (mNode)
			mNode->yaw(Ogre::Radian(Ogre::Degree(_time * 10)));
#endif // MYGUI_OGRE_PLATFORM
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
