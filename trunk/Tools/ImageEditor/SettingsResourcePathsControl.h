/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __SETTINGS_RESOURCE_PATHS_CONTROL_H__
#define __SETTINGS_RESOURCE_PATHS_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Tools/OpenSaveFileDialog.h"

namespace tools
{
	class SettingsResourcePathsControl :
		public wraps::BaseLayout
	{
	public:
		SettingsResourcePathsControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SettingsResourcePathsControl();

		void loadSettings();
		void saveSettings();

	private:
		void notifyClickResourcePathAdd(MyGUI::Widget* _sender);
		void notifyClickResourcePathDelete(MyGUI::Widget* _sender);

		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourcePathAdd;
		MyGUI::Button* mResourcePathDelete;
		MyGUI::ListBox* mResourcePaths;

		OpenSaveFileDialog* mOpenSaveFileDialog;
	};

} // namespace tools

#endif // __SETTINGS_RESOURCE_PATHS_CONTROL_H__
