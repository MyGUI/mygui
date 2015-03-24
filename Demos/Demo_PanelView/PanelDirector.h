/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef PANEL_DIRECTOR_H_
#define PANEL_DIRECTOR_H_

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	enum EventInfo
	{
		EVENT_SHOW_STATIC,
		EVENT_SHOW_DYNAMIC,
		EVENT_COUNT_DYNAMIC
	};

	typedef MyGUI::delegates::CDelegate2<int, size_t> EventHandle_EventDirector;

	class PanelDirector :
		public wraps::BasePanelViewItem
	{
	public:
		PanelDirector();

		virtual void initialise();
		virtual void shutdown();

		EventHandle_EventDirector eventChangePanels;

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);

	private:
		MyGUI::Button* mCheckShowStatic;
		MyGUI::Button* mCheckShowDynamic;
		MyGUI::ComboBox* mComboCount;
	};

} // namespace demo

#endif // PANEL_DIRECTOR_H_
