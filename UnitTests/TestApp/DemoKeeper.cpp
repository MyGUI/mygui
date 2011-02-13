#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WordWrapPanel.h"

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
		MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::WordWrapPanel>("Widget");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 100, 100), MyGUI::Align::Default, "Main");
		MyGUI::IntCoord coord = window->getClientCoord();
		MyGUI::WordWrapPanel* panel = window->createWidget<MyGUI::WordWrapPanel>("PanelEmpty", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);

		panel->setFreezeContent(true);

		srand(GetTickCount());

		for (size_t index = 0; index < 60; ++ index)
		{
			if (rand() % 2)
				addText(panel);
			else
				addImage(panel);
		}

		panel->setFreezeContent(false);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::WordWrapPanel>("Widget");
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
	}

	void DemoKeeper::addImage(MyGUI::Widget* _parent)
	{
		int sizeImage = rand() % 32 + 16;
		MyGUI::ImageBox* image = _parent->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, sizeImage, sizeImage), MyGUI::Align::Default);
		const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		image->setItemResource(names[rand() % 8]);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
