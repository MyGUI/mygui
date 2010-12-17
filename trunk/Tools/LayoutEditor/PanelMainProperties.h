/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "IPropertyField.h"

namespace tools
{
	class PanelMainProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelMainProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyToggleRelativeMode(MyGUI::Widget* _sender = nullptr);

		void commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result);
		void notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner);
		void updatePositionCaption();
		void destroyPropertyFields();

		void updateSize();

	private:
		MyGUI::Button* mButtonRelativePosition;

		MyGUI::Widget* mCurrentWidget;
		typedef std::vector<IPropertyField*> VectorPropertyField;
		VectorPropertyField mFields;
		IPropertyField* mPositionField;
	};

} // namespace tools

#endif // __PANEL_MAIN_PROPERTIES_H__
