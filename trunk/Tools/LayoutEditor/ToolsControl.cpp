#include "Precompiled.h"
#include "ToolsControl.h"

namespace tools
{
	ToolsControl::ToolsControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("ToolsControl.layout", _parent),
		mPropertiesPanelView(nullptr),
		mWidgetsWindow(nullptr)
	{
		assignBase(mPropertiesPanelView, "PropertiesControl");
		assignBase(mWidgetsWindow, "WidgetsControl");
	}

	ToolsControl::~ToolsControl()
	{
	}

} // namespace tools
