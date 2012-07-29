/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _43563b75_17fc_4430_ae3c_864c614811af_
#define _43563b75_17fc_4430_ae3c_864c614811af_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetTypes.h"
#include "WidgetContainer.h"
#include "IPropertyField.h"

namespace tools
{

	class PanelTemplateProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelTemplateProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget, WidgetStyle* _widgetType);

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
