/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_DIRECTOR_H__
#define __PANEL_DIRECTOR_H__

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	enum EventInfo {
		EVENT_SHOW_STATIC,
		EVENT_SHOW_DYNAMIC,
		EVENT_COUNT_DYNAMIC
	};

	typedef MyGUI::delegates::CDelegate2<int, size_t> EventHandle_EventDirector;

	class PanelDirector : public wraps::BasePanelViewItem
	{
	public:
		PanelDirector();

		virtual void initialise();
		virtual void shutdown();

		EventHandle_EventDirector eventChangePanels;

	private:
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index);

	private:
		MyGUI::ButtonPtr mCheckShowStatic;
		MyGUI::ButtonPtr mCheckShowDynamic;
		MyGUI::ComboBoxPtr mComboCount;

	};

} // namespace demo

#endif // __PANEL_DIRECTOR_H__
