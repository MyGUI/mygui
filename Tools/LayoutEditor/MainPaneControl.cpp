#include "Precompiled.h"
#include "MainPaneControl.h"

namespace tools
{

	MainPaneControl::MainPaneControl() :
		wraps::BaseLayout("MainPane.layout"),
		mMainMenuControl(nullptr),
		mMainWorkspaceControl(nullptr)
	{
		assignBase(mMainMenuControl, "MainMenuControl");
		assignBase(mMainWorkspaceControl, "MainWorkspaceControl");
	}

	MainPaneControl::~MainPaneControl()
	{
	}

	void MainPaneControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

}
