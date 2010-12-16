/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "PropertyField.h"

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

		typedef MyGUI::delegates::CDelegate6<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int, PropertyField&> EventHandle_EventCreatePair;
		EventHandle_EventCreatePair eventCreatePair;

	private:
		void notifyToggleRelativeMode(MyGUI::Widget* _sender = nullptr);

		void commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result);
		void notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner);
		void updatePositionCaption();

	private:
		MyGUI::Button* mButtonRelativePosition;

		MyGUI::Widget* mCurrentWidget;
		int mPropertyItemHeight;
		MyGUI::TextBox* mPositionEdit;
	};

} // namespace tools

#endif // __PANEL_MAIN_PROPERTIES_H__
