/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _057a790b_d065_4d08_866d_24238237dc90_
#define _057a790b_d065_4d08_866d_24238237dc90_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "EditorWidgets.h"

namespace tools
{

	class PanelUserData :
		public wraps::BasePanelViewItem
	{
	public:
		PanelUserData();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

	private:
		virtual void notifyChangeWidth(int _width);

		void notifyAddUserData(MyGUI::Widget* _sender = 0);
		void notifyDeleteUserData(MyGUI::Widget* _sender);
		void notifyUpdateUserData(MyGUI::EditBox* _widget);
		void notifySelectUserDataItem(MyGUI::MultiListBox* _widget, size_t _index);

		bool checkUserData(WidgetContainer* _widgetContainer, const std::string& _key);

	private:
		MyGUI::EditBox* mEditKey;
		MyGUI::EditBox* mEditValue;
		MyGUI::Button* mButtonAdd;
		MyGUI::Button* mButtonDelete;
		MyGUI::MultiListBox* mMultilist;

		MyGUI::Widget* mCurrentWidget;

		int mEditLeft;
		int mEditRight;
		int mEditSpace;
		int mButtonLeft;
		int mButtonRight;
		int mButtonSpace;
	};

}

#endif
