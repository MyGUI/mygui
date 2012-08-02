/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionRenameData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "PropertyUtility.h"

namespace tools
{

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
