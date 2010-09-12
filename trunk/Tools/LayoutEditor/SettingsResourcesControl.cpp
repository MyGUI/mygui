/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "precompiled.h"
#include "SettingsResourcesControl.h"
#include "SettingsManager.h"

namespace tools
{
	SettingsResourcesControl::SettingsResourcesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsResourcesControl.layout", _parent),
		mResourceAdd(nullptr),
		mResourceDelete(nullptr),
		mResources(nullptr)
	{
		assignWidget(mResourceAdd, "ResourceAdd");
		assignWidget(mResourceDelete, "ResourceDelete");
		assignWidget(mResources, "Resources");

		mResourceAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathDelete);
	}

	SettingsResourcesControl::~SettingsResourcesControl()
	{
		mResourceAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathDelete);
	}

	void SettingsResourcesControl::loadSettings()
	{
		mResources->removeAllItems();
		SettingsSector::VectorUString paths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalResources");
		for (SettingsSector::VectorUString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResources->addItem(*item);
	}

	void SettingsResourcesControl::saveSettings()
	{
		SettingsSector::VectorUString paths;
		for (size_t index = 0; index < mResources->getItemCount(); ++ index)
			paths.push_back(mResources->getItemNameAt(index));
		SettingsManager::getInstance().getSector("Settings")->setPropertyValueList("AdditionalResources", paths);
	}

	void SettingsResourcesControl::notifyClickResourcePathAdd(MyGUI::Widget* _sender)
	{
		//mOpenSaveFileDialog->doModal();
	}

	void SettingsResourcesControl::notifyClickResourcePathDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResources->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResources->removeItemAt(index);
	}

} // namespace tools
