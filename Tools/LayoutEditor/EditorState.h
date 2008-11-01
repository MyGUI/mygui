#pragma once

#include "BasisState.h"
#include "EditorToolTip.h"
#include "PropertiesPanelView.h"
#include "SettingsWindow.h"
#include "WidgetsWindow.h"

class EditorWidgets;
class WidgetTypes;
class UndoManager;
struct WidgetContainer;

class EditorState : public BasisState
{
public:
	void enter(bool bIsChangeState);
	void exit();

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	bool frameStarted(const Ogre::FrameEvent& evt);

	void windowResize();

//===================================================================================

	void loadSettings(std::string _fileName);
	void saveSettings(std::string _fileName);
	// main panel
	void notifySave();
	void notifyLoadSaveAs(bool _save);
	void notifySettings();
	void notifyTest();
	void notifyClear();
	void notifyQuit();

	void load(const std::string & _file);
private:
	void notifyClearMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);
	void clear(bool _clearName = true);
	void notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);

	// save load message
	void notifyLoadSaveAccept(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget = 0); // calls previous method
	void notifyLoadSaveCancel(MyGUI::WidgetPtr _sender = 0);

	// menu bar
	void notifyWidgetsSelect(MyGUI::WidgetPtr _widget, size_t _index);
	void createWidgetPopup(WidgetContainer* _container, MyGUI::PopupMenuPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
	void notifyWidgetsUpdate(bool _fake = true);//FIXME нужен делегат без параметров

	void notifySelectWidget(MyGUI::WidgetPtr _sender);

	void notifyRecreate(bool _fake){recreate = true;};//FIXME нужен делегат без параметров
private:
	std::string getDescriptionString(MyGUI::WidgetPtr _widget, bool _print_name, bool _print_type, bool _print_skin);

	void notifyPopupMenuAccept(MyGUI::WidgetPtr _sender, MyGUI::PopupMenuPtr _menu, size_t _index);

	int toGrid(int _x){ return _x / grid_step * grid_step; }

	// tooltips
	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

	// конвертирует из анси строки в вайд строку
	Ogre::DisplayString anci_to_utf16(const std::string & _source);

	// конвертирует из вайд в анси
	std::string utf16_to_anci(const Ogre::DisplayString & _source);

	// last click for depth selecting
	int x, y;
	int selectDepth;

	MyGUI::WidgetPtr current_widget;
	// drop select after skin change
	bool recreate;

	// current settings
	//int grid_step;//FIXME_HOOK

	// last loaded/saved file name
	Ogre::String fileName;

	bool testMode;
	MyGUI::VectorWidgetPtr interfaceWidgets;
	MyGUI::xml::xmlDocument * testLayout;

	EditorToolTip mToolTip;

	PropertiesPanelView mPropertiesPanelView;
	SettingsWindow mSettingsWindow;
	WidgetsWindow mWidgetsWindow;

	EditorWidgets * ew;
	WidgetTypes * wt;
	UndoManager * um;

	MyGUI::Gui * mGUI;

	MyGUI::MenuBarPtr bar;
	MyGUI::PopupMenuPtr mPopupMenuFile;
	MyGUI::PopupMenuPtr mPopupMenuWidgets;
	std::vector<MyGUI::PopupMenu::ItemInfo> widgetMenus;
};
