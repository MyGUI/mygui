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

	class PanelItems :
		public wraps::BasePanelViewItem
	{
	public:
		PanelItems();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

	private:
		void updateList();
		void addItem(const std::string& _value);
		void removeItem(size_t _index);

		void notifyAddItem(MyGUI::Widget* _sender = 0);
		void notifyDeleteItem(MyGUI::Widget* _sender);
		void notifyUpdateItem(MyGUI::EditBox* _widget);
		void notifySelectItem(MyGUI::ListBox* _widget, size_t _position);
		virtual void notifyChangeWidth(int _width);

		void selectItem(MyGUI::Widget* _widget);

		void setContainerProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value);

		void setPropertyValue(MyGUI::Widget* _widget, size_t _index, const std::string& _propertyName, const std::string& _propertyValue);
		void erasePropertyValue(MyGUI::Widget* _widget, size_t _index, const std::string& _propertyName);
		void addPropertyValue(MyGUI::Widget* _widget, const std::string& _propertyName, const std::string& _propertyValue);

	private:
		MyGUI::EditBox* mEdit;
		MyGUI::ListBox* mList;
		MyGUI::Button* mButtonAdd;
		MyGUI::Button* mButtonDelete;

		MyGUI::Widget* mCurrentWidget;

		int mButtonLeft;
		int mButtonRight;
		int mButtonSpace;
	};

}

#endif
