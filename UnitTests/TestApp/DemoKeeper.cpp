#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "WrapPanel.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"
#include "HyperTextBox.h"
#include "DataInfoManager.h"
#include "DataManager.h"
#include "CommandManager.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mDataListUI(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp/DataInfo");
		addResourceLocation(getRootMedia() + "/Common/Tools");
	}

	void DemoKeeper::createScene()
	{
		new tools::DataInfoManager();
		new tools::DataManager();
		new tools::ActionManager();
		
		tools::DataInfoManager::getInstance().initialise();
		tools::DataManager::getInstance().initialise();
		tools::ActionManager::getInstance().initialise();

		tools::DataInfoManager::getInstance().load("ImageDataInfo.xml");

		mDataListUI = new DataListUI();

		//MyGUI::LayoutManager::getInstance().load("");

		/*MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
		MyGUI::ResourceManager::getInstance().load("Fonts.xml");
		MyGUI::ResourceManager::getInstance().load("HyperTextSkins.xml");

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::WrapPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::ScrollViewPanel>("Widget");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::HyperTextBox>("Widget");

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

		hyperText->updateContent();*/
	}

	void DemoKeeper::destroyScene()
	{
		tools::DataInfoManager::getInstance().shutdown();
		tools::DataManager::getInstance().shutdown();
		tools::ActionManager::getInstance().shutdown();

		delete tools::DataManager::getInstancePtr();
		delete tools::DataInfoManager::getInstancePtr();
		delete tools::ActionManager::getInstancePtr();
		/*MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::HyperTextBox>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::ScrollViewPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::StackPanel>("Widget");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::WrapPanel>("Widget");*/
	}

	void DemoKeeper::OnClickUrl(MyGUI::HyperTextBox* _sender, const std::string& _url)
	{
		int test = 0;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
