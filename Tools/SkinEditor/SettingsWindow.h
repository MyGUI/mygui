/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"
#include "Dialog.h"
#include "OpenSaveFileDialog.h"

namespace tools
{
	class SettingsWindow :
		public Dialog
	{
	public:
		SettingsWindow();
		virtual ~SettingsWindow();

		void loadSettings();
		void saveSettings();

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyOkSettings(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

		void notifyClickResourcePathAdd(MyGUI::Widget* _sender);
		void notifyClickResourcePathDelete(MyGUI::Widget* _sender);

		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mButtonOkSettings;
		MyGUI::Button* mButtonCancel;
		MyGUI::Button* mResourcePathAdd;
		MyGUI::Button* mResourcePathDelete;
		MyGUI::List* mResourcePaths;

		OpenSaveFileDialog* mOpenSaveFileDialog;
	};

} // namespace tools

#endif // __SETTINGS_WINDOW_H__
