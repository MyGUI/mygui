#ifndef _6def8530_1f19_41bb_b4a1_b4924343a908_
#define _6def8530_1f19_41bb_b4a1_b4924343a908_

#include "SeparatorPartControl.h"
#include "ToolsControl.h"
#include "WorkspaceControl.h"

namespace tools
{

	class MainWorkspaceControl : public SeparatorPartControl
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		ToolsControl* mToolsControl{nullptr};
		WorkspaceControl* mWorkspaceControl{nullptr};
	};

}

#endif
