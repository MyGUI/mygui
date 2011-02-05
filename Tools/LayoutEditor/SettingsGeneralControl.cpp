/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
#include "SettingsGeneralControl.h"
#include "SettingsManager.h"
#include "BackwardCompatibilityManager.h"

namespace tools
{
	SettingsGeneralControl::SettingsGeneralControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SettingsGeneralControl.layout", _parent),
		mGridStep(0),
		mGridEdit(nullptr),
		//mLayoutVersion(nullptr),
		mLoadLastProject(nullptr),
		mWorkspaceSize(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		//assignWidget(mLayoutVersion, "LayoutVersion");
		assignWidget(mLoadLastProject, "LoadLastProject");
		assignWidget(mWorkspaceSize, "WorkspaceSize");

		mGridEdit->eventEditSelectAccept += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
		mLoadLastProject->eventMouseButtonPressed += MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonPressed);
	}

	SettingsGeneralControl::~SettingsGeneralControl()
	{
		mLoadLastProject->eventMouseButtonPressed -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyMouseButtonPressed);
		mGridEdit->eventEditSelectAccept -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStepAccept);
		mGridEdit->eventKeyLostFocus -= MyGUI::newDelegate(this, &SettingsGeneralControl::notifyNewGridStep);
	}

	void SettingsGeneralControl::loadSettings()
	{
		/*mLayoutVersion->removeAllItems();
		MyGUI::VectorString versions = BackwardCompatibilityManager::getInstancePtr()->getVersions();
		for (size_t index = 0; index < versions.size(); ++ index)
		{
			mLayoutVersion->addItem(versions[index]);
			if (BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion() == versions[index])
				mLayoutVersion->setIndexSelected(index);
		}*/

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
		mLoadLastProject->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("LoadLastProject"));
		mWorkspaceSize->setCaption(SettingsManager::getInstance().getSector("Workspace")->getPropertyValue("TextureSize"));
	}

	void SettingsGeneralControl::saveSettings()
	{
		/*if (mLayoutVersion->getIndexSelected() != MyGUI::ITEM_NONE)
		{
			std::string versionName = mLayoutVersion->getItemNameAt(mLayoutVersion->getIndexSelected());
			if (versionName != BackwardCompatibilityManager::getInstancePtr()->getCurrentVersion())
				BackwardCompatibilityManager::getInstancePtr()->setCurrentVersion(versionName);
		}*/

		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("LoadLastProject", mLoadLastProject->getStateSelected());

		MyGUI::IntSize workspaceSize = MyGUI::utility::parseValue<MyGUI::IntSize>(mWorkspaceSize->getCaption());
		//workspaceSize.set(MyGUI::Bitwise::firstPO2From(workspaceSize.width), MyGUI::Bitwise::firstPO2From(workspaceSize.height));
		workspaceSize.set(std::max(64, workspaceSize.width), std::max(64, workspaceSize.height));
		SettingsManager::getInstance().getSector("Workspace")->setPropertyValue("TextureSize", workspaceSize.print());
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

	void SettingsGeneralControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		mLoadLastProject->setStateSelected(!mLoadLastProject->getStateSelected());
	}

} // namespace tools
