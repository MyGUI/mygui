/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

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

	private:
		EditorState* mEditorState;
		TestState* mTestState;

		std::string mLocale;
		VectorWString mParams;
	};

} // namespace tools

#endif // __DEMO_KEEPER_H__
