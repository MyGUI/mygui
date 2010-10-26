/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
#include "SettingsUpdateResourcesControl.h"
#include "SettingsManager.h"
#include "Localise.h"

namespace tools
{
	SettingsUpdateResourcesControl::SettingsUpdateResourcesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsUpdateResourcesControl.layout", _parent),
		mResourceAdd(nullptr),
		mResourceDelete(nullptr),
		mResources(nullptr),
		mTextFieldControl(nullptr)
	{
		assignWidget(mResourceAdd, "ResourceAdd");
		assignWidget(mResourceDelete, "ResourceDelete");
		assignWidget(mResources, "Resources");

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->eventEndDialog = MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyEndDialog);

		mResourceAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickResourcePathDelete);
	}

	SettingsUpdateResourcesControl::~SettingsUpdateResourcesControl()
	{
		mResourceAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickResourcePathDelete);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void SettingsUpdateResourcesControl::loadSettings()
	{
		mResources->removeAllItems();
		SettingsSector::VectorUString paths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("UpdateResources");
		for (SettingsSector::VectorUString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResources->addItem(*item);
	}

	void SettingsUpdateResourcesControl::saveSettings()
	{
		SettingsSector::VectorUString paths;
		for (size_t index = 0; index < mResources->getItemCount(); ++ index)
			paths.push_back(mResources->getItemNameAt(index));
		SettingsManager::getInstance().getSector("Settings")->setPropertyValueList("UpdateResources", paths);
	}

	void SettingsUpdateResourcesControl::notifyClickResourcePathAdd(MyGUI::Widget* _sender)
	{
		mTextFieldControl->setCaption(replaceTags("CaptionAddResource"));
		mTextFieldControl->setTextField("");
		mTextFieldControl->doModal();
	}

	void SettingsUpdateResourcesControl::notifyClickResourcePathDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResources->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResources->removeItemAt(index);
	}

	void SettingsUpdateResourcesControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mTextFieldControl->endModal();

		if (_result)
		{
			if (mTextFieldControl->getTextField() != "")
				mResources->addItem(mTextFieldControl->getTextField());
		}
	}

} // namespace tools
