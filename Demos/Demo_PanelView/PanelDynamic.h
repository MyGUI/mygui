/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_DYNAMIC_H__
#define __PANEL_DYNAMIC_H__

#include <MyGUI.h>
#include "PanelView/PanelBase.h"

class PanelDynamic : public PanelBase
{
public:

	PanelDynamic();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

	void setVisibleCount(size_t _count);

private:
	MyGUI::VectorWidgetPtr mItemsText;
	MyGUI::VectorWidgetPtr mItemsEdit;

};

#endif // __PANEL_DYNAMIC_H__
