/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "SettingsWindow.h"
#include "BasisManager.h"


SettingsWindow::SettingsWindow() :
	BaseLayout("SettingsWindow.layout")
{
}

void SettingsWindow::initialise()
{
	loadLayout();

	assignWidget(mGridEdit, "gridEdit");
	assignWidget(mButtonOkSettings, "buttonOkSettings");
	assignWidget(mComboboxResolution, "comboboxResolution");
	assignWidget(mComboboxFullscreen, "comboboxFullscreen");
	assignWidget(mCheckShowName, "checkShowName");
	assignWidget(mCheckShowType, "checkShowType");
	assignWidget(mCheckShowSkin, "checkShowSkin");
	assignWidget(mCheckEdgeHide, "checkEdgeHide");

	mGridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStepAccept);
	mGridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &SettingsWindow::notifyNewGridStep);
	mGridEdit->setCaption(MyGUI::utility::toString(grid_step));
	mButtonOkSettings->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyOkSettings);

	Ogre::ConfigOptionMap map = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator iter = map.find("Video Mode");
	int selectedIdx = 0;
	int wid, hei;
	for (unsigned int j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		std::string tmp;
		std::istringstream str(videoMode);
		str >> wid >> tmp >> hei;
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		mComboboxResolution->addItem(MyGUI::utility::toString(wid, " x ", hei));
	}
	mComboboxResolution->setItemSelect(selectedIdx);

	iter = map.find("Full Screen");
	selectedIdx = 0;
	for (size_t j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		mComboboxFullscreen->addItem(videoMode);
	}
	mComboboxFullscreen->setItemSelect(selectedIdx);

	mCheckShowName->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowType->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowSkin->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckEdgeHide->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
}

void SettingsWindow::notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0) grid_step = 1;
	_sender->setCaption(Ogre::StringConverter::toString(grid_step));
}

void SettingsWindow::notifyNewGridStepAccept(MyGUI::WidgetPtr _sender)
{
	notifyNewGridStep(_sender);
}

void SettingsWindow::notifyOkSettings(MyGUI::WidgetPtr _sender)
{
	bool fullscreen;
	int width, height;
	std::string tmp;
	std::istringstream str(mComboboxResolution->getCaption());
	str >> width >> tmp >> height;
	fullscreen = (mComboboxFullscreen->getCaption() == "Yes");
	BasisManager::getInstance().setFullscreen(fullscreen);//setFullscreen, width, height);
	mainWidget()->hide();
}

void SettingsWindow::notifyToggleCheck(MyGUI::WidgetPtr _sender)
{
	MyGUI::ButtonPtr checkbox = _sender->castType<MyGUI::Button>();
	checkbox->setButtonPressed(!checkbox->getButtonPressed());
	eventWidgetsUpdate(true);
}
