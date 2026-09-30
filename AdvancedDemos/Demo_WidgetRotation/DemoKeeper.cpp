#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() / "Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		const MyGUI::VectorWidgetPtr& help = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (help.size() == 1)
			help.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption(
				"The window rotates 45 degrees. Its button adds 15 degrees. Click the button and type in the edit "
				"box.");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>(
			"WindowCS",
			MyGUI::IntCoord(200, 130, 420, 310),
			MyGUI::Align::Default,
			"Main");
		window->setCaption("Rotated widget subtree");
		window->setRotationCenter(MyGUI::FloatPoint(210.0f, 155.0f));
		window->setRotation(0.7853981634f);

		MyGUI::TextBox* text =
			window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(65, 65, 280, 45), MyGUI::Align::Default);
		text->setCaption("Text follows the parent rotation");

		MyGUI::Button* button =
			window->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(65, 135, 250, 44), MyGUI::Align::Default);
		button->setCaption("Click me (+15 degrees)");
		button->setRotation(0.2617993878f);
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::notifyButtonClick);

		MyGUI::EditBox* edit =
			window->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(65, 210, 250, 42), MyGUI::Align::Default);
		edit->setCaption("Click and type");
	}

	void DemoKeeper::notifyButtonClick(MyGUI::Widget* _sender)
	{
		++mClickCount;
		_sender->castType<MyGUI::Button>()->setCaption("Clicks: " + std::to_string(mClickCount));
	}

}

MYGUI_APP(demo::DemoKeeper)
