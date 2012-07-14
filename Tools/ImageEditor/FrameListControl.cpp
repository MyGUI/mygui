/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "FrameListControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(FrameListControl);

	FrameListControl::FrameListControl()
	{
	}

	FrameListControl::~FrameListControl()
	{
	}

	void FrameListControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::Initialise(_parent, _place, _layoutName);

		CommandManager::getInstance().registerCommand("Command_CreateFrameData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandCreateImageData));
		CommandManager::getInstance().registerCommand("Command_DestroyFrameData", MyGUI::newDelegate((DataListBaseControl*)this, &DataListBaseControl::commandDestroyImageData));

		setDataInfo("Index", "Frame", "Point");
	}
}
