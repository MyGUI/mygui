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
#include "IPropertyField.h"

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

		void setDeep(size_t _value);
		size_t getDeep() const;

	private:
		void notifyAction(const std::string& _name, const std::string& _value, bool _final);

		size_t addParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget);
		void destroyPropertyFields();
		void hidePropertyFields();

		void updateSize();

		IPropertyField* getPropertyField(MyGUI::Widget* _client, const std::string& _name, const std::string& _type);

	private:
		typedef std::map<std::string, IPropertyField*> MapPropertyField;
		MapPropertyField mFields;
		size_t mDeep;
		MyGUI::Widget* mCurrentWidget;
	};

} // namespace tools

#endif // __PANEL_PROPERTIES_H__
