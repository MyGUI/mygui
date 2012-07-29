/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _3b456737_fa6e_41b4_bc49_bae1aeea0809_
#define _3b456737_fa6e_41b4_bc49_bae1aeea0809_

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

		void saveSelectedWidget();
		void restoreSelectedWidget();

	private:
		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		void checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

		MyGUI::Widget* findWidgetSelected();
		MyGUI::Widget* findWidgetSelected(WidgetContainer* _container);

	private:
		MyGUI::Widget* mCurrentWidget;
		size_t mSelectDepth;
		MyGUI::IntPoint mLastClickPoint;
		std::string mStoreWidgetTag;
	};

}

#endif
