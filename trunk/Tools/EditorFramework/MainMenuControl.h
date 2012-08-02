/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _55ee5c79_b3ce_426d_9486_2e3ac51002a8_
#define _55ee5c79_b3ce_426d_9486_2e3ac51002a8_

#include "Control.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL MainMenuControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		MainMenuControl();
		virtual ~MainMenuControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);
		void command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result);

		void updateRecentFilesMenu();

	private:
		MyGUI::MenuBar* mMainMenu;
		MyGUI::MenuItem* mScaleMenu;
	};

}

#endif
