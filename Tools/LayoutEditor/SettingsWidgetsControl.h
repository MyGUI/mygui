/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _62e284bf_ce42_4aeb_a66f_29294b05ea30_
#define _62e284bf_ce42_4aeb_a66f_29294b05ea30_

#include "Control.h"

namespace tools
{

	class SettingsWidgetsControl :
		public Control
	{
	public:
		SettingsWidgetsControl();
		~SettingsWidgetsControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName) override;
		void OnCommand(const std::string& _command) override;

	private:
		void loadSettings();
		void saveSettings();

		void notifyToggleCheck(MyGUI::Widget* _sender);

	private:
		MyGUI::Button* mCheckShowName;
		MyGUI::Button* mCheckShowType;
		MyGUI::Button* mCheckShowSkin;
	};

}

#endif
