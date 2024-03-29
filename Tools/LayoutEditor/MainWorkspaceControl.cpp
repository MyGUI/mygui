#include "Precompiled.h"
#include "MainWorkspaceControl.h"

namespace tools
{

	MainWorkspaceControl::MainWorkspaceControl(MyGUI::Widget* _parent) :
		SeparatorPartControl("MainWorkspaceControl.layout", _parent)
	{
		assignBase(mToolsControl, "ToolsControl");
		assignBase(mWorkspaceControl, "WorkspaceControl");
	}

}
