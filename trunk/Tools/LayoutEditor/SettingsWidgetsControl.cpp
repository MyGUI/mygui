/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsWidgetsControl.h"
#include "SettingsManager.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsWidgetsControl)

	SettingsWidgetsControl::SettingsWidgetsControl() :
		mCheckShowName(nullptr),
		mCheckShowType(nullptr),
		mCheckShowSkin(nullptr)
	{
	}

	SettingsWidgetsControl::~SettingsWidgetsControl()
	{
		mCheckShowName->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowType->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowSkin->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
	}

	void SettingsWidgetsControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mCheckShowName, "checkShowName");
		assignWidget(mCheckShowType, "checkShowType");
		assignWidget(mCheckShowSkin, "checkShowSkin");

		mCheckShowName->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowType->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowSkin->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
	}

	void SettingsWidgetsControl::loadSettings()
	{
		mCheckShowName->setStateSelected(SettingsManager::getInstance().getValue<bool>("Settings/ShowName"));
		mCheckShowType->setStateSelected(SettingsManager::getInstance().getValue<bool>("Settings/ShowType"));
		mCheckShowSkin->setStateSelected(SettingsManager::getInstance().getValue<bool>("Settings/ShowSkin"));
	}

	void SettingsWidgetsControl::saveSettings()
	{
		SettingsManager::getInstance().setValue("Settings/ShowName", mCheckShowName->getStateSelected());
		SettingsManager::getInstance().setValue("Settings/ShowType", mCheckShowType->getStateSelected());
		SettingsManager::getInstance().setValue("Settings/ShowSkin", mCheckShowSkin->getStateSelected());
	}

	void SettingsWidgetsControl::notifyToggleCheck(MyGUI::Widget* _sender)
	{
		MyGUI::Button* checkbox = _sender->castType<MyGUI::Button>();
		checkbox->setStateSelected(!checkbox->getStateSelected());
	}

	void SettingsWidgetsControl::OnCommand(const std::string& _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
