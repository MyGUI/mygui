/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _dcb9de08_d772_483e_b30d_ac0d3e9fdece_
#define _dcb9de08_d772_483e_b30d_ac0d3e9fdece_

#include "Control.h"
#include "TextFieldControl.h"

namespace tools
{

	class SettingsUpdateResourcesControl : public Control, public sigslot::has_slots<>
	{
	public:
		~SettingsUpdateResourcesControl() override;

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
