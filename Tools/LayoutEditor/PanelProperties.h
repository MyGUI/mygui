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

		typedef MyGUI::delegates::CDelegate6<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int, PropertyField&> EventHandle_EventCreatePair;
		EventHandle_EventCreatePair eventCreatePair;

	private:
		size_t AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, int& y);
		void destroyPropertyFields();

	private:
		int mPropertyItemHeight;
		typedef std::vector<PropertyField> VectorPropertyField;
		VectorPropertyField mFields;
	};

} // namespace tools

#endif // __PANEL_PROPERTIES_H__
