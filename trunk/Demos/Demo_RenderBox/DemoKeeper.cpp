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
	wraps::RenderBox gRenderBox;
	wraps::RenderBoxScene gRenderBoxScene;
}

#endif // MYGUI_OGRE_PLATFORM

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia());
	}

	void DemoKeeper::createScene()
	{
		createDefaultScene();
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
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

#endif // MYGUI_OGRE_PLATFORM
	}

	void DemoKeeper::destroyScene()
	{
#ifdef MYGUI_OGRE_PLATFORM

		gRenderBox.destroy();
		gRenderBoxScene.destroy();

#endif // MYGUI_OGRE_PLATFORM
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
