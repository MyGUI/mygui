#ifndef _d79dc96f_0f50_4950_b515_9a5deafd511b_
#define _d79dc96f_0f50_4950_b515_9a5deafd511b_

#include "Base/BaseDemoManager.h"
#include "sigslot.h"
#include "Control.h"

namespace tools
{

	class Application :
		public base::BaseDemoManager,
		public MyGUI::Singleton<Application>,
		public sigslot::has_slots<>
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

	private:
		void command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result);
		void command_FocusVisible(const MyGUI::UString& _commandName, bool& _result);
		void command_ScreenShot(const MyGUI::UString& _commandName, bool& _result);
		void command_QuitApp(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result);

		void setWindowMaximized(bool _value);
		bool getWindowMaximized();

		void setWindowCoord(const MyGUI::IntCoord& _value);
		MyGUI::IntCoord getWindowCoord();

		void saveSettings();

		void LoadStates();
		void LoadGuiSettings();

		void CreateControls();
		void DestroyControls();

	private:
		VectorWString mParams;
		std::string mLocale;
		typedef std::vector<Control*> VectorControl;
		VectorControl mControls;
	};

}

#endif
