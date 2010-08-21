#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "BaseManager.h"
#include "EditorToolTip.h"
#include "PropertiesPanelView.h"
#include "SettingsWindow.h"
#include "WidgetsWindow.h"
#include "MetaSolutionWindow.h"
#include "OpenSaveFileDialog.h"

class EditorWidgets;
class WidgetTypes;
class UndoManager;
struct WidgetContainer;
class GroupMessage;
class CodeGenerator;

class EditorState : public base::BaseManager
{
public:
	EditorState();
	virtual ~EditorState();

	virtual void createScene();
	virtual void destroyScene();
	virtual void setupResources();

	virtual void injectMouseMove(int _absx, int _absy, int _absz);
	virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
	virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
	virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
	virtual void injectKeyRelease(MyGUI::KeyCode _key);

	virtual void prepare();
	virtual void onFileDrop(const std::wstring& _filename);
	virtual bool onWinodwClose(size_t _handle);

	// main panel
	void notifyLoad();
	bool notifySave();
	void notifySettings();
	void notifyTest();
	void notifyEndTest();
	void notifyClear();
	void notifyQuit();

	template <bool Save>
	void saveOrLoadLayoutEvent(const MyGUI::UString& _file) { saveOrLoadLayout(Save, false, _file); }

	bool saveOrLoadLayout(bool Save, bool Silent, const MyGUI::UString& _file);
	void loadFile(const std::wstring& _file);

private:
	void loadSettings(const MyGUI::UString& _fileName, bool _internal);
	void saveSettings(const MyGUI::UString& _fileName);

	void notifyConfirmLoadMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

	void notifyClearMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
	void clear(bool _clearName = true);
	void notifyConfirmQuitMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

	// menu mBar
	void notifyWidgetsSelect(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);
	void createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin);
	void notifyWidgetsUpdate();
	void notifyOpenSaveEndDialog(bool _result);

	void notifySelectWidget(MyGUI::Widget* _sender);

	void notifyRecreate();

	bool isNeedSolutionLoad(MyGUI::xml::ElementEnumerator _field);
	bool isMetaSolution(const MyGUI::UString& _fileName);

	void clearWidgetWindow();

	void setModeSaveLoadDialog(bool _save, const MyGUI::UString& _filename);

	std::string getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin);

	void createMainMenu();
	void notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);
	void notifyFrameStarted(float _time);

	int toGrid(int _x);

	// tooltips
	void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo & _info);

private:
	// recent files
	static const size_t MAX_RECENT_FILES = 8;
	std::vector<MyGUI::UString> mRecentFiles;

	std::vector<MyGUI::UString> mAdditionalPaths;

	// last click for depth selecting
	int mLastClickX;
	int mLastClickY;
	int mSelectDepth;

	MyGUI::Widget* mCurrentWidget;
	// drop select after skin change
	bool mRecreate;

	bool mModeSaveDialog;

	// last loaded/saved file name
	MyGUI::UString mFileName;

	bool mTestMode;
	MyGUI::VectorWidgetPtr mInterfaceWidgets;
	MyGUI::xml::Document * mTestLayout;

	EditorToolTip * mToolTip;

	PropertiesPanelView * mPropertiesPanelView;
	SettingsWindow * mSettingsWindow;
	WidgetsWindow * mWidgetsWindow;
	MetaSolutionWindow * mMetaSolutionWindow;
	CodeGenerator * mCodeGenerator;
	common::OpenSaveFileDialog* mOpenSaveFileDialog;


	EditorWidgets * mEditorWidgets;
	WidgetTypes * mWidgetTypes;
	UndoManager * mUndoManager;
	GroupMessage * mGroupMessage;

	MyGUI::MenuBar* mBar;
	MyGUI::MenuCtrl* mPopupMenuFile;
	MyGUI::MenuCtrl* mPopupMenuWidgets;
	std::vector<MyGUI::PopupMenu::ItemInfo> mWidgetMenus;

	std::vector<std::wstring> mParams;
	std::string mLocale;
};
#endif // __EDITOR_STATE_H__
