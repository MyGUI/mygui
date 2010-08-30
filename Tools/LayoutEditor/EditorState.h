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

		void notifyRecreate();
		void notifyFrameStarted(float _time);

		void commandLoad(const MyGUI::UString& _commandName);
		void commandSave(const MyGUI::UString& _commandName);
		void commandSaveAs(const MyGUI::UString& _commandName);
		void commandClear(const MyGUI::UString& _commandName);
		void commandTest(const MyGUI::UString& _commandName);
		void commandQuit(const MyGUI::UString& _commandName);
		void commandSettings(const MyGUI::UString& _commandName);
		void commandCodeGenerator(const MyGUI::UString& _commandName);
		void commandRecentFiles(const MyGUI::UString& _commandName);
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

		void setFileName(const MyGUI::UString& _fileName);

	private:
		// drop select after skin change
		bool mRecreate;

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
