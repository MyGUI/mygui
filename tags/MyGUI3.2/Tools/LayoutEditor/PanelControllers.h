/*!
	@file
	@author		Georgiy Evmenov
	@date		12/2009
*/
#ifndef __PANEL_CONTROLLERS_H__
#define __PANEL_CONTROLLERS_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "IPropertyField.h"

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

	private:
		void notifyAction(const std::string& _name, const std::string& _value, bool _final);

		virtual void notifyChangeWidth(int _width);

		void notifyAdd(MyGUI::Widget* _sender = 0);
		void notifyDelete(MyGUI::Widget* _sender);
		void notifySelectItem(MyGUI::ListBox* _sender, size_t _index);

		void loadControllerTypes(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
		void destroyPropertyFields();

		void updateSize();

	private:
		MyGUI::ComboBox* mControllerName;
		MyGUI::Button* mButtonAdd;
		MyGUI::Button* mButtonDelete;
		MyGUI::ListBox* mList;

		MyGUI::Widget* mCurrentWidget;

		int mButtonLeft;
		int mButtonRight;
		int mButtonSpace;

		typedef std::map<std::string, MyGUI::MapString> MapMapString;
		MapMapString mControllersProperties;

		typedef std::vector<IPropertyField*> VectorPropertyField;
		VectorPropertyField mFields;

		size_t mIndexSelected;
	};

} // namespace tools

#endif // __PANEL_CONTROLLERS_H__
