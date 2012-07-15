/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _e5a988fe_bba2_480f_a287_d5c967f58266_
#define _e5a988fe_bba2_480f_a287_d5c967f58266_

#include "BaseManager.h"
#include "StateController.h"
#include "EditorState.h"
#include "TestState.h"

namespace tools
{

	class Application :
		public base::BaseManager,
		public MyGUI::Singleton<Application>,
		public StateController
	{
	public:
		Application();
		virtual ~Application();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);
		virtual void prepare();

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams();

		virtual void resumeState();

	protected:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

		void command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result);
		void command_FocusVisible(const MyGUI::UString& _commandName, bool& _result);
		void command_ScreenShot(const MyGUI::UString& _commandName, bool& _result);
		void command_QuitApp(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result);

	private:
		virtual void setupResources();

		void setWindowMaximized(bool _value);
		bool getWindowMaximized();

		void setWindowCoord(const MyGUI::IntCoord& _value);
		MyGUI::IntCoord getWindowCoord();

		void saveSettings();

	private:
		EditorState* mEditorState;

		std::string mLocale;
		VectorWString mParams;
	};

}

#endif
