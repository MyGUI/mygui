/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __WIDGET_SELECTOR_MANAGER_H__
#define __WIDGET_SELECTOR_MANAGER_H__

#include <MyGUI.h>

namespace tools
{
	typedef MyGUI::delegates::CMultiDelegate1<MyGUI::Widget*> Event_ChangeSelectedWidget;

	class WidgetSelectorManager :
		public MyGUI::Singleton<WidgetSelectorManager>
	{
	public:
		WidgetSelectorManager();
		virtual ~WidgetSelectorManager();

		void initialise();
		void shutdown();

		MyGUI::Widget* getSelectedWidget();
		void setSelectedWidget(MyGUI::Widget* _value);

		Event_ChangeSelectedWidget eventChangeSelectedWidget;

	private:
		MyGUI::Widget* mCurrentWidget;
	};

} // namespace tools

#endif // __WIDGET_SELECTOR_MANAGER_H__
