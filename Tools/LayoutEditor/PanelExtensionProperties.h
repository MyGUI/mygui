/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _cb28cb28_cd8a_4233_9919_9860bf4f1bb2_
#define _cb28cb28_cd8a_4233_9919_9860bf4f1bb2_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetTypes.h"
#include "WidgetContainer.h"
#include "IPropertyField.h"

namespace tools
{

	class PanelExtensionProperties : public wraps::BasePanelViewItem
	{
	public:
		PanelExtensionProperties();

		void initialise() override;
		void shutdown() override;

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyAction(std::string_view _name, std::string_view _value, bool _final);

		void AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget);
		void destroyPropertyFields();

		void updateSize();

	private:
		using VectorPropertyField = std::vector<IPropertyField*>;
		VectorPropertyField mFields;
		MyGUI::Widget* mCurrentWidget{nullptr};
	};

}

#endif
