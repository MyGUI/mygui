/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FrameListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FrameListControl)

	FrameListControl::FrameListControl()
	{
	}

	FrameListControl::~FrameListControl()
	{
	}

	void FrameListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().getEvent("Command_CreateFrameData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCreateData);
		CommandManager::getInstance().getEvent("Command_CloneFrameData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCloneData);
		CommandManager::getInstance().getEvent("Command_DestroyFrameData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandDestroyData);

		setDataInfo("Index", "Frame", "Point", "");
	}

}
