#ifndef _fb654ad0_db5f_4387_ba52_53c0a5f3b023_
#define _fb654ad0_db5f_4387_ba52_53c0a5f3b023_

#include "SettingsWindow.h"
#include "OpenSaveFileDialog.h"
#include "CodeGenerator.h"
#include "StateController.h"
#include "MainPaneControl.h"
#include "sigslot.h"

namespace tools
{

	class EditorState :
		public StateController,
		public sigslot::has_slots<>
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
		void command_Quit(const MyGUI::UString& _commandName, bool& _result);
		void command_Settings(const MyGUI::UString& _commandName, bool& _result);
		void command_CodeGenerator(const MyGUI::UString& _commandName, bool& _result);
		void command_OpenRecentFile(const MyGUI::UString& _commandName, bool& _result);
		void command_FileDrop(const MyGUI::UString& _commandName, bool& _result);
		void command_SaveItemAs(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateItemName(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateResources(const MyGUI::UString& _commandName, bool& _result);

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

		MainPaneControl* mMainPaneControl;

		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;
		MyGUI::UString mDropFileName;
	};

}

#endif
