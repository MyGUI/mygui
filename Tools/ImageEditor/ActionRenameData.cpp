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
#include "PropertyUtility.h"

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
		mOldValues.push_back(std::make_pair(getProperty(), getProperty()->getValue()));
		getProperty()->setValue(getValue());

		DataPtr parent = getProperty()->getOwner()->getParent();
		PropertyUtility::storeUniqueNameProperty("Name", "UniqueName", parent, mOldValues);
	}

	void ActionRenameData::undoAction()
	{
		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

}
