#ifndef __MAIN_PANE_CONTROL_H__
#define __MAIN_PANE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "MainMenuControl.h"
#include "ToolsControl.h"
#include "WorkspaceControl.h"

namespace tools
{
	class MainPaneControl :
		public wraps::BaseLayout
	{
	public:
		MainPaneControl();
		virtual ~MainPaneControl();

		void setVisible(bool _value);

	private:
		MainMenuControl* mMainMenuControl;
		ToolsControl* mToolsControl;
		WorkspaceControl* mWorkspaceControl;
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
