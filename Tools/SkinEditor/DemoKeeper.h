/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "MainPane.h"
#include "OpenSaveFileDialog.h"
#include "TestWindow.h"
#include "MessageBoxFadeControl.h"

namespace tools
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);
		virtual void prepare();

	protected:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

	private:
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyEndDialogTest(Dialog* _sender, bool _result);

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

		virtual void setupResources();
		void updateCaption();

		void notifyChanges(bool _changes);

	private:
		MainPane* mMainPane;
		bool mChanges;
		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;
		MyGUI::UString mDropFileName;

		OpenSaveFileDialog* mOpenSaveFileDialog;
		TestWindow* mTestWindow;
		MessageBoxFadeControl* mMessageBoxFadeControl;

		std::string mLocale;
		typedef std::vector<std::wstring> VectorWString;
		VectorWString mParams;
	};

} // namespace tools

#endif // __DEMO_KEEPER_H__
