/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

namespace tools
{
	class PanelMainProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelMainProperties();

		virtual void initialise();
		virtual void shutdown();

		void notifyToggleRelativeMode(MyGUI::Widget* _sender = nullptr);
		void update(MyGUI::Widget* _currentWidget);

		typedef MyGUI::delegates::CDelegate5<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
		EventHandle_EventCreatePair eventCreatePair;

		typedef MyGUI::delegates::CDelegate1<const std::string&> EventHandle_EventSetPositionText;
		EventHandle_EventSetPositionText eventSetPositionText;

		//FIXME
		MyGUI::Widget* getMainWidget()
		{
			return mWidgetClient;
		}

	private:
		MyGUI::Button* mButtonRelativePosition;

		MyGUI::Widget* mCurrentWidget;
		int mPropertyItemHeight;
	};

} // namespace tools

#endif // __PANEL_MAIN_PROPERTIES_H__
