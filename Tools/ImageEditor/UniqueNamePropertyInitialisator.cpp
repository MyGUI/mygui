/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "UniqueNamePropertyInitialisator.h"
#include "FactoryManager.h"
#include "DataManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(UniqueNamePropertyInitialisator)

	UniqueNamePropertyInitialisator::UniqueNamePropertyInitialisator()
	{
	}

	UniqueNamePropertyInitialisator::~UniqueNamePropertyInitialisator()
	{
	}

	void UniqueNamePropertyInitialisator::initialise(Property* _property)
	{
		Data* parent = DataUtility::getSelectedParentDataByType(_property->getOwner()->getType()->getName());
		if (parent == nullptr)
			return;

		for (size_t index = 1; index < (std::numeric_limits<size_t>::max)(); index ++)
		{
			std::string name = MyGUI::utility::toString("unnamed_", index);
			bool unique = checkUniqueName(parent, name);
			if (unique)
			{
				_property->setValue(name);
				return;
			}
		}

		_property->setValue("unnamed");
	}

	bool UniqueNamePropertyInitialisator::checkUniqueName(Data* _parent, const std::string& _name)
	{
		for (Data::VectorData::const_iterator child = _parent->getChilds().begin(); child != _parent->getChilds().end(); child ++)
		{
			if ((*child)->getPropertyValue("Name") == _name)
				return false;
		}

		return true;
	}

}
