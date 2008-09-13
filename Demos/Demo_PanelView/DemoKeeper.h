/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "PanelView/PanelCell.h"
#include "PanelView/PanelViewWindow.h"
#include "PanelDirector.h"
#include "PanelDynamic.h"
#include "PanelStatic.h"

class DemoKeeper
{
public:
	void start();
	void end();

private:
	void notifyChangePanels(int _key, size_t _value);

private:
	PanelViewWindow mView;
	PanelDirector mPanelDirector;
	PanelDynamic mPanelDynamic;
	PanelStatic mPanelStatic;
};

#endif // __DEMO_KEEPER_H__
