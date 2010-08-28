/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "StateController.h"
#include "MainPane.h"
#include "OpenSaveFileDialog.h"
#include "TestWindow.h"
#include "MessageBoxFadeControl.h"

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

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyEndDialogTest(Dialog* _sender, bool _result);
		void notifyChanges(bool _changes);

		void commandLoad(const MyGUI::UString& _commandName);
		void commandSave(const MyGUI::UString& _commandName);
		void commandSaveAs(const MyGUI::UString& _commandName);
		void commandClear(const MyGUI::UString& _commandName);
		void commandTest(const MyGUI::UString& _commandName);
		void commandQuit(const MyGUI::UString& _commandName);
		void commandFileDrop(const MyGUI::UString& _commandName);

		void clear();
		void save();
		void load();
		void loadDropFile();

		void showLoadWindow();
		void showSaveAsWindow();

		void updateCaption();

	private:
		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;
		MyGUI::UString mDropFileName;

		MainPane* mMainPane;
		OpenSaveFileDialog* mOpenSaveFileDialog;
		TestWindow* mTestWindow;
		MessageBoxFadeControl* mMessageBoxFadeControl;
	};

} // namespace tools

#endif // __EDITOR_STATE_H__
