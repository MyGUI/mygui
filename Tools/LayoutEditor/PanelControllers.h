/*!
	@file
	@author		Georgiy Evmenov
	@date		12/2009
*/
#ifndef __PANEL_CONTROLLERS_H__
#define __PANEL_CONTROLLERS_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

namespace tools
{
	class PanelControllers :
		public wraps::BasePanelViewItem
	{
	public:
		PanelControllers();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

		typedef MyGUI::delegates::CDelegate5<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
		EventHandle_EventCreatePair eventCreatePair;

		typedef MyGUI::delegates::CDelegate1<MyGUI::Widget*> EventHandle_WidgetVoid;
		EventHandle_WidgetVoid eventHidePairs;

	private:
		virtual void notifyChangeWidth(int _width);

		void notifyAdd(MyGUI::Widget* _sender = 0);
		void notifyDelete(MyGUI::Widget* _sender);
		void notifySelectItem(MyGUI::List* _sender, size_t _index);

		void loadControllerTypes(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

	private:
		MyGUI::ComboBox* mControllerName;
		MyGUI::Button* mButtonAdd;
		MyGUI::Button* mButtonDelete;
		MyGUI::List* mList;

		MyGUI::Widget* mCurrentWidget;

		int mButtonLeft;
		int mButtonRight;
		int mButtonSpace;

		std::map<std::string, MyGUI::MapString> mControllersProperties;

		int mPropertyItemHeight;
	};

} // namespace tools

#endif // __PANEL_CONTROLLERS_H__
