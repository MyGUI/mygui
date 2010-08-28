/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "SettingsWindow.h"
#include "SettingsManager.h"

extern int grid_step;//FIXME_HOOK

SettingsWindow::SettingsWindow() :
	Dialog("SettingsWindow.layout"),
	mGridEdit(nullptr),
	mButtonOkSettings(nullptr),
	mButtonCancel(nullptr),
	mCheckShowName(nullptr),
	mCheckShowType(nullptr),
	mCheckShowSkin(nullptr),
	mCheckEdgeHide(nullptr)
{
	assignWidget(mGridEdit, "gridEdit");
	assignWidget(mButtonOkSettings, "buttonOkSettings");
	assignWidget(mButtonCancel, "Cancel");
	assignWidget(mCheckShowName, "checkShowName");
	assignWidget(mCheckShowType, "checkShowType");
	assignWidget(mCheckShowSkin, "checkShowSkin");
	assignWidget(mCheckEdgeHide, "checkEdgeHide");

	mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStepAccept);
	mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStep);
	mButtonOkSettings->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyOkSettings);
	mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyCancel);

	mCheckShowName->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowName->setUserString("PropertyName", "ShowName");

	mCheckShowType->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowType->setUserString("PropertyName", "ShowType");

	mCheckShowSkin->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowSkin->setUserString("PropertyName", "ShowSkin");

	mCheckEdgeHide->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckEdgeHide->setUserString("PropertyName", "EdgeHide");

	loadSettings();

	mMainWidget->setVisible(false);
}

SettingsWindow::~SettingsWindow()
{
	saveSettings();
}

void SettingsWindow::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
{
	MyGUI::StaticText* text = _sender->castType<MyGUI::StaticText>();
	grid_step = MyGUI::utility::parseInt(text->getCaption());
	grid_step = std::max(1, grid_step);
	text->setCaption(MyGUI::utility::toString(grid_step));
}

void SettingsWindow::notifyNewGridStepAccept(MyGUI::Edit* _sender)
{
	notifyNewGridStep(_sender);
}

void SettingsWindow::notifyOkSettings(MyGUI::Widget* _sender)
{
	eventEndDialog(this, true);
}

void SettingsWindow::notifyCancel(MyGUI::Widget* _sender)
{
	eventEndDialog(this, false);
}

void SettingsWindow::notifyToggleCheck(MyGUI::Widget* _sender)
{
	MyGUI::Button* checkbox = _sender->castType<MyGUI::Button>();
	checkbox->setStateSelected(!checkbox->getStateSelected());
}

void SettingsWindow::onDoModal()
{
	loadSettings();

	MyGUI::IntSize windowSize = mMainWidget->getSize();
	MyGUI::IntSize parentSize = mMainWidget->getParentSize();

	mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
}

void SettingsWindow::onEndModal()
{
}

void SettingsWindow::saveSettings()
{
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "Grid", grid_step);
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowName", getShowName());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowType", getShowType());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowSkin", getShowSkin());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "EdgeHide", getEdgeHide());
}

void SettingsWindow::loadSettings()
{
	grid_step = tools::SettingsManager::getInstance().getPropertyValue<int>("SettingsWindow", "Grid");
	setShowName(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowName"));
	setShowType(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowType"));
	setShowSkin(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowSkin"));
	setEdgeHide(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "EdgeHide"));
}
