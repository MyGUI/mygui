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

class BasePanel
{
public:
	BasePanel() : m_panelCell(0)
	{
	}

	virtual void initialiseCell(PanelCell * _cell)
	{
		m_panelCell = _cell;
	}

	virtual void shutdownCell()
	{
		m_panelCell = 0;
	}

	inline PanelCell * getPanelCell() { return m_panelCell; }

protected:
	PanelCell * m_panelCell;
};

#endif // __BASE_PANEL_H__
