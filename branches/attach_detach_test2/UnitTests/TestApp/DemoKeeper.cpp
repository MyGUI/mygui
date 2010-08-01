#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "Widget.h"
#include "Gui.h"

namespace demo
{
	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::Widget* widgetTest = MyGUI::LayoutManager::getInstance().loadLayout("TestApp3.layout").at(0)->findWidget("Button");

		MyGUI::WidgetContainer* parent = widgetTest->getWidgetContainer();
		parent->detachWidget(widgetTest);
		MyGUI::Gui::getInstance().attachWidget(widgetTest);
		//parent->attachWidget(widgetTest);
		//widgetTest->setWidgetStyle(MyGUI::WidgetStyle::Popup);
		//widgetTest->setLayerName("Overlapped");
		//MyGUI::LayerManager::getInstance().attachToLayerNode("Overlapped", widgetTest);

		//widgetTest->setWidgetStyle(MyGUI::WidgetStyle::Child);
		//MyGUI::LayerManager::getInstance().detachFromLayer(widgetTest);
		//widgetTest->setSkinName("Edit");
		//MyGUI::LayerManager::getInstance().attachToLayerNode("Overlapped", widgetTest);
		

		//MyGUI::LayerManager::getInstance().detachFromLayer(widgetTest);
		//widgetTest->setWidgetStyle(MyGUI::WidgetStyle::Child);

		/*MyGUI::Gui2* gui = new MyGUI::Gui2();

		MyGUI::Control* widget = MyGUI::Gui2::getInstance().createChild();
		widget->changeSkin("");
		widget->setCoord(MyGUI::IntCoord(10, 10, 100, 100));

		MyGUI::Control* widget2 = widget->createChild();
		widget2->setCoord(MyGUI::IntCoord(10, 10, 50, 50));

		widget->changeSkin("WindowC");

		widget->destroyChild(widget2);

		MyGUI::Gui2::getInstance().destroyChild(widget);

		delete gui;*/
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
