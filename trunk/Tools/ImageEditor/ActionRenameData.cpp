/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionRenameData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ActionRenameData)

	ActionRenameData::ActionRenameData()
	{
	}

	ActionRenameData::~ActionRenameData()
	{
	}

	void ActionRenameData::doAction()
	{
		mOldName = getProperty()->getValue();
		getProperty()->setValue(getValue());
	}

	void ActionRenameData::undoAction()
	{
		getProperty()->setValue(mOldName);
	}

}
