#ifndef _d79dc96f_0f50_4950_b515_9a5deafd511b_
#define _d79dc96f_0f50_4950_b515_9a5deafd511b_

#include "Base/BaseDemoManager.h"
#include "sigslot.h"
#include "Control.h"

namespace tools
{

	class Application : public base::BaseDemoManager, public sigslot::has_slots<>
	{
		MYGUI_SINGLETON_DECLARATION(Application);

	public:
		Application();
		~Application() override;

		void createScene() override;
		void destroyScene() override;
		void setupResources() override;

		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;
		void injectKeyRelease(MyGUI::KeyCode _key) override;

		void prepare() override;
		void onFileDrop(const std::wstring& _filename) override;
		bool onWindowClose(size_t _handle) override;

		using VectorWString = std::vector<std::wstring>;
		const VectorWString& getParams();

	private:
		void command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result);
		void command_FocusVisible(const MyGUI::UString& _commandName, bool& _result);
		void command_ScreenShot(const MyGUI::UString& _commandName, bool& _result);
		void command_QuitApp(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result);

		void saveSettings();

		void LoadStates();
		void LoadGuiSettings();

		void CreateControls();
		void DestroyControls();

	private:
		VectorWString mParams;
		std::string mLocale;
		using VectorControl = std::vector<Control*>;
		VectorControl mControls;
	};

}

#endif
