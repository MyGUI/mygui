/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsGeneralControl.h"
#include "SettingsManager.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsGeneralControl)

	SettingsGeneralControl::SettingsGeneralControl() :
		mGridStep(0),
		mGridEdit(nullptr),
		mSaveLastTexture(nullptr),
		mInterfaceLanguage(nullptr)
	{
	}

	SettingsGeneralControl::~SettingsGeneralControl()
	{
		mSaveLastTexture->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonClick);
		mGridEdit->eventEditSelectAccept -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
	}

	void SettingsGeneralControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mSaveLastTexture, "SaveLastTexture");
		assignWidget(mInterfaceLanguage, "InterfaceLanguage");

		mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
		mSaveLastTexture->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonClick);
	}

	void SettingsGeneralControl::loadSettings()
	{
		mGridStep = SettingsManager::getInstance().getValue<int>("Settings/GridStep");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
		mSaveLastTexture->setStateSelected(SettingsManager::getInstance().getValue<bool>("Settings/SaveLastTexture"));
		setLanguageValue(SettingsManager::getInstance().getValue("Settings/InterfaceLanguage"));
	}

	void SettingsGeneralControl::saveSettings()
	{
		SettingsManager::getInstance().setValue("Settings/GridStep", mGridStep);
		SettingsManager::getInstance().setValue("Settings/SaveLastTexture", mSaveLastTexture->getStateSelected());
		SettingsManager::getInstance().setValue("Settings/InterfaceLanguage", getLanguageValue());
	}

	void SettingsGeneralControl::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mGridStep = MyGUI::utility::parseInt(mGridEdit->getOnlyText());
		mGridStep = (std::max)(1, mGridStep);
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
	}

	void SettingsGeneralControl::notifyNewGridStepAccept(MyGUI::EditBox* _sender)
	{
		notifyNewGridStep(_sender);
	}

	void SettingsGeneralControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>(false);
		if (button != nullptr)
			button->setStateSelected(!button->getStateSelected());
	}

	void SettingsGeneralControl::setLanguageValue(const std::string& _value)
	{
		for (size_t index = 0; index < mInterfaceLanguage->getItemCount(); index ++)
		{
			if (mInterfaceLanguage->getItemNameAt(index) == _value)
			{
				mInterfaceLanguage->setIndexSelected(index);
				return;
			}
		}
		for (size_t index = 0; index < mInterfaceLanguage->getItemCount(); index ++)
		{
			if (mInterfaceLanguage->getItemNameAt(index) == "Auto")
			{
				mInterfaceLanguage->setIndexSelected(index);
				return;
			}
		}
	}

	std::string SettingsGeneralControl::getLanguageValue()
	{
		if (mInterfaceLanguage->getIndexSelected() == MyGUI::ITEM_NONE)
			return "Auto";
		return mInterfaceLanguage->getItemNameAt(mInterfaceLanguage->getIndexSelected());
	}

	void SettingsGeneralControl::OnCommand(const std::string& _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
