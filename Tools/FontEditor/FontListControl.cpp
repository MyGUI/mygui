/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FontListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FontListControl)

	FontListControl::FontListControl()
	{
	}

	FontListControl::~FontListControl()
	{
	}

	void FontListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		DataListBaseControl::OnInitialise(_parent, _place, _layoutName);

		CommandManager::getInstance().getEvent("Command_CreateFontData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCreateData);
		CommandManager::getInstance().getEvent("Command_CloneFontData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandCloneData);
		CommandManager::getInstance().getEvent("Command_DestroyFontData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandDestroyData);
		CommandManager::getInstance().getEvent("Command_RenameFontData")->connect(static_cast<DataListBaseControl*>(this), &DataListBaseControl::commandRenameData);

		setDataInfo("Root", "Font", "Name", "UniqueName");
	}

}
