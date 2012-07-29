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

	class SettingsResourcePathsControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		SettingsResourcePathsControl();
		virtual ~SettingsResourcePathsControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
		virtual void OnCommand(const std::string& _command);

	private:
		void loadSettings();
		void saveSettings();

		void notifyClickAdd(MyGUI::Widget* _sender);
		void notifyClickDelete(MyGUI::Widget* _sender);

		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourcePathAdd;
		MyGUI::Button* mResourcePathDelete;
		MyGUI::ListBox* mResourcePaths;

		OpenSaveFileDialog* mOpenSaveFileDialog;
	};

}

#endif
