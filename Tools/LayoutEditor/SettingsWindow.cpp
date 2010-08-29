/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "SettingsWindow.h"
#include "SettingsManager.h"

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
		mCheckEdgeHide(nullptr),
		mGridStep(0)
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

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SettingsWindow::notifyWindowButtonPressed);

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
		MyGUI::Edit* text = _sender->castType<MyGUI::Edit>();
		mGridStep = MyGUI::utility::parseInt(text->getOnlyText());
		mGridStep = std::max(1, mGridStep);
		text->setCaption(MyGUI::utility::toString(mGridStep));
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
		SettingsManager::getInstance().getSector("SettingsWindow")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("SettingsWindow")->setPropertyValue("ShowName", getShowName());
		SettingsManager::getInstance().getSector("SettingsWindow")->setPropertyValue("ShowType", getShowType());
		SettingsManager::getInstance().getSector("SettingsWindow")->setPropertyValue("ShowSkin", getShowSkin());
		SettingsManager::getInstance().getSector("SettingsWindow")->setPropertyValue("EdgeHide", getEdgeHide());
	}

	void SettingsWindow::loadSettings()
	{
		mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");
		setShowName(SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<bool>("ShowName"));
		setShowType(SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<bool>("ShowType"));
		setShowSkin(SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<bool>("ShowSkin"));
		setEdgeHide(SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<bool>("EdgeHide"));
	}

	void SettingsWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

} // namespace tools