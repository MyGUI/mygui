#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "SettingsWindow.h"
#include "OpenSaveFileDialog.h"
#include "BackgroundControl.h"
#include "CodeGenerator.h"
#include "MessageBoxFadeControl.h"
#include "StateController.h"
#include "MainPaneControl.h"

namespace tools
{
	class EditorState :
		public StateController
	{
	public:
		EditorState();
		virtual ~EditorState();

		virtual void initState();
		virtual void cleanupState();

		virtual void pauseState();
		virtual void resumeState();

	private:
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void clear();
		bool save();
		void load();
		void loadDropFile();

		void command_Load(const MyGUI::UString& _commandName, bool& _result);
		void command_Save(const MyGUI::UString& _commandName, bool& _result);
		void command_SaveAs(const MyGUI::UString& _commandName, bool& _result);
		void command_Clear(const MyGUI::UString& _commandName, bool& _result);
		void command_Test(const MyGUI::UString& _commandName, bool& _result);
		void command_Quit(const MyGUI::UString& _commandName, bool& _result);
		void command_Settings(const MyGUI::UString& _commandName, bool& _result);
		void command_CodeGenerator(const MyGUI::UString& _commandName, bool& _result);
		void command_OpenRecentFile(const MyGUI::UString& _commandName, bool& _result);
		void command_FileDrop(const MyGUI::UString& _commandName, bool& _result);
		void command_SaveItemAs(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateItemName(const MyGUI::UString& _commandName, bool& _result);

		void updateCaption();
		bool checkCommand();

		void notifySettingsWindowEndDialog(Dialog* _dialog, bool _result);
		void notifyEndDialogCodeGenerator(Dialog* _dialog, bool _result);
		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);
		void notifyChanges(bool _changes);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void showLoadWindow();
		void showSaveAsWindow();

		void setFileName(const MyGUI::UString& _fileName);
		MyGUI::UString convertProjectName(const MyGUI::UString& _fileName);

		bool isProjectFile(const MyGUI::UString& _fileName);

	private:
		SettingsWindow* mSettingsWindow;
		CodeGenerator* mCodeGenerator;
		OpenSaveFileDialog* mOpenSaveFileDialog;

		MessageBoxFadeControl* mMessageBoxFadeControl;
		BackgroundControl* mBackgroundControl;
		MainPaneControl* mMainPaneControl;

		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;
		MyGUI::UString mDropFileName;
	};

} // namespace tools

#endif // __EDITOR_STATE_H__
