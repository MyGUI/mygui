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
		void notifyMenuCtrlAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);

		void commandLoad();
		void commandSave();
		void commandSaveAs();
		void commandClear();
		void commandQuit();

		/*void hideMessageBox();
		void showMessageBox();*/

	private:
		MyGUI::MenuBar* mMainMenu;
		//MyGUI::Message* mMessageBox;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
