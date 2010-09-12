/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __SETTINGS_WIDGETS_CONTROL_H__
#define __SETTINGS_WIDGETS_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class SettingsWidgetsControl :
		public wraps::BaseLayout
	{
	public:
		SettingsWidgetsControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SettingsWidgetsControl();

		void loadSettings();
		void saveSettings();

	private:
		void notifyToggleCheck(MyGUI::Widget* _sender);

	private:
		MyGUI::Button* mCheckShowName;
		MyGUI::Button* mCheckShowType;
		MyGUI::Button* mCheckShowSkin;
	};

} // namespace tools

#endif // __SETTINGS_WIDGETS_CONTROL_H__
