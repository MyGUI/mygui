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

	class PanelExtensionProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelExtensionProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyAction(const std::string& _name, const std::string& _value, bool _final);

		void AddParametrs(WidgetStyle* widgetType, WidgetContainer* widgetContainer, MyGUI::Widget* _currentWidget);
		void destroyPropertyFields();

		void updateSize();

	private:
		typedef std::vector<IPropertyField*> VectorPropertyField;
		VectorPropertyField mFields;
		MyGUI::Widget* mCurrentWidget;
	};

}

#endif
