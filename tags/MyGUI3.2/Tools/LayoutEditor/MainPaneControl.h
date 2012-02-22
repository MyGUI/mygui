#ifndef __MAIN_PANE_CONTROL_H__
#define __MAIN_PANE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "MainMenuControl.h"
#include "MainWorkspaceControl.h"

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
		MainWorkspaceControl* mMainWorkspaceControl;
	};

} // namespace tools

#endif // __MAIN_PANE_CONTROL_H__
