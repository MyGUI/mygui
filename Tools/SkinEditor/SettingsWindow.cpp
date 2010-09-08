/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "SettingsWindow.h"
#include "SettingsManager.h"
#include "Localise.h"

namespace tools
{
	SettingsWindow::SettingsWindow() :
		Dialog("SettingsWindow.layout"),
		mButtonOkSettings(nullptr),
		mButtonCancel(nullptr),
		mResourcePathAdd(nullptr),
		mResourcePathDelete(nullptr),
		mResourcePaths(nullptr),
		mOpenSaveFileDialog(nullptr)
	{
		assignWidget(mButtonOkSettings, "buttonOkSettings");
		assignWidget(mButtonCancel, "Cancel");

		assignWidget(mResourcePathAdd, "ResourcePathAdd");
		assignWidget(mResourcePathDelete, "ResourcePathDelete");
		assignWidget(mResourcePaths, "ResourcePaths");

		mButtonOkSettings->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyOkSettings);
		mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyCancel);

		mResourcePathAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyClickResourcePathAdd);
		mResourcePathDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyClickResourcePathDelete);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SettingsWindow::notifyWindowButtonPressed);

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFolder"), replaceTags("ButtonOpenFolder"), true);
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &SettingsWindow::notifyEndDialogOpenSaveFile);

		loadSettings();

		mMainWidget->setVisible(false);
	}

	SettingsWindow::~SettingsWindow()
	{
		saveSettings();

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;
	}

	void SettingsWindow::notifyOkSettings(MyGUI::Widget* _sender)
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
		SettingsSector::VectorUString paths;
		for (size_t index = 0; index < mResourcePaths->getItemCount(); ++ index)
			paths.push_back(mResourcePaths->getItemNameAt(index));
		SettingsManager::getInstance().getSector("Settings")->setPropertyValueList("AdditionalPaths", paths);
	}

	void SettingsWindow::loadSettings()
	{
		mResourcePaths->removeAllItems();
		SettingsSector::VectorUString paths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalPaths");
		for (SettingsSector::VectorUString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResourcePaths->addItem(*item);
	}

	void SettingsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

	void SettingsWindow::notifyClickResourcePathAdd(MyGUI::Widget* _sender)
	{
		mOpenSaveFileDialog->doModal();
	}

	void SettingsWindow::notifyClickResourcePathDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResourcePaths->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResourcePaths->removeItemAt(index);
	}

	void SettingsWindow::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
			mResourcePaths->addItem(mOpenSaveFileDialog->getCurrentFolder());

		mOpenSaveFileDialog->endModal();
	}

} // namespace tools
