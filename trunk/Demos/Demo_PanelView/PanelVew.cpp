/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelVew.h"

PanelVew::PanelVew() :
	BaseLayout("PanelVew.layout")
{
}

void PanelVew::initialise()
{
	loadLayout();

	assignWidget(mScrollView, "scroll_View");
}

void PanelVew::addPanel(Panel * _panel)
{
	_panel->initialise(mMainWidget);
	_panel->eventUpdatePanel = MyGUI::newDelegate(this, &PanelVew::notifyUpdatePanel);
}

void PanelView::notifyUpdatePanel(Panel * _panel)
{
	updateView();
}

void PanelView::updateView()
{
}
