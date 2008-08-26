/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelDynamic.h"

PanelDynamic::PanelDynamic() :
	BasePanel()
{
}

void PanelDynamic::initialiseCell(PanelCell * _cell)
{
	BasePanel::initialiseCell(_cell);

	_cell->setClientHeight(50, false);
	_cell->setCaption("Dynamic");
}

void PanelDynamic::shutdownCell()
{
	BasePanel::shutdownCell();
}
