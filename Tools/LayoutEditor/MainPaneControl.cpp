#include "Precompiled.h"
#include "MainPaneControl.h"

namespace tools
{

	MainPaneControl::MainPaneControl() :
		wraps::BaseLayout("MainPane.layout")
	{
		assignBase(mMainMenuControl, "MainMenuControl");
		assignBase(mMainWorkspaceControl, "MainWorkspaceControl");
	}

	void MainPaneControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

}
