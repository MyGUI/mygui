/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _45147871_9597_4bee_b5ea_e6a2c1dd53b2_
#define _45147871_9597_4bee_b5ea_e6a2c1dd53b2_

#include "Dialog.h"
#include "Control.h"

namespace tools
{

	class MYGUI_EXPORT_DLL SettingsWindow :
		public Dialog,
		public Control,
		public sigslot::has_slots<>
	{
	public:
		SettingsWindow();
		virtual ~SettingsWindow();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		bool checkCommand();
		void commandSettingsAccept(const MyGUI::UString& _commandName, bool& _result);
		void commandSettingsCancel(const MyGUI::UString& _commandName, bool& _result);

		void InitialiseListTab();
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);

	private:
		MyGUI::ListBox* mListSettings;
		MyGUI::TabControl* mTabSettings;
	};

}

#endif
