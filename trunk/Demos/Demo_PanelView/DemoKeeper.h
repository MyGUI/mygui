/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "PanelCell.h"
#include "PanelViewWindow.h"
#include "PanelStaticLayout.h"

class DemoKeeper
{
public:
	void start();
	void end();

private:
	PanelViewWindow m_view;
	PanelStaticLayout m_panel1;
	/*PanelCell m_panel2;
	PanelCell m_panel3;
	PanelCell m_panel4;*/
};

#endif // __DEMO_KEEPER_H__
