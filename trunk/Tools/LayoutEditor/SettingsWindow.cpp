/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "SettingsWindow.h"

namespace tools
{
	SettingsWindow::SettingsWindow() :
		Dialog("SettingsWindow.layout"),
		mButtonOk(nullptr),
		mButtonCancel(nullptr),
		mSettingsResourcesControl(nullptr),
		mSettingsResourcePathsControl(nullptr),
		mSettingsGeneralControl(nullptr),
		mSettingsWidgetsControl(nullptr)
	{
		assignWidget(mButtonOk, "Ok");
		assignWidget(mButtonCancel, "Cancel");

		assignBase(mSettingsResourcesControl, "SettingsResourcesControl");
		assignBase(mSettingsResourcePathsControl, "SettingsResourcePathsControl");
		assignBase(mSettingsGeneralControl, "SettingsGeneralControl");
		assignBase(mSettingsWidgetsControl, "SettingsWidgetsControl");

		mButtonOk->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyOk);
		mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyCancel);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SettingsWindow::notifyWindowButtonPressed);

		loadSettings();

		mMainWidget->setVisible(false);
	}

	SettingsWindow::~SettingsWindow()
	{
	}

	void SettingsWindow::notifyOk(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, true);
	}

	void SettingsWindow::notifyCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
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
		mSettingsResourcesControl->saveSettings();
		mSettingsResourcePathsControl->saveSettings();
		mSettingsGeneralControl->saveSettings();
		mSettingsWidgetsControl->saveSettings();
	}

	void SettingsWindow::loadSettings()
	{
		mSettingsResourcesControl->loadSettings();
		mSettingsResourcePathsControl->loadSettings();
		mSettingsGeneralControl->loadSettings();
		mSettingsWidgetsControl->loadSettings();
	}

	void SettingsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

} // namespace tools
