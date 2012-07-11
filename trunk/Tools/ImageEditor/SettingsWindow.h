/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"
#include "Tools/Dialog.h"
#include "SettingsResourcesControl.h"
#include "SettingsResourcePathsControl.h"
#include "SettingsGeneralControl.h"

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
		void notifyOk(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

	private:
		MyGUI::Button* mButtonOk;
		MyGUI::Button* mButtonCancel;

		SettingsGeneralControl* mSettingsGeneralControl;
		SettingsResourcesControl* mSettingsResourcesControl;
		SettingsResourcePathsControl* mSettingsResourcePathsControl;
	};

} // namespace tools

#endif // __SETTINGS_WINDOW_H__
