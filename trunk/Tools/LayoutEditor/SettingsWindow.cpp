/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "SettingsWindow.h"
#include "SettingsManager.h"
#include "Localise.h"
#include "BackwardCompatibilityManager.h"

namespace tools
{
	SettingsWindow::SettingsWindow() :
		Dialog("SettingsWindow.layout"),
		mGridEdit(nullptr),
		mButtonOkSettings(nullptr),
		mButtonCancel(nullptr),
		mCheckShowName(nullptr),
		mCheckShowType(nullptr),
		mCheckShowSkin(nullptr),
		mResourcePathAdd(nullptr),
		mResourcePathDelete(nullptr),
		mResourcePaths(nullptr),
		mLayoutVersion(nullptr),
		mOpenSaveFileDialog(nullptr),
		mGridStep(0),
		mSettingsResourcesControl(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mButtonOkSettings, "buttonOkSettings");
		assignWidget(mButtonCancel, "Cancel");
		assignWidget(mCheckShowName, "checkShowName");
		assignWidget(mCheckShowType, "checkShowType");
		assignWidget(mCheckShowSkin, "checkShowSkin");

		assignWidget(mResourcePathAdd, "ResourcePathAdd");
		assignWidget(mResourcePathDelete, "ResourcePathDelete");
		assignWidget(mResourcePaths, "ResourcePaths");

		assignWidget(mLayoutVersion, "LayoutVersion");

		assignBase(mSettingsResourcesControl, "SettingsResourcesControl");

		mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStep);
		mButtonOkSettings->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyOkSettings);
		mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyCancel);

		mResourcePathAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyClickResourcePathAdd);
		mResourcePathDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyClickResourcePathDelete);

		MyGUI::VectorString versions = BackwardCompatibilityManager::getInstancePtr()->getVersions();
		for (size_t index = 0; index < versions.size(); ++ index)
		{
			mLayoutVersion->addItem(versions[index]);
			if (BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion() == versions[index])
				mLayoutVersion->setIndexSelected(index);
		}

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SettingsWindow::notifyWindowButtonPressed);

		mCheckShowName->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
		mCheckShowType->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
		mCheckShowSkin->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);

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

	void SettingsWindow::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mGridStep = MyGUI::utility::parseInt(mGridEdit->getOnlyText());
		mGridStep = std::max(1, mGridStep);
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
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
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowName", getShowName());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowType", getShowType());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowSkin", getShowSkin());

		SettingsSector::VectorUString paths;
		for (size_t index = 0; index < mResourcePaths->getItemCount(); ++ index)
			paths.push_back(mResourcePaths->getItemNameAt(index));
		SettingsManager::getInstance().getSector("Settings")->setPropertyValueList("AdditionalPaths", paths);

		if (mLayoutVersion->getIndexSelected() != MyGUI::ITEM_NONE)
		{
			std::string versionName = mLayoutVersion->getItemNameAt(mLayoutVersion->getIndexSelected());
			if (versionName != BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion())
				BackwardCompatibilityManager::getInstancePtr()->setCurrentVersion(versionName);
		}

		mSettingsResourcesControl->saveSettings();
	}

	void SettingsWindow::loadSettings()
	{
		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));

		setShowName(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowName"));
		setShowType(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowType"));
		setShowSkin(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowSkin"));

		mResourcePaths->removeAllItems();
		SettingsSector::VectorUString paths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalPaths");
		for (SettingsSector::VectorUString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResourcePaths->addItem(*item);

		mSettingsResourcesControl->loadSettings();
	}

	void SettingsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

	bool SettingsWindow::getShowName()
	{
		return mCheckShowName->getStateSelected();
	}

	void SettingsWindow::setShowName(bool _pressed)
	{
		mCheckShowName->setStateSelected(_pressed);
	}

	bool SettingsWindow::getShowType()
	{
		return mCheckShowType->getStateSelected();
	}

	void SettingsWindow::setShowType(bool _pressed)
	{
		mCheckShowType->setStateSelected(_pressed);
	}

	bool SettingsWindow::getShowSkin()
	{
		return mCheckShowSkin->getStateSelected();
	}

	void SettingsWindow::setShowSkin(bool _pressed)
	{
		mCheckShowSkin->setStateSelected(_pressed);
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
