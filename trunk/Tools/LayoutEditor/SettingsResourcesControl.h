/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _f3143a06_b26a_4728_bc55_25891e77f89c_
#define _f3143a06_b26a_4728_bc55_25891e77f89c_

#include "Control.h"
#include "TextFieldControl.h"

namespace tools
{

	class SettingsResourcesControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		SettingsResourcesControl();
		virtual ~SettingsResourcesControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
		virtual void OnCommand(const std::string& _command);

	private:
		void loadSettings();
		void saveSettings();

		void notifyClickAdd(MyGUI::Widget* _sender);
		void notifyClickDelete(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourceAdd;
		MyGUI::Button* mResourceDelete;
		MyGUI::ListBox* mResources;

		TextFieldControl* mTextFieldControl;
	};

}

#endif
