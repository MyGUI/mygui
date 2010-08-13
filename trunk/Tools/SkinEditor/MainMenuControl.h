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

	/*enum MenuCommand
	{
		MenuCommandClear,
		MenuCommandLoad
	};*/

	class MainMenuControl :
		public wraps::BaseLayout
	{
	public:
		MainMenuControl(MyGUI::Widget* _parent);
		virtual ~MainMenuControl();

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item);
		//void notifyMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void commandLoad();
		void commandSave();
		void commandSaveAs();
		void commandClear();
		void commandQuit();

		//void showMessageBox(MenuCommand _command, const MyGUI::UString& _text);

		//void save(const MyGUI::UString& _fileName);
		void clear();
		void showLoadWindow();
		void save();

	private:
		MyGUI::MenuBar* mMainMenu;
		MyGUI::UString mFileName;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
