/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
#include "SettingsResourcesControl.h"
#include "SettingsManager.h"
#include "Localise.h"

namespace tools
{
	SettingsResourcesControl::SettingsResourcesControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsResourcesControl.layout", _parent),
		mResourceAdd(nullptr),
		mResourceDelete(nullptr),
		mResources(nullptr),
		mTextFieldControl(nullptr)
	{
		assignWidget(mResourceAdd, "ResourceAdd");
		assignWidget(mResourceDelete, "ResourceDelete");
		assignWidget(mResources, "Resources");

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->eventEndDialog.connect(this, &SettingsResourcesControl::notifyEndDialog);

		mResourceAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathDelete);
	}

	SettingsResourcesControl::~SettingsResourcesControl()
	{
		mResourceAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathAdd);
		mResourceDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickResourcePathDelete);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void SettingsResourcesControl::loadSettings()
	{
		mResources->removeAllItems();
		SettingsManager::VectorString paths = SettingsManager::getInstance().getValueList("Resources/AdditionalResource.List");
		for (SettingsManager::VectorString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResources->addItem(*item);
	}

	void SettingsResourcesControl::saveSettings()
	{
		SettingsManager::VectorString paths;
		for (size_t index = 0; index < mResources->getItemCount(); ++ index)
			paths.push_back(mResources->getItemNameAt(index));

		SettingsManager::getInstance().setValueList("Resources/AdditionalResource.List", paths);
	}

	void SettingsResourcesControl::notifyClickResourcePathAdd(MyGUI::Widget* _sender)
	{
		mTextFieldControl->setCaption(replaceTags("CaptionAddResource"));
		mTextFieldControl->setTextField("");
		mTextFieldControl->doModal();
	}

	void SettingsResourcesControl::notifyClickResourcePathDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResources->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResources->removeItemAt(index);
	}

	void SettingsResourcesControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mTextFieldControl->endModal();

		if (_result)
		{
			if (mTextFieldControl->getTextField() != "")
				mResources->addItem(mTextFieldControl->getTextField());
		}
	}

} // namespace tools
