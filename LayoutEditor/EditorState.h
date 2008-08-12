#pragma once

#include "BasisState.h"
#include "MyGUI.h"

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

	void loadSettings();
	void saveSettings();
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
	void clear();
	void notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button);

	// save load message
	void notifyLoadSaveAccept(MyGUI::WidgetPtr _sender);
	void notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget = 0); // calls previous method
	void notifyLoadSaveCancel(MyGUI::WidgetPtr _sender = 0);

	// widget panel
	void notifySelectWidgetType(MyGUI::WidgetPtr _sender);
	void notifySelectWidgetTypeDoubleclick(MyGUI::WidgetPtr _sender);
	void notifyWidgetsUpdate();
	void createWidgetPopup(WidgetContainer* _container, MyGUI::PopupMenuPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
	void notifyWidgetsSelect(MyGUI::WidgetPtr _widget, size_t _index);

	// settings panel
	void notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new = 0);
	void notifyNewGridStepAccept(MyGUI::WidgetPtr _sender); // calls previous method
	void notifyOkSettings(MyGUI::WidgetPtr _sender);
	void notifyToggleCheck(MyGUI::WidgetPtr _sender);

	// widget selecting, properties panel
	void notifySelectWidget(MyGUI::WidgetPtr _sender);

	void updatePropertiesPanel(MyGUI::WidgetPtr _widget);
	void createPropertiesWidgetsPair(MyGUI::WidgetPtr _window, Ogre::String _property, Ogre::String _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h);
	void notifyApplyProperties(MyGUI::WidgetPtr _sender);
	void notifyApplyPropertiesCombo(MyGUI::WidgetPtr _widget); // calls previous method
	void notifyToggleRelativeMode(MyGUI::WidgetPtr _sender = 0);

	std::string getDescriptionString(MyGUI::WidgetPtr _widget, bool _print_name, bool _print_type, bool _print_skin);

	void notifyPopupMenuAccept(MyGUI::WidgetPtr _sender, MyGUI::PopupMenuPtr _menu, size_t _index);

	// widget editing
	void notifyRectangleResize(MyGUI::WidgetPtr _sender);
	void notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender);
	void notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
	void addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption = "");

	// panels mnimizing
	void notifyMinimizePanel(MyGUI::WidgetPtr _sender);
	void notifyEndResize(MyGUI::WidgetPtr _sender);
	// items panel
	void syncItems(bool _apply, bool _add = false, Ogre::String _value = "");
	void notifyAddItem(MyGUI::WidgetPtr _sender = 0);
	void notifyDeleteItem(MyGUI::WidgetPtr _sender);
	void notifySelectSheet(MyGUI::WidgetPtr _sender);
	void notifyUpdateItem(MyGUI::WidgetPtr _widget);
	void notifySelectItem(MyGUI::WidgetPtr _widget, size_t _position);
	// UserData panel
	void notifyAddUserData(MyGUI::WidgetPtr _sender = 0);
	void notifyDeleteUserData(MyGUI::WidgetPtr _sender);
	void notifyUpdateUserData(MyGUI::WidgetPtr _widget);
	void notifySelectUserDataItem(MyGUI::WidgetPtr _widget, size_t _index);

	// конвертирует из анси строки в вайд строку
	Ogre::DisplayString anci_to_utf16(const std::string & _source);

	// конвертирует из вайд в анси
	std::string utf16_to_anci(const Ogre::DisplayString & _source);


	MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести
	MyGUI::IntCoord convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // это можно в методы гуи занести

	MyGUI::ComboBoxPtr allWidgetsCombo;

	// info for new widget
	int x1, y1, x2, y2;
	std::string current_widget_type;
	std::string current_widget_skin;
	MyGUI::WidgetPtr current_widget;
	MyGUI::WindowPtr current_widget_rectangle;
	// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
	int creating_status;
	// drop select after skin change
	bool recreate;
	// widget was moved by keyboard arrows
	bool arrow_move;

	// current settings
	int grid_step;

	// interface settings
		// widgets panel
	int widgetsButtonWidth;
	int widgetsButtonHeight;
	int widgetsButtonsInOneLine;
		// properties panel
	//int 

	bool want_quit;
	// last loaded/saved file name
	Ogre::String fileName;

	// properties wndow
	MyGUI::VectorWidgetPtr propertiesText;
	MyGUI::VectorWidgetPtr propertiesElement;
	MyGUI::VectorWidgetPtr panelMinimizeButtons;
	MyGUI::VectorWidgetPtr panels;

	bool testMode;
	MyGUI::VectorWidgetPtr interfaceWidgets;
	MyGUI::xml::xmlDocument * testLayout;

	EditorWidgets * ew;
	WidgetTypes * wt;
	UndoManager * um;

	MyGUI::Gui * mGUI;
	MyGUI::LanguageManager * langManager;
	inline const Ogre::UTFString localise(const Ogre::UTFString & _str) {
		return langManager->getTag(_str);
	};

	MyGUI::MenuBarPtr bar;
	MyGUI::PopupMenuPtr mPopupMenuFile;
	MyGUI::PopupMenuPtr mPopupMenuWidgets;
	std::vector<MyGUI::PopupMenu::ItemInfo> widgetMenus;
};
