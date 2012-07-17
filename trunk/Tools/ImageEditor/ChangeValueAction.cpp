/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ChangeValueAction.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ChangeValueAction)

	ChangeValueAction::ChangeValueAction()
	{
	}

	ChangeValueAction::~ChangeValueAction()
	{
	}

	void ChangeValueAction::doAction()
	{
		mOldValue = getProperty()->getValue();
		getProperty()->setValue(getValue());
	}

	void ChangeValueAction::undoAction()
	{
		getProperty()->setValue(mOldValue);
	}

}
