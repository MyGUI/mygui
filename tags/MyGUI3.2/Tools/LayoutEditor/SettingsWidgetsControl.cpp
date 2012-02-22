/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
#include "SettingsWidgetsControl.h"
#include "SettingsManager.h"

namespace tools
{
	SettingsWidgetsControl::SettingsWidgetsControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsWidgetsControl.layout", _parent),
		mCheckShowName(nullptr),
		mCheckShowType(nullptr),
		mCheckShowSkin(nullptr)
	{
		assignWidget(mCheckShowName, "checkShowName");
		assignWidget(mCheckShowType, "checkShowType");
		assignWidget(mCheckShowSkin, "checkShowSkin");

		mCheckShowName->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowType->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowSkin->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
	}

	SettingsWidgetsControl::~SettingsWidgetsControl()
	{
		mCheckShowName->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowType->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
		mCheckShowSkin->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsWidgetsControl::notifyToggleCheck);
	}

	void SettingsWidgetsControl::loadSettings()
	{
		mCheckShowName->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowName"));
		mCheckShowType->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowType"));
		mCheckShowSkin->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowSkin"));
	}

	void SettingsWidgetsControl::saveSettings()
	{
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowName", mCheckShowName->getStateSelected());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowType", mCheckShowType->getStateSelected());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ShowSkin", mCheckShowSkin->getStateSelected());
	}

	void SettingsWidgetsControl::notifyToggleCheck(MyGUI::Widget* _sender)
	{
		MyGUI::Button* checkbox = _sender->castType<MyGUI::Button>();
		checkbox->setStateSelected(!checkbox->getStateSelected());
	}

} // namespace tools
