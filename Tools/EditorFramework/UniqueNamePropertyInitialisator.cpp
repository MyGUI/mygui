/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "UniqueNamePropertyInitialisator.h"
#include "DataManager.h"
#include "DataUtility.h"

namespace tools
{

	UniqueNamePropertyInitialisator::UniqueNamePropertyInitialisator()
	{
	}

	UniqueNamePropertyInitialisator::~UniqueNamePropertyInitialisator()
	{
	}

	void UniqueNamePropertyInitialisator::initialise(PropertyPtr _property)
	{
		DataPtr parent = DataUtility::getSelectedParentDataByType(_property->getOwner()->getType()->getName());
		if (parent == nullptr)
			return;

		std::string name = DataUtility::getUniqueName(parent, "unnamed_");
		_property->setValue(name);
	}

}
