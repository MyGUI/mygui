#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WrapPanel.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"

namespace demo
{
	DemoKeeper::DemoKeeper()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
		addResourceLocation(getRootMedia() + "/Common/Tools");
	}

	void DemoKeeper::createScene()
	{
		srand(GetTickCount());

		MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::WrapPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::ScrollViewPanel>("Widget");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 300, 300), MyGUI::Align::Default, "Main");
		window->eventWindowChangeCoord += MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);
		MyGUI::IntCoord coord = window->getClientCoord();
		//MyGUI::ScrollViewPanel* scrollViewPanel = window->createWidget<MyGUI::ScrollViewPanel>("ScrollView", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);
		MyGUI::StackPanel* stackPanel = window->createWidget<MyGUI::StackPanel>("PanelEmpty", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);

		MyGUI::TextBox* text = stackPanel->createWidget<MyGUI::TextBox>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default);
		//const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		//const char* fonts[3] = { "DejaVuSansFont.17", "DejaVuSansFont.14", "MicroFont.11" };
		//text->setCaption("test");
		//text->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		//text->setFontHeight(rand() % 50 + 10);
		//text->setFontName("MicroFont.11");
		//MyGUI::Panel::invalidateMeasure(text);

		//addText(stackPanel);

		for (size_t indexLine = 0; indexLine < 20; ++ indexLine)
		{
			MyGUI::WrapPanel* panel = stackPanel->createWidget<MyGUI::WrapPanel>("Button", MyGUI::IntCoord(), MyGUI::Align::Default);
			for (size_t index = 0; index < 20; ++ index)
			{
				if (rand() % 2)
					addText(panel);
				else
					addImage(panel);
			}
		}

		//MyGUI::Panel::invalidateMeasure(scrollViewPanel);
		notifyWindowChangeCoord(window);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::ScrollViewPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::WrapPanel>("Widget");
	}

	void DemoKeeper::addText(MyGUI::Widget* _parent)
	{
		MyGUI::TextBox* text = _parent->createWidget<MyGUI::TextBox>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default);
		const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		const char* fonts[3] = { "DejaVuSansFont.17", "DejaVuSansFont.14", "MicroFont.11" };
		text->setCaption(names[rand() % 8]);
		text->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		text->setFontHeight(rand() % 50 + 10);
		text->setFontName(fonts[rand() %3 ]);

		//MyGUI::Panel::invalidateMeasure(text);
	}

	void DemoKeeper::addImage(MyGUI::Widget* _parent)
	{
		int sizeImage = rand() % 32 + 16;
		MyGUI::ImageBox* image = _parent->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, sizeImage, sizeImage), MyGUI::Align::Default);
		const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		image->setItemResource(names[rand() % 8]);
	}

	void DemoKeeper::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		/*MyGUI::EnumeratorWidgetPtr child = _sender->getEnumerator();
		while (child.next())
		{
			MyGUI::Panel* panel = child->castType<MyGUI::Panel>(false);
			if (panel != nullptr)
				panel->invalidateMeasure();
		}*/
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
