#ifndef __MAIN_WORKSPACE_CONTROL_H__
#define __MAIN_WORKSPACE_CONTROL_H__

#include "SeparatorPartControl.h"
#include "ToolsControl.h"
#include "WorkspaceControl.h"

namespace tools
{

	class MainWorkspaceControl :
		public SeparatorPartControl
	{
	public:
		MainWorkspaceControl(MyGUI::Widget* _parent);
		virtual ~MainWorkspaceControl();

	private:
		ToolsControl* mToolsControl;
		WorkspaceControl* mWorkspaceControl;
	};

} // namespace tools

#endif // __MAIN_WORKSPACE_CONTROL_H__
