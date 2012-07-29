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
		virtual ~SettingsWidgetsControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
		virtual void OnCommand(const std::string& _command);

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
