#include "Precompiled.h"
#include "ToolsControl.h"

namespace tools
{

	void ToolsControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, "ToolsControl.layout");

		MyGUI::Widget* propertiesControl = nullptr;
		assignWidget(propertiesControl, "PropertiesControl");
		mPropertiesPanelView = new PropertiesPanelView();
		mPropertiesPanelView->Initialise(this, propertiesControl, {});

		MyGUI::Widget* widgetsControl = nullptr;
		assignWidget(widgetsControl, "WidgetsControl");
		mWidgetsWindow = new WidgetsWindow();
		mWidgetsWindow->Initialise(this, widgetsControl, {});

		MyGUI::Widget* projectControl = nullptr;
		assignWidget(projectControl, "ProjectControl");
		mProjectControl = new ProjectControl();
		mProjectControl->Initialise(this, projectControl, {});
	}

}
