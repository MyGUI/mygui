/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_STATIC_H__
#define __PANEL_STATIC_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "PanelBase.h"

class PanelStatic : public BaseLayout, public PanelBase
{
public:

	PanelStatic();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

};

#endif // __PANEL_STATIC_H__
