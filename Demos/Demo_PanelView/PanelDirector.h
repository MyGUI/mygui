/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_DIRECTOR_H__
#define __PANEL_DIRECTOR_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

enum EventInfo {
	EVENT_SHOW_STATIC,
	EVENT_SHOW_DYNAMIC,
	EVENT_COUNT_DYNAMIC
};

typedef MyGUI::delegates::CDelegate2<int, size_t> EventInfo_EventDirector;

class PanelDirector : public wraps::BaseLayout, public PanelBase
{
public:

	PanelDirector();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

	EventInfo_EventDirector eventChangePanels;

private:
	void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
	void notifyComboAccept(MyGUI::WidgetPtr _sender, size_t _index);

private:

	MyGUI::ButtonPtr mCheckShowStatic;
	MyGUI::ButtonPtr mCheckShowDynamic;
	MyGUI::ComboBoxPtr mComboCount;

};

#endif // __PANEL_DIRECTOR_H__
