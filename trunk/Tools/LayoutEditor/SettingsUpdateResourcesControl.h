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

	class SettingsUpdateResourcesControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		SettingsUpdateResourcesControl();
		virtual ~SettingsUpdateResourcesControl();

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
