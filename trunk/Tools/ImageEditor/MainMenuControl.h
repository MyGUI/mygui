/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include "Control.h"

namespace tools
{

	class MainMenuControl :
		public Control
	{
	public:
		MainMenuControl();
		virtual ~MainMenuControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);

		void updateRecentFilesMenu();

	private:
		MyGUI::MenuBar* mMainMenu;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
