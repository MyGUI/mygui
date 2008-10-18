/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelDirector.h"

PanelDirector::PanelDirector() :
	BaseLayout("PanelDirector.layout"),
	PanelBase()
{
}

void PanelDirector::initialiseCell(PanelCell * _cell)
{
	PanelBase::initialiseCell(_cell);

	loadLayout(_cell->getClient());
	mMainWidget->setCoord(0, 0, _cell->getClient()->getWidth(), mMainWidget->getHeight());
	_cell->setClientHeight(mMainWidget->getHeight(), false);
	_cell->setCaption("Director panel");

	assignWidget(mCheckShowStatic, "check_Static");
	assignWidget(mCheckShowDynamic, "check_Dynamic");
	assignWidget(mComboCount, "combo_Count");

	mCheckShowStatic->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
	mCheckShowDynamic->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
	mComboCount->eventComboAccept = MyGUI::newDelegate(this, &PanelDirector::notifyComboAccept);
	mComboCount->setItemSelectedAt(4);
}

void PanelDirector::shutdownCell()
{
	PanelBase::shutdownCell();

	BaseLayout::shutdown();
}

void PanelDirector::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
{
	if (_sender == mCheckShowStatic) {
		mCheckShowStatic->setButtonPressed( ! mCheckShowStatic->getButtonPressed());
		eventChangePanels(EVENT_SHOW_STATIC, (size_t)mCheckShowStatic->getButtonPressed());
	}
	else if (_sender == mCheckShowDynamic) {
		mCheckShowDynamic->setButtonPressed( ! mCheckShowDynamic->getButtonPressed());
		eventChangePanels(EVENT_SHOW_DYNAMIC, (size_t)mCheckShowDynamic->getButtonPressed());
	}
}

void PanelDirector::notifyComboAccept(MyGUI::WidgetPtr _sender)
{
	eventChangePanels(EVENT_COUNT_DYNAMIC, MyGUI::utility::parseInt(_sender->getCaption()));
}
