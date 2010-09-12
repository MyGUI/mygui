/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "precompiled.h"
#include "SettingsGeneralControl.h"
#include "SettingsManager.h"
#include "BackwardCompatibilityManager.h"

namespace tools
{
	SettingsGeneralControl::SettingsGeneralControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsGeneralControl.layout", _parent),
		mGridStep(0),
		mGridEdit(nullptr),
		mLayoutVersion(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mLayoutVersion, "LayoutVersion");

		mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
	}

	SettingsGeneralControl::~SettingsGeneralControl()
	{
		mGridEdit->eventEditSelectAccept -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
	}

	void SettingsGeneralControl::loadSettings()
	{
		mLayoutVersion->removeAllItems();
		MyGUI::VectorString versions = BackwardCompatibilityManager::getInstancePtr()->getVersions();
		for (size_t index = 0; index < versions.size(); ++ index)
		{
			mLayoutVersion->addItem(versions[index]);
			if (BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion() == versions[index])
				mLayoutVersion->setIndexSelected(index);
		}

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
	}

	void SettingsGeneralControl::saveSettings()
	{
		if (mLayoutVersion->getIndexSelected() != MyGUI::ITEM_NONE)
		{
			std::string versionName = mLayoutVersion->getItemNameAt(mLayoutVersion->getIndexSelected());
			if (versionName != BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion())
				BackwardCompatibilityManager::getInstancePtr()->setCurrentVersion(versionName);
		}

		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
	}

	void SettingsGeneralControl::notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mGridStep = MyGUI::utility::parseInt(mGridEdit->getOnlyText());
		mGridStep = std::max(1, mGridStep);
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
	}

	void SettingsGeneralControl::notifyNewGridStepAccept(MyGUI::Edit* _sender)
	{
		notifyNewGridStep(_sender);
	}

} // namespace tools
