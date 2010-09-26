/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __WIDGET_SELECTOR_MANAGER_H__
#define __WIDGET_SELECTOR_MANAGER_H__

#include <MyGUI.h>
#include "WidgetContainer.h"

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

		void selectWidget(const MyGUI::IntPoint& _mousePosition);
		void resetDepth();

	private:
		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		bool checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

	private:
		MyGUI::Widget* mCurrentWidget;
		size_t mSelectDepth;
		MyGUI::IntPoint mLastClickPoint;
	};

} // namespace tools

#endif // __WIDGET_SELECTOR_MANAGER_H__
