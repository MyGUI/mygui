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
#include "PropertyField.h"

namespace tools
{
	class PanelProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType);

	private:
		size_t AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, int& y, MyGUI::Widget* _currentWidget);
		void destroyPropertyFields();

	private:
		int mPropertyItemHeight;
		typedef std::vector<PropertyField> VectorPropertyField;
		VectorPropertyField mFields;
	};

} // namespace tools

#endif // __PANEL_PROPERTIES_H__
