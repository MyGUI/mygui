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

	class SettingsResourcesControl : public Control, public sigslot::has_slots<>
	{
	public:
		~SettingsResourcesControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
		void OnCommand(std::string_view _command) override;

	private:
		void loadSettings();
		void saveSettings();

		void notifyClickAdd(MyGUI::Widget* _sender);
		void notifyClickDelete(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);

	private:
		MyGUI::Button* mResourceAdd{nullptr};
		MyGUI::Button* mResourceDelete{nullptr};
		MyGUI::ListBox* mResources{nullptr};

		TextFieldControl* mTextFieldControl{nullptr};
	};

}

#endif
