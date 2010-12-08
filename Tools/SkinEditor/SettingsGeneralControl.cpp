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
		mSaveLastTexture(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mSaveLastTexture, "SaveLastTexture");

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
	}

	void SettingsGeneralControl::saveSettings()
	{
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("SaveLastTexture", mSaveLastTexture->getStateSelected());
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

} // namespace tools
