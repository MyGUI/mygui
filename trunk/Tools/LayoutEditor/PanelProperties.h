/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetTypes.h"
#include "WidgetContainer.h"

namespace tools
{
	class PanelProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _current_widget, WidgetStyle* _widgetType);

		typedef MyGUI::delegates::CDelegate5<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
		EventHandle_EventCreatePair eventCreatePair;

	private:
		size_t AddParametrs(WidgetStyle * widgetType, WidgetContainer * widgetContainer, int& y);

	private:
		int mPropertyItemHeight;
	};

} // namespace tools

#endif // __PANEL_PROPERTIES_H__
