/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "OpenSaveFileDialog.h"

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
		void notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);
		void notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

		void notifyEndDialog(bool _result);

		void commandLoad();
		void commandSave();
		void commandSaveAs();
		void commandClear();
		void commandQuit();

		void clear();
		void save();
		void load();

		void showLoadWindow();
		void showSaveAsWindow();

		void updateWidgetCaption();

	private:
		MyGUI::MenuBar* mMainMenu;
		MyGUI::UString mFileName;
		common::OpenSaveFileDialog* mOpenSaveFileDialog;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
