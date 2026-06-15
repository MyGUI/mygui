/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _a5f70053_8679_4663_81f8_928be16e5873_
#define _a5f70053_8679_4663_81f8_928be16e5873_

#include "Control.h"
#include "OpenSaveFileDialog.h"

namespace tools
{

	class SettingsResourcePathsControl : public Control, public sigslot::has_slots<>
	{
	public:
		~SettingsResourcePathsControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
		void OnCommand(std::string_view _command) override;

	private:
		void loadSettings();
		void saveSettings();

		void notifyClickAdd(MyGUI::Widget* _sender);
		void notifyClickDelete(MyGUI::Widget* _sender);

		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourcePathAdd{nullptr};
		MyGUI::Button* mResourcePathDelete{nullptr};
		MyGUI::ListBox* mResourcePaths{nullptr};

		OpenSaveFileDialog* mOpenSaveFileDialog{nullptr};
	};

}

#endif
