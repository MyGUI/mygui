#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WrapPanel.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"

namespace demo
{
	DemoKeeper::DemoKeeper() :
		mBold(false),
		mCaption(false),
		mStrike(false),
		mUnder(false)
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
		//window->eventWindowChangeCoord += MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);
		MyGUI::IntCoord coord = window->getClientCoord();

		MyGUI::ScrollViewPanel* scrollViewPanel = window->createWidget<MyGUI::ScrollViewPanel>("ScrollView", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);
		MyGUI::StackPanel* stackPanel = scrollViewPanel->createWidget<MyGUI::StackPanel>("PanelEmpty", MyGUI::IntCoord(), MyGUI::Align::Default);
		//MyGUI::StackPanel* stackPanel = window->createWidget<MyGUI::StackPanel>("PanelEmpty", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);

		//MyGUI::TextBox* text = stackPanel->createWidget<MyGUI::TextBox>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default);
		//const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		//const char* fonts[3] = { "DejaVuSansFont.17", "DejaVuSansFont.14", "MicroFont.11" };
		//text->setCaption("test");
		//text->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		//text->setFontHeight(rand() % 50 + 10);
		//text->setFontName("MicroFont.11");
		//MyGUI::Panel::invalidateMeasure(text);

		//std::string text = "<h1>Header</h1>This is <b>text</b></body>";
		//scrollViewPanel->addItem("Header");

		//addText(stackPanel);

		/*for (size_t indexLine = 0; indexLine < 3; ++ indexLine)
		{
			MyGUI::WrapPanel* panel = stackPanel->createWidget<MyGUI::WrapPanel>("Button", MyGUI::IntCoord(), MyGUI::Align::Default);
			panel->setContentAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
			panel->setSpacer(MyGUI::IntSize(10, 0));

			for (size_t index = 0; index < 20; ++ index)
			{
				if (rand() % 2)
					addText(panel);
				else
					addImage(panel);
			}
		}*/
		addLine(stackPanel, "<p align='center'>Caption</p>");
		addLine(stackPanel, "<p><s>This is strike.</s></p>");
		addLine(stackPanel, "<p><u>This is under.</u></p>");
		addLine(stackPanel, "<p>This <b>is</b> text.</p>");
		addLine(stackPanel, "<p>This is image.<img width='32' height='32' src='HandPointerImage'/></p>");
		//addLine(stackPanel, "<p>This is image.<img width='32' height='32'>HandPointerImage</img></p>");
		//addLine(stackPanel, "<p>This is user tag.<character>user</character></p>");

		scrollViewPanel->setCanvasAlign(MyGUI::Align::Default);
		scrollViewPanel->setVisibleHScroll(false);
		scrollViewPanel->updateContent();
		//notifyWindowChangeCoord(window);

		// времено, так как не правильно обновляется иерархия
		//MyGUI::Panel::updateArrange(scrollViewPanel, scrollViewPanel->getCoord());

		//MyGUI::Panel::updateMeasure(scrollViewPanel, MyGUI::IntSize(0, 0));
		//MyGUI::Panel::invalidateMeasure(scrollViewPanel);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::ScrollViewPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::WrapPanel>("Widget");
	}

	/*void DemoKeeper::addText(MyGUI::Widget* _parent)
	{
		MyGUI::TextBox* text = _parent->createWidget<MyGUI::TextBox>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default);
		const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		const char* fonts[3] = { "DejaVuSansFont.17", "DejaVuSansFont.14", "MicroFont.11" };
		text->setCaption(names[rand() % 8]);
		text->setTextAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		text->setFontHeight(rand() % 50 + 10);
		text->setFontName(fonts[rand() %3 ]);

		//MyGUI::Panel::invalidateMeasure(text);
	}*/

	/*void DemoKeeper::addImage(MyGUI::Widget* _parent)
	{
		int sizeImage = rand() % 32 + 16;
		MyGUI::ImageBox* image = _parent->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, sizeImage, sizeImage), MyGUI::Align::Default);
		const char* names[8] = { "ArrowPointerImage", "BeamPointerImage", "SizeLeftPointerImage", "SizeRightPointerImage", "SizeHorzPointerImage", "SizeVertPointerImage", "HandPointerImage", "LinkPointerImage" };
		image->setItemResource(names[rand() % 8]);
	}*/

	//void DemoKeeper::notifyWindowChangeCoord(MyGUI::Window* _sender)
	//{
		/*MyGUI::EnumeratorWidgetPtr child = _sender->getEnumerator();
		while (child.next())
		{
			MyGUI::Panel* panel = child->castType<MyGUI::Panel>(false);
			if (panel != nullptr)
				panel->invalidateMeasure();
		}*/
	//}

	void DemoKeeper::addLine(MyGUI::Widget* _parent, const std::string& _value)
	{
		MyGUI::WrapPanel* panel = _parent->createWidget<MyGUI::WrapPanel>("Button", MyGUI::IntCoord(), MyGUI::Align::Default);
		panel->setContentAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		panel->setSpacer(MyGUI::IntSize(10, 0));

		std::string::const_iterator textItem = _value.end();
		for (std::string::const_iterator item = _value.begin(); item != _value.end(); ++ item)
		{
			if ((*item) == '<')
			{
				// отрезаем текст
				if (textItem < item)
					parseText(panel, _value.substr(textItem - _value.begin(), item - textItem));

				// ищем конец тега
				for (std::string::const_iterator tagItem = item; tagItem != _value.end(); ++ tagItem)
				{
					if ((*tagItem) == '>')
					{
						parseTag(panel, _value.substr(item - _value.begin(), tagItem - item + 1));
						item = tagItem;
						textItem = tagItem + 1;
						break;
					}
				}
			}
		}
	}

	void DemoKeeper::parseText(MyGUI::Widget* _parent, const std::string& _value)
	{
		const int defaultSize = 16;

		MyGUI::VectorString result = MyGUI::utility::split(_value);
		for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
		{
			MyGUI::TextBox* text = _parent->createWidget<MyGUI::TextBox>("Button", MyGUI::IntCoord(0, 0, defaultSize, defaultSize), MyGUI::Align::Default);
			text->setCaption(*item);
			if (mBold)
			{
				text->setFontHeight(30);
			}
			else if (mStrike)
			{
				MyGUI::Widget* line = text->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(0, 0, defaultSize, 1), MyGUI::Align::HStretch | MyGUI::Align::VCenter);
				line->setColour(MyGUI::Colour::Black);
			}
			else if (mUnder)
			{
				MyGUI::Widget* line = text->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(0, defaultSize - 5, defaultSize, 1), MyGUI::Align::HStretch | MyGUI::Align::Bottom);
				line->setColour(MyGUI::Colour::Black);
			}
			else if (mCaption)
			{
				// достаточно один раз сделать
				MyGUI::WrapPanel* panel = _parent->castType<MyGUI::WrapPanel>(false);
				if (panel != nullptr)
					panel->setContentAlign(MyGUI::Align::Center);
			}
		}
	}

	void DemoKeeper::parseTag(MyGUI::Widget* _parent, const std::string& _value)
	{
		const std::string imageStartTagName = "<img ";
		const std::string imageEndTagName = "/>";
		const std::string paragraphStartTagName = "<p ";
		const std::string paragraphEndTagName = ">";

		if (_value == "<b>")
		{
			mBold = true;
		}
		else if (_value == "</b>")
		{
			mBold = false;
		}
		/*else if (_value == "<h1>")
		{
			mCaption = true;
		}
		else if (_value == "</h1>")
		{
			mCaption = false;
		}*/
		else if (_value == "<s>")
		{
			mStrike = true;
		}
		else if (_value == "</s>")
		{
			mStrike = false;
		}
		else if (_value == "<u>")
		{
			mUnder = true;
		}
		else if (_value == "</u>")
		{
			mUnder = false;
		}
		else if (MyGUI::utility::startWith(_value, paragraphStartTagName))
		{
			mCaption = true;
		}
		else if (_value == "</p>")
		{
			mCaption = false;
		}
		else if (MyGUI::utility::startWith(_value, imageStartTagName) && MyGUI::utility::endWith(_value, imageEndTagName))
		{
			MyGUI::IntSize size;
			std::string source;

			const std::string widthTagName = "width=";
			const std::string heightTagName = "height=";
			const std::string srcTagName = "src=";

			std::string valueImage = _value.substr(imageStartTagName.size(), _value.size() - (imageStartTagName.size() + imageEndTagName.size()));
			MyGUI::VectorString result = MyGUI::utility::split(valueImage);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (MyGUI::utility::startWith(*item, widthTagName))
				{
					if ((widthTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(widthTagName.size() + 1, (*item).size() - (widthTagName.size() + 2));
						size.width = MyGUI::utility::parseValue<int>(value);
					}
				}
				else if (MyGUI::utility::startWith(*item, heightTagName))
				{
					if ((heightTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(heightTagName.size() + 1, (*item).size() - (heightTagName.size() + 2));
						size.height = MyGUI::utility::parseValue<int>(value);
					}
				}
				else if (MyGUI::utility::startWith(*item, srcTagName))
				{
					if ((srcTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(srcTagName.size() + 1, (*item).size() - (srcTagName.size() + 2));
						source = value;
					}
				}
			}

			if (!source.empty() && size.width != 0 && size.height != 0)
			{
				MyGUI::ImageBox* image = _parent->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Default);
				image->setItemResource(source);
			}
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
