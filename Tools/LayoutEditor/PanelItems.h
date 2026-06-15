/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _f8e3cc5e_1360_4275_b9d9_3295570d5149_
#define _f8e3cc5e_1360_4275_b9d9_3295570d5149_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetContainer.h"

namespace tools
{

	class PanelItems : public wraps::BasePanelViewItem
	{
	public:
		PanelItems();

		void initialise() override;
		void shutdown() override;

		void update(MyGUI::Widget* _currentWidget);

	private:
		void updateList();
		void addItem(std::string_view _value);
		void removeItem(size_t _index);

		void notifyAddItem(MyGUI::Widget* _sender = nullptr);
		void notifyDeleteItem(MyGUI::Widget* _sender);
		void notifyUpdateItem(MyGUI::EditBox* _widget);
		void notifySelectItem(MyGUI::ListBox* _widget, size_t _position);
		void notifyChangeWidth(int _width) override;

		void selectItem(MyGUI::Widget* _widget);

		void setContainerProperty(MyGUI::Widget* _widget, std::string_view _key, std::string_view _value);

		void setPropertyValue(
			MyGUI::Widget* _widget,
			size_t _index,
			std::string_view _propertyName,
			std::string_view _propertyValue);
		void erasePropertyValue(MyGUI::Widget* _widget, size_t _index, std::string_view _propertyName);
		void addPropertyValue(MyGUI::Widget* _widget, std::string_view _propertyName, std::string_view _propertyValue);

	private:
		MyGUI::EditBox* mEdit{nullptr};
		MyGUI::ListBox* mList{nullptr};
		MyGUI::Button* mButtonAdd{nullptr};
		MyGUI::Button* mButtonDelete{nullptr};

		MyGUI::Widget* mCurrentWidget{nullptr};

		int mButtonLeft{0};
		int mButtonRight{0};
		int mButtonSpace{0};
	};

}

#endif
