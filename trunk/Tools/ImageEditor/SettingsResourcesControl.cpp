/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsResourcesControl.h"
#include "SettingsManager.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsResourcesControl)

	SettingsResourcesControl::SettingsResourcesControl() :
		mResourceAdd(nullptr),
		mResourceDelete(nullptr),
		mResources(nullptr),
		mTextFieldControl(nullptr)
	{
	}

	SettingsResourcesControl::~SettingsResourcesControl()
	{
		mResourceAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickAdd);
		mResourceDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickDelete);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void SettingsResourcesControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mResourceAdd, "ResourceAdd");
		assignWidget(mResourceDelete, "ResourceDelete");
		assignWidget(mResources, "Resources");

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->Initialise();
		mTextFieldControl->eventEndDialog = MyGUI::newDelegate(this, &SettingsResourcesControl::notifyEndDialog);

		mResourceAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickAdd);
		mResourceDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcesControl::notifyClickDelete);
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

	void SettingsResourcesControl::notifyClickAdd(MyGUI::Widget* _sender)
	{
		mResources->addItem("");
		mResources->beginToItemAt(mResources->getItemCount() - 1);

		mTextFieldControl->setCaption(replaceTags("CaptionAddResource"));
		mTextFieldControl->setTextField("");

		MyGUI::Widget* widget = mResources->getWidgetByIndex(mResources->getItemCount() - 1);
		if (widget != nullptr)
			mTextFieldControl->setCoord(MyGUI::IntCoord(widget->getAbsoluteLeft(), widget->getAbsoluteTop(), widget->getWidth(), widget->getHeight()));

		mTextFieldControl->doModal();
	}

	void SettingsResourcesControl::notifyClickDelete(MyGUI::Widget* _sender)
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
			{
				mResources->setItemNameAt(mResources->getItemCount() - 1, mTextFieldControl->getTextField());
				return;
			}
		}

		mResources->removeItemAt(mResources->getItemCount() - 1);
	}

	void SettingsResourcesControl::OnCommand(const std::string& _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
