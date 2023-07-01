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

	class PanelUserData : public wraps::BasePanelViewItem
	{
	public:
		PanelUserData();

		void initialise() override;
		void shutdown() override;

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyChangeWidth(int _width) override;

		void notifyAddUserData(MyGUI::Widget* _sender = nullptr);
		void notifyDeleteUserData(MyGUI::Widget* _sender);
		void notifyUpdateUserData(MyGUI::EditBox* _widget);
		void notifySelectUserDataItem(MyGUI::MultiListBox* _widget, size_t _index);

		bool checkUserData(WidgetContainer* _widgetContainer, std::string_view _key);

	private:
		MyGUI::EditBox* mEditKey{nullptr};
		MyGUI::EditBox* mEditValue{nullptr};
		MyGUI::Button* mButtonAdd{nullptr};
		MyGUI::Button* mButtonDelete{nullptr};
		MyGUI::MultiListBox* mMultilist{nullptr};

		MyGUI::Widget* mCurrentWidget{nullptr};

		int mEditLeft{0};
		int mEditRight{0};
		int mEditSpace{0};
		int mButtonLeft{0};
		int mButtonRight{0};
		int mButtonSpace{0};
	};

}

#endif
