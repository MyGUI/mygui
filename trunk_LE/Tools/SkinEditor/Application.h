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

		void setCaption(const MyGUI::UString& _value);

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams();

		virtual void resumeState();

	protected:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

		void commandStatisticInfo(const MyGUI::UString& _commandName, bool& _result);
		void commandFocusVisible(const MyGUI::UString& _commandName, bool& _result);
		void commandQuitApp(const MyGUI::UString& _commandName, bool& _result);

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
