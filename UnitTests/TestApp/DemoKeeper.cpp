#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WrapPanel.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"
#include "sigslot.h"

namespace demo
{
	class A :
		public sigslot::has_slots
	{
	public:
		void foo1(float _value)
		{
			delete this;
			int test = 0;
		}
		void foo0()
		{
			int test = 0;
		}
		static void foo()
		{
		}
	};

	class B
	{
	public:
		sigslot::signal1<float> signal1;
		sigslot::signal0 signal0;
	};

	DemoKeeper::DemoKeeper() :
		mBold(false),
		mItalic(false),
		mStrike(false),
		mUnder(false),
		mImage(false),
		mHeader1(false),
		mHeader2(false),
		mHeader3(false),
		mColour(false),
		mUrl(false)
	{
		A* a1 = new A();
		A* a2 = new A();
		B b;

		b.signal1.connect(a1, &A::foo1);
		b.signal1.connect(a2, &A::foo1);

		b.signal0.connect(a1, &A::foo0);
		b.signal0.connect(a2, &A::foo0);

		b.signal1.emit(2);
		//delete a1;

		//b.signal.emit(3);

		mSpacer.set(3, 3);
		mDefaultTextSkin = "TextBox";
		mDefaultParagraphSkin = "Default";
		//mDefaultTextSkin = "Button";
		//mDefaultParagraphSkin = "Button";
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
		MyGUI::ResourceManager::getInstance().load("Fonts.xml");

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::WrapPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::ScrollViewPanel>("Widget");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 400, 400), MyGUI::Align::Default, "Main");
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

		//addLine(stackPanel, "<p align='left'><h1>Caption1 left</h1></p>");
		//addLine(stackPanel, "<p align='center'><h2>Caption2 center</h2></p>");
		//addLine(stackPanel, "<p align='right'><h3>Caption3 right</h3></p>");
		//addLine(stackPanel, "<p><s>This is strike.</s></p>");
		//addLine(stackPanel, "<p><s><color value='#FF00FF'>This is strike and colour.</color></s></p>");
		//addLine(stackPanel, "<p><u>This is under.</u></p>");
		//addLine(stackPanel, "<p><color value='#FFFFFF'>This is color.</color></p>");
		//addLine(stackPanel, "<p><url value='http://www.google.com'>http://www.google.com</url></p>");
		//addLine(stackPanel, "<p>This is image.<img>HandPointerImage</img></p>");
		//addLine(stackPanel, "<p><b>This is bold text.</b></p>");
		//addLine(stackPanel, "<p><i>This is italic text.</i></p>");
		//addLine(stackPanel, "<p><i><b>This is bold and italic text.</b></i></p>");
		//addLine(stackPanel, "<p><i><b><s><u>This is bold and italic and under and strike text.</u></s></b></i></p>");
		//addLine(stackPanel, "<p>This is user tag.<character>user</character></p>");
		addLine(stackPanel, "<p float='left'><img width='64' height='64'>HandPointerImage</img>text texttext text text texttext texttext text texttext text texttext text texttext text texttext text</p>");

		stackPanel->setSpacer(MyGUI::IntSize(0, mSpacer.height));
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
		MyGUI::WrapPanel* panel = _parent->createWidget<MyGUI::WrapPanel>(mDefaultParagraphSkin, MyGUI::IntCoord(), MyGUI::Align::Default);
		panel->setContentAlign(MyGUI::Align::Left | MyGUI::Align::Bottom);
		panel->setSpacer(MyGUI::IntSize(mSpacer.width, 0));

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

		if (mImage)
		{
			if (mImageSize.width == 0 || mImageSize.height == 0)
			{
				MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().getByName(_value, false);
				if (resource != nullptr)
				{
					MyGUI::ResourceImageSet* imageSet = resource->castType<MyGUI::ResourceImageSet>(false);
					if (imageSet != nullptr)
					{
						MyGUI::ImageIndexInfo info = imageSet->getIndexInfo(0, 0);
						mImageSize = info.size;
					}
				}
			}

			MyGUI::ImageBox* image = _parent->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, mImageSize.width, mImageSize.height), MyGUI::Align::Default);
			image->setItemResource(_value);
		}
		else if (mUrl)
		{
			MyGUI::TextBox* text = _parent->createWidget<MyGUI::TextBox>(mDefaultTextSkin, MyGUI::IntCoord(0, 0, defaultSize, defaultSize), MyGUI::Align::Default);
			text->setCaption(_value);
			text->setPointer("link");

			MyGUI::Widget* line = text->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(0, defaultSize - 1, defaultSize, 1), MyGUI::Align::HStretch | MyGUI::Align::Bottom);
			line->setColour(MyGUI::Colour::Black);
			line->setVisible(false);
			line->setNeedMouseFocus(false);

			text->eventMouseLostFocus += MyGUI::newDelegate(this, &DemoKeeper::OnTextLostFocus);
			text->eventMouseSetFocus += MyGUI::newDelegate(this, &DemoKeeper::OnTextSetFocus);
			text->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::OnTextClick);
			text->setUserString("Url", mUrlValue);
		}
		else
		{
			MyGUI::VectorString result = MyGUI::utility::split(_value);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				MyGUI::TextBox* text = _parent->createWidget<MyGUI::TextBox>(mDefaultTextSkin, MyGUI::IntCoord(0, 0, defaultSize, defaultSize), MyGUI::Align::Default);
				text->setCaption(*item);
				if (mBold)
				{
					if (mItalic)
						text->setFontName("DejaVuSansBoldItalicFont.17");
					else
						text->setFontName("DejaVuSansBoldFont.17");
				}
				else if (mItalic)
				{
					text->setFontName("DejaVuSansItalicFont.17");
				}
				else if (mHeader1)
				{
					text->setFontName("DejaVuSansFont.26");
				}
				else if (mHeader2)
				{
					text->setFontName("DejaVuSansFont.25");
				}
				else if (mHeader3)
				{
					text->setFontName("DejaVuSansFont.20");
				}

				if (mColour)
				{
					text->setTextColour(mColourValue);
				}

				if (mStrike)
				{
					MyGUI::Widget* line = text->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(0, 0, defaultSize, 1), MyGUI::Align::HStretch | MyGUI::Align::VCenter);
					line->setColour(MyGUI::Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}

				if (mUnder)
				{
					MyGUI::Widget* line = text->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(0, defaultSize - 1, defaultSize, 1), MyGUI::Align::HStretch | MyGUI::Align::Bottom);
					line->setColour(MyGUI::Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}
			}
		}
	}

	void DemoKeeper::parseTag(MyGUI::Widget* _parent, const std::string& _value)
	{
		const std::string imageStartTagName = "<img";
		const std::string imageEndTagName = ">";

		const std::string paragraphStartTagName = "<p";
		const std::string paragraphEndTagName = ">";

		const std::string colourStartTagName = "<color";
		const std::string colourEndTagName = ">";

		const std::string urlStartTagName = "<url";
		const std::string urlEndTagName = ">";

		if (_value == "<h1>")
		{
			mHeader1 = true;
		}
		else if (_value == "</h1>")
		{
			mHeader1 = false;
		}
		else if (_value == "<h2>")
		{
			mHeader2 = true;
		}
		else if (_value == "</h2>")
		{
			mHeader2 = false;
		}
		else if (_value == "<h3>")
		{
			mHeader3 = true;
		}
		else if (_value == "</h3>")
		{
			mHeader3 = false;
		}
		else if (_value == "<b>")
		{
			mBold = true;
		}
		else if (_value == "</b>")
		{
			mBold = false;
		}
		else if (_value == "<i>")
		{
			mItalic = true;
		}
		else if (_value == "</i>")
		{
			mItalic = false;
		}
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
		else if (MyGUI::utility::startWith(_value, colourStartTagName))
		{
			mColour = true;
			mColourValue = MyGUI::Colour::Black;

			const std::string colourTagName = "value=";

			std::string valueColour = _value.substr(colourStartTagName.size(), _value.size() - (colourStartTagName.size() + colourEndTagName.size()));
			MyGUI::VectorString result = MyGUI::utility::split(valueColour);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (MyGUI::utility::startWith(*item, colourTagName))
				{
					if ((colourTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(colourTagName.size() + 1, (*item).size() - (colourTagName.size() + 2));
						mColourValue = MyGUI::Colour::parse(value);
					}
				}
			}
		}
		else if (_value == "</color>")
		{
			mColour = false;
		}
		else if (MyGUI::utility::startWith(_value, urlStartTagName))
		{
			mUrl = true;
			mUrlValue.clear();

			const std::string urlTagName = "value=";

			std::string valueUrl = _value.substr(urlStartTagName.size(), _value.size() - (urlStartTagName.size() + urlEndTagName.size()));
			MyGUI::VectorString result = MyGUI::utility::split(valueUrl);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (MyGUI::utility::startWith(*item, urlTagName))
				{
					if ((urlTagName.size() + 2) < ((*item).size()))
					{
						mUrlValue = (*item).substr(urlTagName.size() + 1, (*item).size() - (urlTagName.size() + 2));
					}
				}
			}
		}
		else if (_value == "</url>")
		{
			mUrl = false;
		}
		else if (MyGUI::utility::startWith(_value, paragraphStartTagName))
		{
			MyGUI::Align alignResult = MyGUI::Align::Default;
			bool needAlign = false;

			MyGUI::Align floatResult = MyGUI::Align::Default;
			bool needFloat = false;

			const std::string alightTagName = "align=";
			const std::string floatTagName = "float=";

			std::string valueParagraph = _value.substr(paragraphStartTagName.size(), _value.size() - (paragraphStartTagName.size() + paragraphEndTagName.size()));
			MyGUI::VectorString result = MyGUI::utility::split(valueParagraph);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (MyGUI::utility::startWith(*item, alightTagName))
				{
					if ((alightTagName.size() + 2) < ((*item).size()))
					{
						needAlign = true;
						std::string value = (*item).substr(alightTagName.size() + 1, (*item).size() - (alightTagName.size() + 2));
						if (value == "left")
							alignResult = MyGUI::Align::Default;
						else if (value == "center")
							alignResult = MyGUI::Align::HCenter | MyGUI::Align::Top;
						else if (value == "right")
							alignResult = MyGUI::Align::Right | MyGUI::Align::Top;
						else
							alignResult = MyGUI::Align::Default;
					}
					else if ((floatTagName.size() + 2) < ((*item).size()))
					{
						needFloat = true;
						std::string value = (*item).substr(floatTagName.size() + 1, (*item).size() - (floatTagName.size() + 2));
						if (value == "left")
							floatResult = MyGUI::Align::Default;
						else if (value == "center")
							floatResult = MyGUI::Align::HCenter | MyGUI::Align::Top;
						else if (value == "right")
							floatResult = MyGUI::Align::Right | MyGUI::Align::Top;
						else
							floatResult = MyGUI::Align::Default;
					}
				}
			}

			MyGUI::WrapPanel* panel = _parent->castType<MyGUI::WrapPanel>(false);
			if (panel != nullptr)
			{
				if (needAlign)
				{
					panel->setContentAlign(alignResult);
				}
				if (needFloat)
				{
					panel->setContentFloat(true);
					panel->setSnapFloat(floatResult);
				}
			}
		}
		else if (_value == "</p>")
		{
		}
		else if (MyGUI::utility::startWith(_value, imageStartTagName))
		{
			mImageSize.clear();
			mImage = true;

			const std::string widthTagName = "width=";
			const std::string heightTagName = "height=";

			std::string valueImage = _value.substr(imageStartTagName.size(), _value.size() - (imageStartTagName.size() + imageEndTagName.size()));
			MyGUI::VectorString result = MyGUI::utility::split(valueImage);
			for (MyGUI::VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (MyGUI::utility::startWith(*item, widthTagName))
				{
					if ((widthTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(widthTagName.size() + 1, (*item).size() - (widthTagName.size() + 2));
						mImageSize.width = MyGUI::utility::parseValue<int>(value);
					}
				}
				else if (MyGUI::utility::startWith(*item, heightTagName))
				{
					if ((heightTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(heightTagName.size() + 1, (*item).size() - (heightTagName.size() + 2));
						mImageSize.height = MyGUI::utility::parseValue<int>(value);
					}
				}
			}
		}
		else if (_value == "</img>")
		{
			mImage = false;
		}
	}

	void DemoKeeper::OnTextLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		size_t count = _sender->getChildCount();
		if (count > 0)
			_sender->getChildAt(0)->setVisible(false);
	}

	void DemoKeeper::OnTextSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		size_t count = _sender->getChildCount();
		if (count > 0)
			_sender->getChildAt(0)->setVisible(true);
	}

	void DemoKeeper::OnTextClick(MyGUI::Widget* _sender)
	{
		std::string url = _sender->getUserString("Url");
		int test = 0;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
