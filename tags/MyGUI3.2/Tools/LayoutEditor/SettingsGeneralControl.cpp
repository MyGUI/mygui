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
		mLoadLastProject(nullptr),
		mWorkspaceSize(nullptr),
		mInterfaceLanguage(nullptr)
	{
		assignWidget(mGridEdit, "gridEdit");
		assignWidget(mLoadLastProject, "LoadLastProject");
		assignWidget(mWorkspaceSize, "WorkspaceSize");
		assignWidget(mInterfaceLanguage, "InterfaceLanguage");

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
		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		mGridEdit->setCaption(MyGUI::utility::toString(mGridStep));
		mLoadLastProject->setStateSelected(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("LoadLastProject"));
		setLanguageValue(SettingsManager::getInstance().getSector("Settings")->getPropertyValue("InterfaceLanguage"));

		mWorkspaceSize->setCaption(SettingsManager::getInstance().getSector("Workspace")->getPropertyValue("TextureSize"));
	}

	void SettingsGeneralControl::saveSettings()
	{
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("Grid", mGridStep);
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("LoadLastProject", mLoadLastProject->getStateSelected());
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue("InterfaceLanguage", getLanguageValue());

		MyGUI::IntSize workspaceSize = MyGUI::utility::parseValue<MyGUI::IntSize>(mWorkspaceSize->getCaption());
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
