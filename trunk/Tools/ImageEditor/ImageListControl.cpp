/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ImageListControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "Tools/DialogManager.h"
#include "MessageBoxManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ImageListControl);

	ImageListControl::ImageListControl()
	{
		CommandManager::getInstance().registerCommand("Command_CreateData", MyGUI::newDelegate(this, &ImageListControl::commandCreateData));
		CommandManager::getInstance().registerCommand("Command_DestroyData", MyGUI::newDelegate(this, &ImageListControl::commandDestroyData));
		CommandManager::getInstance().registerCommand("Command_RenameData", MyGUI::newDelegate(this, &ImageListControl::commandRenameData));
	}

	ImageListControl::~ImageListControl()
	{
	}

	bool ImageListControl::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

	void ImageListControl::commandCreateData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		_result = true;
	}

	void ImageListControl::commandDestroyData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		_result = true;
	}

	void ImageListControl::commandRenameData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		_result = true;
	}
}
