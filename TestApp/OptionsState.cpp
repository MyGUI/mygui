
#include "BasisManager.h"

#include "MyGUI_Gui.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayoutManager.h"
//#include "MyGUI_ParserManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_List.h"
#include "utility.h"

void OptionsState::enter(bool bIsChangeState)
{

	new MyGUI::Gui();
	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	// load plugin
//#if _DEBUG
//	MyGUI::PluginManager::getInstance().loadPlugin("TestPlugin_d.dll");
//#else
//	MyGUI::PluginManager::getInstance().loadPlugin("TestPlugin.dll");
//#endif


//	MyGUI::LayerManager::getInstance().load("main.layer");
	//MyGUI::PointerManager::getInstance().load("main.pointer");
	//MyGUI::PointerManager::getInstance().show();

	// загружаем скин
//	MyGUI::SkinManager::getInstance().load("main.skin");
	// загружаем лейаут
	//MyGUI::LayoutManager::getInstance().load("mygui.layout");

	//MyGUI::FontManager::getInstance().load("MyGUI.font");


	//MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonAutoAlpha"));
	//button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed1);

	/*button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonManualAlpha"));
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed2);*/


	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCS", 20, 20, 300, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	window->setCaption("caption");
	window->setAutoAlpha(true);
	window->showSmooth(true);

	MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(window->createWidget("Edit", "Edit", 10, 10, 200, 200, MyGUI::ALIGN_STRETCH));
	edit->setCaption(L"Мы только тем и дороги\nкому должны вернуть долги");

	//	MyGUI::ListPtr list = static_cast<MyGUI::ListPtr>(window.createWidget("List", "List", 10, 10, 200, 200, MyGUI::ALIGN_STRETCH));

	//MyGUI::StaticTextPtr st = static_cast<MyGUI::StaticTextPtr>(MyGUI::Gui::getInstance().createWidget("StaticText", "StaticText", 20, 20, 100, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	//st->setCaption("static");

	/*button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonLeft"));
	button->setCaption("<<");
	button->setUserData("button", 1);
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonRight"));
	button->setCaption(">>");
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	list1 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list1"));
	list2 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list2"));*/

	//MYGUI_EXCEPT("1");

	//MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("Button", "Button", 10, 10, 200, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	//button->setCaption(L"#00FF00хнопка\t=)");

//	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCS", 20, 20, 300, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
//	window->show(false, true);

	//MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(MyGUI::Gui::getInstance().createWidget("Edit", "Edit", 200, 50, 300, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	//edit->setCaption(L"Мы только тем и дороги");

//	MyGUI::LayoutManager::getInstance().load("mygui.layout");

//	MyGUI::WidgetManager::getInstance().destroyAllWidget();

//	MyGUI::LayoutManager::getInstance().load("mygui.layout");

	//MyGUI::LayoutManager::getInstance().load("mygui.layout");

//	MYGUI_EXCEPT("test");


/*	Ogre::OverlayManager & manager = Ogre::OverlayManager::getSingleton();
	Ogre::Overlay * overlay = manager.create("test1");
	overlay->setZOrder(100);
	overlay->show();
	Ogre::OverlayElement * element = manager.createOverlayElement("Panel", "test2");
	element->setMaterialName("test");
	element->setPosition(0.1, 0.1);
	element->setDimensions(0.5, 0.5);
	overlay->add2D((Ogre::OverlayContainer*)element);*/

	//MyGUI::Gui::getInstance().shutdown();
	//MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	//button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("Button", "Button", 10, 10, 200, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	//button->setCaption(L"#00FF00хнопка\t=)");

	//edit = static_cast<MyGUI::EditPtr>(MyGUI::Gui::getInstance().createWidget("Edit", "Edit", 200, 50, 300, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	//edit->setCaption(L"Мы только тем и дороги");//*/

	//MyGUI::LayoutManager::getInstance().load("mygui.layout");


	/*const std::string _file("test.log"); // файл лога
	std::ofstream stream;

	struct tm current_time;
	time_t info_time;
	time(&info_time);
	localtime_s(&current_time, &info_time );

	static bool first_run = true;
	if (first_run) {
		stream.open(_file.c_str(), std::ios_base::out);
		stream << " ---------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
		stream << "                                          logging report for : "
			<< std::setw(2) << std::setfill('0') << current_time.tm_mon + 1 << "/"
			<< std::setw(2) << std::setfill('0') << current_time.tm_mday << "/"
			<< std::setw(4) << std::setfill('0') << current_time.tm_year + 1900 << "   "
			<< std::setw(2) << std::setfill('0') << current_time.tm_hour << ":"
			<< std::setw(2) << std::setfill('0') << current_time.tm_min << ":"
			<< std::setw(2) << std::setfill('0') << current_time.tm_sec << std::endl;
		stream << " ---------------------------------------------------------------------------------------------------------------------------------- " << std::endl << std::endl;
		first_run = false;
		stream.close();
	}

	stream.open(_file.c_str(), std::ios_base::app);
	if (stream.is_open()) {

		stream
			<< std::setw(2) << std::setfill('0') << current_time.tm_hour << ":"
			<< std::setw(2) << std::setfill('0') << current_time.tm_min << ":"
			<< std::setw(2) << std::setfill('0') << current_time.tm_sec << "   "
			<< "dermo" << std::endl;


		stream.close();
	}*/

	//MyGUI::LayoutManager::getInstance().load("mygui.layout");


}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::keyPressed( const OIS::KeyEvent &arg )
{

	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool OptionsState::keyReleased( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyRelease(arg);
	return true;
}
//===================================================================================
void OptionsState::exit()
{
	//MyGUI::PluginManager::shutdown();
	MyGUI::Gui::getInstance().shutdown();
	delete MyGUI::Gui::getInstancePtr();
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
}
//===================================================================================
