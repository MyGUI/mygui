#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "BasisState.h"
#include "EditorToolTip.h"
#include "PropertiesPanelView.h"
#include "SettingsWindow.h"
#include "WidgetsWindow.h"
#include "SaveLoadWindow.h"

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

	void loadSettings(std::string _fileName, bool _ogreResourse);
	void saveSettings(std::string _fileName, bool _ogreResourse);
	// main panel
	void notifySave();
	void notifySettings();
	void notifyTest();
	void notifyClear();
	void notifyQuit();

	template <bool Save>
	void saveOrLoadLayout(const std::string & _file);
private:
	void notifyClearMessage(MyGUI::MessagePtr _sender, MyGUI::MessageBoxStyle _result);
	void clear(bool _clearName = true);
	void notifyQuitMessage(MyGUI::MessagePtr _sender, MyGUI::MessageBoxStyle _result);

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
	static const size_t MAX_RECENT_FILES = 8;
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
	MyGUI::xml::Document * testLayout;

	EditorToolTip * mToolTip;

	PropertiesPanelView * mPropertiesPanelView;
	SettingsWindow * mSettingsWindow;
	WidgetsWindow * mWidgetsWindow;
	SaveLoadWindow * mSaveLoadWindow;

	EditorWidgets * ew;
	WidgetTypes * wt;
	UndoManager * um;

	MyGUI::Gui * mGUI;

	MyGUI::MenuBarPtr bar;
	MyGUI::MenuCtrlPtr mPopupMenuFile;
	MyGUI::MenuCtrlPtr mPopupMenuWidgets;
	std::vector<MyGUI::PopupMenu::ItemInfo> widgetMenus;
};
#endif // __EDITOR_STATE_H__
