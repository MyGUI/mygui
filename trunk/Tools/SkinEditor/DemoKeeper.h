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

namespace tools
{

	class DemoKeeper :
		public base::BaseManager,
		public MyGUI::Singleton<DemoKeeper>
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);

		void setChanges(bool _value);

		void registerMessageBox(MyGUI::Message* _message);

	protected:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

	private:
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifMessageBoxResultRegister(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyEndDialogTest(Dialog* _sender, bool _result);

		void commandLoad(const MyGUI::UString & _commandName);
		void commandSave(const MyGUI::UString & _commandName);
		void commandSaveAs(const MyGUI::UString & _commandName);
		void commandClear(const MyGUI::UString & _commandName);
		void commandExport(const MyGUI::UString & _commandName);
		void commandTest(const MyGUI::UString & _commandName);
		void commandQuit(const MyGUI::UString & _commandName);

		void clear();
		void save();
		void load();
		void exportSkin(const MyGUI::UString& _fileName);
		void convertSkin(MyGUI::xml::Element* _from, MyGUI::xml::Element* _to);

		void showLoadWindow();
		void showSaveAsWindow();
		void showExportWindow();

		virtual void setupResources();
		void updateCaption();

	private:
		tools::MainPane* mMainPane;
		bool mChanges;
		MyGUI::UString mFileName;
		MyGUI::UString mDefaultFileName;

		OpenSaveFileDialog* mOpenSaveFileDialog;
		TestWindow* mTestWindow;

		MyGUI::Message* mMessageBox;
	};

} // namespace tools

#endif // __DEMO_KEEPER_H__
