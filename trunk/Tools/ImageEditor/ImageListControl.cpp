/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ImageListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(ImageListControl);

	ImageListControl::ImageListControl()
	{
	}

	ImageListControl::~ImageListControl()
	{
	}

	void ImageListControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::Initialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateImageData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandCreateImageData));
		CommandManager::getInstance().registerCommand("Command_DestroyImageData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandDestroyImageData));
		CommandManager::getInstance().registerCommand("Command_RenameImageData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandRenameImageData));

		setDataInfo("Root", "Image", "Name");
	}
}
