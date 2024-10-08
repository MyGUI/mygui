/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _96474ba1_c55c_4e24_8076_f88c873a0043_
#define _96474ba1_c55c_4e24_8076_f88c873a0043_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetTypes.h"
#include "WidgetContainer.h"
#include "IPropertyField.h"

namespace tools
{

	class PanelProperties : public wraps::BasePanelViewItem
	{
	public:
		PanelProperties();

		void initialise() override;
		void shutdown() override;

		void update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType);

		void setDepth(size_t _value);
		size_t getDepth() const;

	private:
		void notifyAction(std::string_view _name, std::string_view _value, bool _final);

		size_t addParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget);
		void destroyPropertyFields();
		void hidePropertyFields();

		void updateSize();

		IPropertyField* getPropertyField(MyGUI::Widget* _client, std::string_view _name, std::string_view _type);

	private:
		using MapPropertyField = std::map<std::string, IPropertyField*, std::less<>>;
		MapPropertyField mFields;
		size_t mDepth{0};
		MyGUI::Widget* mCurrentWidget{nullptr};
	};

}

#endif
