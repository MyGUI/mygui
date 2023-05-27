#include "Precompiled.h"
#include "ToolsControl.h"

namespace tools
{

	ToolsControl::ToolsControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("ToolsControl.layout", _parent)
	{
		assignBase(mPropertiesPanelView, "PropertiesControl");
		assignBase(mWidgetsWindow, "WidgetsControl");
		assignBase(mProjectControl, "ProjectControl");
	}

}
