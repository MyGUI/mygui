#ifndef __TOOLS_CONTROL_H__
#define __TOOLS_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"

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
	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
