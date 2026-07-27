#include "Precompiled.h"
#include "MainWorkspaceControl.h"

namespace tools
{

	void MainWorkspaceControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		SeparatorPartControl::OnInitialise(_parent, _place, "MainWorkspaceControl.layout");

		MyGUI::Widget* toolsControl = nullptr;
		assignWidget(toolsControl, "ToolsControl");
		mToolsControl = new ToolsControl();
		mToolsControl->Initialise(this, toolsControl, {});

		MyGUI::Widget* workspaceControl = nullptr;
		assignWidget(workspaceControl, "WorkspaceControl");
		mWorkspaceControl = new WorkspaceControl();
		mWorkspaceControl->Initialise(this, workspaceControl, {});
	}

}
