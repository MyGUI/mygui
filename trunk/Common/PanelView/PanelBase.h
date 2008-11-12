/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __BASE_PANEL_H__
#define __BASE_PANEL_H__

#include <MyGUI.h>
#include "PanelCell.h"

class PanelBase
{
public:
	PanelBase() : mPanelCell(0), mWidgetClient(null) {};

	virtual ~PanelBase(){};

	virtual void initialiseCell(PanelCell * _cell)
	{
		mPanelCell = _cell;
		mWidgetClient = mPanelCell->getClient();
	}

	virtual void shutdownCell()
	{
		mPanelCell = 0;
		mWidgetClient = null;
	}

	// реально изменилась ширина €чейки
	virtual void notifyChangeWidth(int _width)
	{
	}

	virtual void show()
	{
		mPanelCell->mainWidget()->show();
		mPanelCell->eventUpdatePanel(mPanelCell);
	}

	virtual void hide()
	{
		mPanelCell->mainWidget()->hide();
		mPanelCell->eventUpdatePanel(mPanelCell);
	}

	PanelCell * getPanelCell() { return mPanelCell; }

protected:
	PanelCell * mPanelCell;
	MyGUI::WidgetPtr mWidgetClient;
};

#endif // __BASE_PANEL_H__
