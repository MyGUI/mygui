/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelStatic.h"

PanelStatic::PanelStatic() :
	BaseLayout("PanelStatic.layout"),
	PanelBase()
{
}

void PanelStatic::initialiseCell(PanelCell * _cell)
{
	PanelBase::initialiseCell(_cell);

	loadLayout(_cell->getClient());
	mMainWidget->setCoord(0, 0, _cell->getClient()->getWidth(), mMainWidget->getHeight());
	_cell->setClientHeight(mMainWidget->getHeight(), false);
	_cell->setCaption("Static panel");

}

void PanelStatic::shutdownCell()
{
	PanelBase::shutdownCell();

	BaseLayout::shutdown();
}
