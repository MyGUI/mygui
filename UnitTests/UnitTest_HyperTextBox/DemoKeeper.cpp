#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WrapPanel.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"
#include "HyperTextBox.h"

namespace demo
{
	DemoKeeper::DemoKeeper()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_HyperTextBox");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::ResourceManager::getInstance().load("Fonts.xml");
		MyGUI::ResourceManager::getInstance().load("HyperTextSkins.xml");

		std::string category = MyGUI::WidgetManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::WrapPanel>(category);
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::StackPanel>(category);
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::ScrollViewPanel>(category);
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::HyperTextBox>(category);

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10, 10, 500, 500), MyGUI::Align::Default, "Main");
		MyGUI::IntCoord coord = window->getClientCoord();

		MyGUI::HyperTextBox* hyperText = window->createWidget<MyGUI::HyperTextBox>("HyperTextBox", MyGUI::IntCoord(0, 0, coord.width, coord.height), MyGUI::Align::Stretch);
		hyperText->eventUrlClick += MyGUI::newDelegate(this, &DemoKeeper::OnClickUrl);

		hyperText->setCaption(
			"<p align='left'><h1>Caption1 left</h1></p>\n"\
			"<p align='center'><h2>Caption2 center</h2></p>\n"\
			"<p align='right'><h3>Caption3 right</h3></p>\n"\
			"<p><s>This is strike.</s></p>\n"\
			"<p><s><color value='#FF00FF'>This is strike and colour.</color></s></p>\n"\
			"<p><u>This is under.</u></p>\n"\
			"<p><color value='#FFFFFF'>This is color.</color></p>\n"\
			"<br/>\n"\
			"<p><url value='url_text1'>http://www.mygui.info</url></p>\n"\
			"<p><i><b><url value='url_text2'>http://www.mygui.info</url></b></i></p>\n"\
			"<p>This is image.<img>HandPointerImage</img></p>\n"\
			"<p>This is linked image.<url value='url_img'><img>HandPointerImage</img></url></p>\n"\
			"<p><b>This is bold text.</b></p>\n"\
			"<p><i>This is italic text.</i></p>\n"\
			"<p><t/><i><b>This is bold and italic text.</b></i></p>\n"\
			"<p><i><b><s><u>This is bold and italic and under and strike text.</u></s></b></i></p>\n"\
			"<p float='left'><img width='48' height='48'>HandPointerImage</img>text1 texttext2 text3 text4 texttext5 texttexttexttext6 text7 text8 texttext9 text10 texttext11 text12</p>\n"\
			"<p float='right' align='right'><img width='48' height='48'>HandPointerImage</img>text1 texttext2 text3 text4 texttext5 texttexttexttext6 text7 text8 texttext9 text10 texttext11 text12</p>");

		hyperText->updateContent();
	}

	void DemoKeeper::destroyScene()
	{
		std::string category = MyGUI::WidgetManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::HyperTextBox>(category);
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::ScrollViewPanel>(category);
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::StackPanel>(category);
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::WrapPanel>(category);
	}

	void DemoKeeper::OnClickUrl(MyGUI::HyperTextBox* _sender, const std::string& _url)
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
