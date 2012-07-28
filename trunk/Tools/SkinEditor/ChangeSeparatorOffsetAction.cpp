/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ChangeSeparatorOffsetAction.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"
#include "PropertyUtility.h"
#include "SkinDataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ChangeSeparatorOffsetAction)

	ChangeSeparatorOffsetAction::ChangeSeparatorOffsetAction()
	{
	}

	ChangeSeparatorOffsetAction::~ChangeSeparatorOffsetAction()
	{
	}

	void ChangeSeparatorOffsetAction::doAction()
	{
		storeOldValues();
		setNewValues();
	}

	void ChangeSeparatorOffsetAction::undoAction()
	{
		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	bool ChangeSeparatorOffsetAction::doMerge(Action* _action)
	{
		ChangeSeparatorOffsetAction* action = dynamic_cast<ChangeSeparatorOffsetAction*>(_action);
		if (action != nullptr)
		{
			if (action->getProperty() == getProperty())
			{
				setValue(action->getValue());
				setNewValues();
				return true;
			}
		}

		return false;
	}

	void ChangeSeparatorOffsetAction::storeRegionValues(DataPtr _skinData, VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _skinData->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			if ((*child)->getType()->getName() != "Region")
				continue;

			PropertyPtr property = (*child)->getProperty("Coord");
			_store.push_back(std::make_pair(property, property->getValue()));
		}
	}

	void ChangeSeparatorOffsetAction::storeOldValues()
	{
		mOldValues.push_back(std::make_pair(getProperty(), getProperty()->getValue()));

		DataPtr skinData = getProperty()->getOwner()->getParent();
		storeRegionValues(skinData, mOldValues);
	}

	void ChangeSeparatorOffsetAction::setNewValues()
	{
		getProperty()->setValue(getValue());

		DataPtr skinData = getProperty()->getOwner()->getParent();

		MyGUI::IntSize size = SkinDataUtility::getSkinSize(skinData);
		MyGUI::IntRect separators = SkinDataUtility::getSeparatorsOffset(skinData);
		SkinDataUtility::VectorCoord coords = SkinDataUtility::getRegions(size, separators);
		SkinDataUtility::fillRegionCoords(skinData, coords);
	}

}
