/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsUpdateResourcesControl.h"
#include "SettingsManager.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsUpdateResourcesControl)

	SettingsUpdateResourcesControl::SettingsUpdateResourcesControl() :
		mResourceAdd(nullptr),
		mResourceDelete(nullptr),
		mResources(nullptr),
		mTextFieldControl(nullptr)
	{
	}

	SettingsUpdateResourcesControl::~SettingsUpdateResourcesControl()
	{
		mResourceAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickAdd);
		mResourceDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickDelete);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void SettingsUpdateResourcesControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mResourceAdd, "ResourceAdd");
		assignWidget(mResourceDelete, "ResourceDelete");
		assignWidget(mResources, "Resources");

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->Initialise();
		mTextFieldControl->eventEndDialog.connect(this, &SettingsUpdateResourcesControl::notifyEndDialog);

		mResourceAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickAdd);
		mResourceDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsUpdateResourcesControl::notifyClickDelete);
	}

	void SettingsUpdateResourcesControl::loadSettings()
	{
		mResources->removeAllItems();
		SettingsManager::VectorString paths = SettingsManager::getInstance().getValueList("Resources/UpdateResource.List");
		for (SettingsManager::VectorString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResources->addItem(*item);
	}

	void SettingsUpdateResourcesControl::saveSettings()
	{
		SettingsManager::VectorString paths;
		for (size_t index = 0; index < mResources->getItemCount(); ++ index)
			paths.push_back(mResources->getItemNameAt(index));
		SettingsManager::getInstance().setValueList("Resources/UpdateResource.List", paths);
	}

	void SettingsUpdateResourcesControl::notifyClickAdd(MyGUI::Widget* _sender)
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

	void SettingsUpdateResourcesControl::notifyClickDelete(MyGUI::Widget* _sender)
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
			{
				mResources->setItemNameAt(mResources->getItemCount() - 1, mTextFieldControl->getTextField());
				return;
			}
		}

		mResources->removeItemAt(mResources->getItemCount() - 1);
	}

	void SettingsUpdateResourcesControl::OnCommand(const std::string& _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
