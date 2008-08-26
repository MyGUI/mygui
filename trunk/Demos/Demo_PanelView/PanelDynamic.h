/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_DYNAMIC_H__
#define __PANEL_DYNAMIC_H__

#include <MyGUI.h>
#include "BasePanel.h"

class PanelDynamic : public BasePanel
{
public:

	PanelDynamic();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

private:

	//MyGUI::EditPtr mEditName;
	//MyGUI::EditPtr mEditPosition;

};

#endif // __PANEL_DYNAMIC_H__
