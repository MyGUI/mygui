/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "Panel.h"
#include "PanelViewWindow.h"

class DemoKeeper
{
public:
	void start();
	void end();

private:
	PanelViewWindow m_view;
	Panel m_panel1;
	Panel m_panel2;
	Panel m_panel3;
	Panel m_panel4;
};

#endif // __DEMO_KEEPER_H__
