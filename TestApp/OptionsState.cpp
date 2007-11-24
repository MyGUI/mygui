
#include "BasisManager.h"

#include "Gui.h"
#include "PluginManager.h"
#include "LayerManager.h"
#include "PointerManager.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "LayoutManager.h"
#include "ParserManager.h"
#include "MyGUI_FontManager.h"
//#include "Window.h"
#include "Button.h"
#include "Edit.h"
//#include "HScroll.h"
//#include "VScroll.h"
//#include "List.h"
#include "utility.h"

void OptionsState::enter(bool bIsChangeState)
{

	new MyGUI::Gui();
	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	// load plugin
//#if _DEBUG
	MyGUI::PluginManager::getInstance().loadPlugin("TestPlugin_d.dll");
	//MyGUI::PluginManager::getInstance().unloadPlugin("TestPlugin_d.dll");
//#else
//	MyGUI::PluginManager::Instance()->loadPlugin("TestPlugin.dll");
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


/*	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::WidgetManager::getInstance().findWidget("windowListDemo"));
	window->show(false);
	//MyGUI::ParserManager::getInstance().parse(window, "Caption", util::utf8<void>("дерьмо"));

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonLeft"));
	button->setCaption("<<");
	button->setUserData("button", 1);
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonRight"));
	button->setCaption(">>");
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	list1 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list1"));
	list2 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list2"));*/

	MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("StrangeButton", "Button", 10, 10, 200, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	//button->setCaption(L"#00FF00хнопка\t=)");

	MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(MyGUI::Gui::getInstance().createWidget("Edit", "Edit", 200, 50, 300, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	edit->setCaption(L"Мы только тем и дороги");

	/*Ogre::FontPtr mpFont = Ogre::FontManager::getSingleton().getByName("MyGUI_font");
	if (!mpFont.isNull()) {
		const Ogre::MaterialPtr & material = mpFont->getMaterial();
		Ogre::Pass * pass = material->getTechnique(0)->getPass(0);
		pass->createTextureUnitState("char_select.png", 1);
		pass->getTextureUnitState(1)->setColourOperationEx(LBX_SOURCE1, LBS_CURRENT, LBS_CURRENT);
		pass->getTextureUnitState(1)->setAlphaOperation(LBX_BLEND_TEXTURE_ALPHA, LBS_TEXTURE, LBS_CURRENT);
	}*/

/*	Ogre::OverlayManager & manager = Ogre::OverlayManager::getSingleton();
	Ogre::Overlay * overlay = manager.create("test1");
	overlay->setZOrder(100);
	overlay->show();
	Ogre::OverlayElement * element = manager.createOverlayElement("Panel", "test2");
	element->setMaterialName("test");
	element->setPosition(0.1, 0.1);
	element->setDimensions(0.5, 0.5);
	overlay->add2D((Ogre::OverlayContainer*)element);*/

	/*MyGUI::Gui::getInstance().shutdown();
	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("Button", "Button", 10, 10, 200, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	button->setCaption(L"#00FF00хнопка\t=)");

	edit = static_cast<MyGUI::EditPtr>(MyGUI::Gui::getInstance().createWidget("Edit", "Edit", 200, 50, 300, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	edit->setCaption(L"Мы только тем и дороги");*/

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
