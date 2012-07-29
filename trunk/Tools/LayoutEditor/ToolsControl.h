#ifndef _6fb10723_0bea_4e88_bf83_b4f8da4796a0_
#define _6fb10723_0bea_4e88_bf83_b4f8da4796a0_

#include "BaseLayout/BaseLayout.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"
#include "ProjectControl.h"

namespace tools
{

	class ToolsControl :
		public wraps::BaseLayout
	{
	public:
		ToolsControl(MyGUI::Widget* _parent = nullptr);
		virtual ~ToolsControl();

	private:
		PropertiesPanelView* mPropertiesPanelView;
		WidgetsWindow* mWidgetsWindow;
		ProjectControl* mProjectControl;
	};

}

#endif
