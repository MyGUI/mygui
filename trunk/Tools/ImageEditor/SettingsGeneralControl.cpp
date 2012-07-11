/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
#include "SettingsGeneralControl.h"
#include "SettingsManager.h"

namespace tools
{
	SettingsGeneralControl::SettingsGeneralControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsGeneralControl.layout", _parent),
		mGridStep(0),
		mGridEdit(nullptr),
		mSaveLastTexture(nullptr),
		mInterfaceLanguage(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mSaveLastTexture, "SaveLastTexture");
		assignWidget(mInterfaceLanguage, "InterfaceLanguage");

		mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
		mSaveLastTexture->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonClick);
	}

	SettingsGeneralControl::~SettingsGeneralControl()
	{
		mSaveLastTexture->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonClick);
		mGridEdit->eventEditSelectAccept -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
	}

	void SettingsGeneralControl::loadSettings()
	{
		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
		mSaveLastTexture->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("SaveLastTexture"));
		setLanguageValue(SettingsManager::getInstance().getSector("Settings")->getPropertyValue("InterfaceLanguage"));
	}

	void SettingsGeneralControl::saveSettings()
	{
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("SaveLastTexture", mSaveLastTexture->getStateSelected());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("InterfaceLanguage", getLanguageValue());
	}

	void SettingsGeneralControl::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mGridStep = MyGUI::utility::parseInt(mGridEdit->getOnlyText());
		mGridStep = std::max(1, mGridStep);
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

} // namespace tools
