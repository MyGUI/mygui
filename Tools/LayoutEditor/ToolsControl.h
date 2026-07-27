#ifndef _6fb10723_0bea_4e88_bf83_b4f8da4796a0_
#define _6fb10723_0bea_4e88_bf83_b4f8da4796a0_

#include "Control.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"
#include "ProjectControl.h"

namespace tools
{

	class ToolsControl : public Control
	{
	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		PropertiesPanelView* mPropertiesPanelView{nullptr};
		WidgetsWindow* mWidgetsWindow{nullptr};
		ProjectControl* mProjectControl{nullptr};
	};

}

#endif
