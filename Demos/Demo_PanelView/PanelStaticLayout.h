/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_STATIC_LAYOUT_H__
#define __PANEL_STATIC_LAYOUT_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "BasePanel.h"

class PanelStaticLayout : public BaseLayout, public BasePanel
{
public:

	PanelStaticLayout();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

private:

	MyGUI::EditPtr mEditName;
	MyGUI::EditPtr mEditPosition;

};

#endif // __PANEL_STATIC_LAYOUT_H__
