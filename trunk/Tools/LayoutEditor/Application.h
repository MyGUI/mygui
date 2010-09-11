#ifndef __APPLICATION_H__
#define __APPLICATION_H__

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
		virtual void setupResources();

		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

		virtual void prepare();
		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams();

		void setCaption(const MyGUI::UString& _value);

		virtual void resumeState();

	private:
		void commandQuitApp(const MyGUI::UString& _commandName);
		void commandStatisticInfo(const MyGUI::UString& _commandName);
		void commandFocusVisible(const MyGUI::UString& _commandName);

	private:
		VectorWString mParams;
		std::string mLocale;

		EditorState* mEditorState;
		TestState* mTestState;
	};

} // namespace tools

#endif // __APPLICATION_H__
