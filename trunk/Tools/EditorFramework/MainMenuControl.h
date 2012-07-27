/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _77263d40_3ebd_4f36_abc7_64ce6f294aef_
#define _77263d40_3ebd_4f36_abc7_64ce6f294aef_

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
