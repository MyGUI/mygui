/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _e5a988fe_bba2_480f_a287_d5c967f58266_
#define _e5a988fe_bba2_480f_a287_d5c967f58266_

#include "BaseManager.h"
#include "sigslot.h"
#include "Control.h"

namespace tools
{

	class Application : public base::BaseManager, public sigslot::has_slots<>
	{
		MYGUI_SINGLETON_DECLARATION(Application);

	public:
		Application();
		~Application() override;

		void createScene() override;
		void destroyScene() override;

		void onFileDrop(const std::wstring& _filename) override;
		bool onWindowClose(size_t _handle) override;
		void prepare() override;

		using VectorWString = std::vector<std::wstring>;
		const VectorWString& getParams();

	protected:
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;

		void command_ScreenShot(const MyGUI::UString& _commandName, bool& _result);
		void command_QuitApp(const MyGUI::UString& _commandName, bool& _result);
		void command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result);

	private:
		void setupResources() override;

		void saveSettings();

		void LoadStates();
		void LoadGuiSettings();

		void CreateControls();
		void DestroyControls();

	private:
		std::string mLocale;
		VectorWString mParams;
		using VectorControl = std::vector<Control*>;
		VectorControl mControls;
	};

}

#endif
