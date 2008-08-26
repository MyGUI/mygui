/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelStaticLayout.h"

PanelStaticLayout::PanelStaticLayout() :
	BaseLayout("PanelMain.layout"),
	BasePanel()
{
}

void PanelStaticLayout::initialiseCell(PanelCell * _cell)
{
	BasePanel::initialiseCell(_cell);

	loadLayout(_cell->getClient());
	mMainWidget->setPosition(0, 0);

	assignWidget(mEditName, "edit_Name");
	assignWidget(mEditPosition, "edit_Position");

	_cell->setClientHeight(mMainWidget->getHeight(), false);
	_cell->setCaption("StaticLayout");
}

void PanelStaticLayout::shutdownCell()
{
	BasePanel::shutdownCell();

	BaseLayout::shutdown();
}
