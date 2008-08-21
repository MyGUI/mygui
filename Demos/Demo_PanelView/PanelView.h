/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "Panel.h"

class PanelVew : public BaseLayout
{
public:
	PanelVew();

	virtual void initialise();

	void addPanel(Panel * _panel);

private:
	void notifyUpdatePanel(Panel * _panel);
	void updateView();

private:
	MyGUI::ScrollViewPtr mScrollView;
};

#endif // __PANEL_VIEW_H__
