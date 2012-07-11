/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class MainMenuControl :
		public wraps::BaseLayout
	{
	public:
		MainMenuControl(MyGUI::Widget* _parent);
		virtual ~MainMenuControl();

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);

		void updateRecentFilesMenu();

	private:
		MyGUI::MenuBar* mMainMenu;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
