/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsResourcePathsControl.h"
#include "SettingsManager.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsResourcePathsControl)

	SettingsResourcePathsControl::~SettingsResourcePathsControl()
	{
		mResourcePathAdd->eventMouseButtonClick -=
			MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickAdd);
		mResourcePathDelete->eventMouseButtonClick -=
			MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickDelete);

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;
	}

	void SettingsResourcePathsControl::OnInitialise(
		Control* _parent,
		MyGUI::Widget* _place,
		std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mResourcePathAdd, "ResourcePathAdd");
		assignWidget(mResourcePathDelete, "ResourcePathDelete");
		assignWidget(mResourcePaths, "ResourcePaths");

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->Initialise(
			SettingsManager::getInstance().getValue("EditorState/OpenSaveFileDialogLayout"));
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFolder"), replaceTags("ButtonOpenFolder"), true);
		mOpenSaveFileDialog->eventEndDialog.connect(this, &SettingsResourcePathsControl::notifyEndDialogOpenSaveFile);

		mResourcePathAdd->eventMouseButtonClick +=
			MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickAdd);
		mResourcePathDelete->eventMouseButtonClick +=
			MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickDelete);
	}

	void SettingsResourcePathsControl::loadSettings()
	{
		mResourcePaths->removeAllItems();
		SettingsManager::VectorString paths =
			SettingsManager::getInstance().getValueList("Resources/AdditionalPath.List");
		for (const auto& path : paths)
			mResourcePaths->addItem(path);
	}

	void SettingsResourcePathsControl::saveSettings()
	{
		SettingsManager::VectorString paths;
		for (size_t index = 0; index < mResourcePaths->getItemCount(); ++index)
			paths.push_back(mResourcePaths->getItemNameAt(index));
		SettingsManager::getInstance().setValueList("Resources/AdditionalPath.List", paths);
	}

	void SettingsResourcePathsControl::notifyClickAdd(MyGUI::Widget* _sender)
	{
		mOpenSaveFileDialog->doModal();
	}

	void SettingsResourcePathsControl::notifyClickDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResourcePaths->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResourcePaths->removeItemAt(index);
	}

	void SettingsResourcePathsControl::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
			mResourcePaths->addItem(mOpenSaveFileDialog->getCurrentFolder());

		mOpenSaveFileDialog->endModal();
	}

	void SettingsResourcePathsControl::OnCommand(std::string_view _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
