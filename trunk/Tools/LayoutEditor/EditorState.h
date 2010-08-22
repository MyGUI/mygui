#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "BaseManager.h"
#include "PropertiesPanelView.h"
#include "SettingsWindow.h"
#include "WidgetsWindow.h"
#include "OpenSaveFileDialog.h"
#include "MainMenuControl.h"


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
	void notifyConfirmLoadMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

	void notifyClearMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
	void clear(bool _clearName = true);
	void notifyConfirmQuitMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

	void notifyOpenSaveEndDialog(tools::Dialog* _dialog, bool _result);
	void notifySettingsWindowEndDialog(tools::Dialog* _dialog, bool _result);

	void notifyRecreate();

	void clearWidgetWindow();

	void setModeSaveLoadDialog(bool _save, const MyGUI::UString& _filename);

	void notifyFrameStarted(float _time);

	int toGrid(int _x);

	void commandLoad(const MyGUI::UString& _commandName);
	void commandSave(const MyGUI::UString& _commandName);
	void commandSaveAs(const MyGUI::UString& _commandName);
	void commandClear(const MyGUI::UString& _commandName);
	void commandTest(const MyGUI::UString& _commandName);
	void commandQuit(const MyGUI::UString& _commandName);
	void commandSettings(const MyGUI::UString& _commandName);
	void commandCodeGenerator(const MyGUI::UString& _commandName);
	void commandRecentFiles(const MyGUI::UString& _commandName);

private:
	// last click for depth selecting
	int mLastClickX;
	int mLastClickY;
	int mSelectDepth;

	// drop select after skin change
	bool mRecreate;

	// last loaded/saved file name
	MyGUI::UString mFileName;

	bool mTestMode;
	MyGUI::VectorWidgetPtr mInterfaceWidgets;
	MyGUI::xml::Document * mTestLayout;

	PropertiesPanelView * mPropertiesPanelView;
	SettingsWindow * mSettingsWindow;
	WidgetsWindow * mWidgetsWindow;
	CodeGenerator * mCodeGenerator;
	tools::OpenSaveFileDialog* mOpenSaveFileDialog;


	EditorWidgets * mEditorWidgets;
	WidgetTypes * mWidgetTypes;
	UndoManager * mUndoManager;
	GroupMessage * mGroupMessage;

	std::vector<std::wstring> mParams;
	std::string mLocale;

	tools::MainMenuControl* mMainMenuControl;
};
#endif // __EDITOR_STATE_H__
