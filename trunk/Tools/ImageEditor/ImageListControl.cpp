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

	FACTORY_ITEM_ATTRIBUTE(ImageListControl)

	ImageListControl::ImageListControl()
	{
	}

	ImageListControl::~ImageListControl()
	{
	}

	void ImageListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().getEvent("Command_CreateImageData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCreateData);
		CommandManager::getInstance().getEvent("Command_CloneImageData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCloneData);
		CommandManager::getInstance().getEvent("Command_DestroyImageData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandDestroyData);
		CommandManager::getInstance().getEvent("Command_RenameImageData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandRenameData);

		setDataInfo("Root", "Image", "Name", "UniqueName");
	}

}
