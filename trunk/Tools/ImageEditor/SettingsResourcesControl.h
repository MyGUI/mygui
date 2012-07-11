/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __SETTINGS_RESOURCES_CONTROL_H__
#define __SETTINGS_RESOURCES_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "TextFieldControl.h"

namespace tools
{
	class SettingsResourcesControl :
		public wraps::BaseLayout
	{
	public:
		SettingsResourcesControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SettingsResourcesControl();

		void loadSettings();
		void saveSettings();

	private:
		void notifyClickResourcePathAdd(MyGUI::Widget* _sender);
		void notifyClickResourcePathDelete(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourceAdd;
		MyGUI::Button* mResourceDelete;
		MyGUI::ListBox* mResources;

		TextFieldControl* mTextFieldControl;
	};

} // namespace tools

#endif // __SETTINGS_RESOURCES_CONTROL_H__
