#include "Precompiled.h"
#include "MainPaneControl.h"

namespace tools
{

	void MainPaneControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, "MainPane.layout");

		MyGUI::Widget* mainMenuControl = nullptr;
		assignWidget(mainMenuControl, "MainMenuControl");
		mMainMenuControl = new MainMenuControlLE();
		mMainMenuControl->Initialise(this, mainMenuControl, {});

		MyGUI::Widget* mainWorkspaceControl = nullptr;
		assignWidget(mainWorkspaceControl, "MainWorkspaceControl");
		mMainWorkspaceControl = new MainWorkspaceControl();
		mMainWorkspaceControl->Initialise(this, mainWorkspaceControl, {});
	}

	void MainPaneControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

}
