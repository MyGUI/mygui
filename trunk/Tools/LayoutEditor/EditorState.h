#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "BaseManager.h"
#include "PropertiesPanelView.h"
#include "SettingsWindow.h"
#include "WidgetsWindow.h"
#include "OpenSaveFileDialog.h"
#include "MainMenuControl.h"
#include "CodeGenerator.h"
#include "MessageBoxFadeControl.h"

namespace tools
{
	class EditorState :
		public base::BaseManager
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

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams() { return mParams; }

		void setCaption(const MyGUI::UString& _value);

	private:
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void clear();
		bool save();
		void load();
		void loadDropFile();

		void notifyRecreate();
		void notifyFrameStarted(float _time);

		int toGrid(int _x);

		void commandLoad(const MyGUI::UString& _commandName);
		void commandSave(const MyGUI::UString& _commandName);
		void commandSaveAs(const MyGUI::UString& _commandName);
		void commandClear(const MyGUI::UString& _commandName);
		//void commandTest(const MyGUI::UString& _commandName);
		void commandQuit(const MyGUI::UString& _commandName);
		void commandQuitApp(const MyGUI::UString& _commandName);
		void commandSettings(const MyGUI::UString& _commandName);
		void commandCodeGenerator(const MyGUI::UString& _commandName);
		void commandRecentFiles(const MyGUI::UString& _commandName);
		void commandStatisticInfo(const MyGUI::UString& _commandName);
		void commandFocusVisible(const MyGUI::UString& _commandName);
		void commandFileDrop(const MyGUI::UString& _commandName);

		void updateCaption();
		bool checkCommand();

		void notifySettingsWindowEndDialog(Dialog* _dialog, bool _result);
		void notifyEndDialogCodeGenerator(Dialog* _dialog, bool _result);
		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);
		void notifyChanges(bool _changes);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void showLoadWindow();
		void showSaveAsWindow();

	private:
		// last click for depth selecting
		int mLastClickX;
		int mLastClickY;
		int mSelectDepth;

		// drop select after skin change
		bool mRecreate;

		//bool mTestMode;
		MyGUI::VectorWidgetPtr mInterfaceWidgets;
		MyGUI::xml::Document * mTestLayout;

		PropertiesPanelView * mPropertiesPanelView;
		SettingsWindow * mSettingsWindow;
		WidgetsWindow * mWidgetsWindow;
		CodeGenerator * mCodeGenerator;
		OpenSaveFileDialog* mOpenSaveFileDialog;

		VectorWString mParams;
		std::string mLocale;

		MainMenuControl* mMainMenuControl;
		MessageBoxFadeControl* mMessageBoxFadeControl;

		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;
		MyGUI::UString mDropFileName;

		int mGridStep;
	};

} // namespace tools

#endif // __EDITOR_STATE_H__
