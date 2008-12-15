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

	EditorState() {};
	virtual ~EditorState() {};

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
	void notifyWidgetsSelect(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item);
	void createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrlPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
	void notifyWidgetsUpdate();

	void notifySelectWidget(MyGUI::WidgetPtr _sender);

	void notifyRecreate(){recreate = true;};
private:
	std::string getDescriptionString(MyGUI::WidgetPtr _widget, bool _print_name, bool _print_type, bool _print_skin);

	void createMainMenu();
	void notifyPopupMenuAccept(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item);

	int toGrid(int _x){ return _x / grid_step * grid_step; }

	// tooltips
	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

	// recent files
	static const int MAX_RECENT_FILES = 8;
	std::vector<Ogre::String> recentFiles;
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
	MyGUI::MenuCtrlPtr mPopupMenuFile;
	MyGUI::MenuCtrlPtr mPopupMenuWidgets;
	std::vector<MyGUI::PopupMenu::ItemInfo> widgetMenus;
};
