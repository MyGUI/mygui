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
	BaseLayout("SettingsWindow.layout")
{
	assignWidget(mGridEdit, "gridEdit");
	assignWidget(mButtonOkSettings, "buttonOkSettings");
	//assignWidget(mComboboxResolution, "comboboxResolution");
	//assignWidget(mComboboxFullscreen, "comboboxFullscreen");
	assignWidget(mCheckShowName, "checkShowName");
	assignWidget(mCheckShowType, "checkShowType");
	assignWidget(mCheckShowSkin, "checkShowSkin");
	assignWidget(mCheckEdgeHide, "checkEdgeHide");

	mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStepAccept);
	mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStep);
	mButtonOkSettings->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyOkSettings);

	/*Ogre::ConfigOptionMap map = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator iter = map.find("Video Mode");
	int selectedIdx = 0;
	int wid, hei;
	for (unsigned int j = 0; j<iter->second.possibleValues.size();j++)
	{
		std::string videoMode = iter->second.possibleValues[j];
		std::string tmp;
		std::istringstream str(videoMode);
		str >> wid >> tmp >> hei;
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		mComboboxResolution->addItem(MyGUI::utility::toString(wid, " x ", hei));
	}
	mComboboxResolution->setIndexSelected(selectedIdx);

	iter = map.find("Full Screen");
	selectedIdx = 0;
	for (size_t j = 0; j<iter->second.possibleValues.size();j++)
	{
		std::string videoMode = iter->second.possibleValues[j];
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		mComboboxFullscreen->addItem(videoMode);
	}
	mComboboxFullscreen->setIndexSelected(selectedIdx);*/

	mCheckShowName->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowName->setUserString("PropertyName", "ShowName");

	mCheckShowType->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowType->setUserString("PropertyName", "ShowType");

	mCheckShowSkin->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowSkin->setUserString("PropertyName", "ShowSkin");

	mCheckEdgeHide->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckEdgeHide->setUserString("PropertyName", "EdgeHide");

	grid_step = tools::SettingsManager::getInstance().getPropertyValue<int>("SettingsWindow", "Grid");
	setShowName(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowName"));
	setShowType(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowType"));
	setShowSkin(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowSkin"));
	setEdgeHide(tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "EdgeHide"));

	mMainWidget->setVisible(false);
}

SettingsWindow::~SettingsWindow()
{
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "Grid", grid_step);
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowName", getShowName());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowType", getShowType());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "ShowSkin", getShowSkin());
	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", "EdgeHide", getEdgeHide());
}

void SettingsWindow::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
{
	MyGUI::StaticText* text = _sender->castType<MyGUI::StaticText>();
	grid_step = MyGUI::utility::parseInt(text->getCaption());
	if (grid_step <= 0)
		grid_step = 1;
	text->setCaption(MyGUI::utility::toString(grid_step));
}

void SettingsWindow::notifyNewGridStepAccept(MyGUI::Edit* _sender)
{
	notifyNewGridStep(_sender);
}

void SettingsWindow::notifyOkSettings(MyGUI::Widget* _sender)
{
	//bool fullscreen;
	//int width, height;
	//std::string tmp;
	//std::istringstream str(mComboboxResolution->getCaption());
	//str >> width >> tmp >> height;
	//fullscreen = (mComboboxFullscreen->getCaption() == "Yes");
	//BasisManager::getInstance().setFullscreen(fullscreen);//setFullscreen, width, height);
	mMainWidget->setVisible(false);
}

void SettingsWindow::notifyToggleCheck(MyGUI::Widget* _sender)
{
	MyGUI::Button* checkbox = _sender->castType<MyGUI::Button>();
	checkbox->setStateSelected(!checkbox->getStateSelected());

	tools::SettingsManager::getInstance().setPropertyValue("SettingsWindow", _sender->getUserString("PropertyName"), checkbox->getStateSelected());
}

void SettingsWindow::setVisible(bool _value)
{
	if (mMainWidget->getVisible() != _value)
	{
		mMainWidget->setVisible(_value);

		if (_value)
		{
			MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
			addDialog(this);

			MyGUI::IntSize windowSize = mMainWidget->getSize();
			MyGUI::IntSize parentSize = mMainWidget->getParentSize();

			mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
		}
		else
		{
			MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
			removeDialog(this);
		}
	}
}

bool SettingsWindow::getVisible()
{
	return mMainWidget->getVisible();
}
