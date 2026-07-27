#ifndef _c094d4c6_6c07_414d_979a_f85dde3f4e7b_
#define _c094d4c6_6c07_414d_979a_f85dde3f4e7b_

#include "Control.h"
#include "MainMenuControl.h"
#include "MainWorkspaceControl.h"

namespace tools
{

	class MainPaneControl : public Control
	{
	public:
		void setVisible(bool _value);

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		MainMenuControlLE* mMainMenuControl{nullptr};
		MainWorkspaceControl* mMainWorkspaceControl{nullptr};
	};

}

#endif
