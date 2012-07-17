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

		Data* paremt = getProperty()->getOwner()->getParent();
		const Data::VectorData& childs = paremt->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = PropertyUtility::isUniqueName((*child), "Name");
			std::string value = unique ? "True" : "False";
			Property* property = (*child)->getProperties().find("UniqueName")->second;

			if (property->getValue() != value)
			{
				mOldValues.push_back(std::make_pair(property, property->getValue()));
				property->setValue(value);
			}
		}
	}

	void ActionRenameData::undoAction()
	{
		for (VectorPairProperty::const_iterator value = mOldValues.begin(); value != mOldValues.end(); value ++)
			(*value).first->setValue((*value).second);
		mOldValues.clear();
	}

}
