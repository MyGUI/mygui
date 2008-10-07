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
	mComboboxResolution->setItemSelectedAt(selectedIdx);

	iter = map.find("Full Screen");
	selectedIdx = 0;
	for (size_t j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		mComboboxFullscreen->addItem(videoMode);
	}
	mComboboxFullscreen->setItemSelectedAt(selectedIdx);

	mCheckShowName->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowType->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckShowSkin->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
	mCheckEdgeHide->eventMouseButtonClick = MyGUI::newDelegate(this, &SettingsWindow::notifyToggleCheck);
}

void SettingsWindow::load(MyGUI::xml::xmlNodeIterator _field)
{
	MyGUI::xml::xmlNodeIterator field = _field->getNodeIterator();
	while (field.nextNode()) {
		std::string key, value;

		if (field->getName() == "Property")
		{
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;

			if (key == "Grid") grid_step = MyGUI::utility::parseInt(value);
			else if (key == "ShowName")
				setShowName(MyGUI::utility::parseBool(value));
			else if (key == "ShowType")
				setShowType(MyGUI::utility::parseBool(value));
			else if (key == "ShowSkin")
				setShowSkin(MyGUI::utility::parseBool(value));
			else if (key == "EdgeHide")
				setEdgeHide(MyGUI::utility::parseBool(value));
		}
	}

	if (grid_step <= 0) grid_step = 1;
	mGridEdit->setCaption(MyGUI::utility::toString(grid_step));
}

void SettingsWindow::save(MyGUI::xml::xmlNodePtr root)
{
	root = root->createChild("SettingsWindow");
	MyGUI::xml::xmlNodePtr nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "Grid");
	nodeProp->addAttributes("value", grid_step);

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowName");
	nodeProp->addAttributes("value", getShowName());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowType");
	nodeProp->addAttributes("value", getShowType());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowSkin");
	nodeProp->addAttributes("value", getShowSkin());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "EdgeHide");
	nodeProp->addAttributes("value", getEdgeHide());
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
