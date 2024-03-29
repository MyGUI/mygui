/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ChangeSkinSizeAction.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"
#include "SkinDataUtility.h"
#include "PropertyUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ChangeSkinSizeAction)

	void ChangeSkinSizeAction::doAction()
	{
		storeOldValues();
		setNewValues();
	}

	void ChangeSkinSizeAction::undoAction()
	{
		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	bool ChangeSkinSizeAction::doMerge(Action* _action)
	{
		ChangeSkinSizeAction* action = dynamic_cast<ChangeSkinSizeAction*>(_action);
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

	void ChangeSkinSizeAction::storeRegionValues(DataPtr _skinData, VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _skinData->getChilds();
		for (const auto& child : childs)
		{
			if (child->getType()->getName() != "Region")
				continue;

			PropertyPtr property = child->getProperty("Coord");
			_store.emplace_back(property, property->getValue());
		}
	}

	void ChangeSkinSizeAction::storeOldValues()
	{
		mOldValues.emplace_back(getProperty(), getProperty()->getValue());

		DataPtr skinData = getProperty()->getOwner();
		storeRegionValues(skinData, mOldValues);
	}

	void ChangeSkinSizeAction::setNewValues()
	{
		getProperty()->setValue(getValue());

		DataPtr skinData = getProperty()->getOwner();
		MyGUI::IntSize size = SkinDataUtility::getSkinSize(skinData);
		MyGUI::IntRect separators = SkinDataUtility::getSeparatorsOffset(skinData);
		SkinDataUtility::VectorCoord coords = SkinDataUtility::getRegions(size, separators);
		SkinDataUtility::fillRegionCoords(skinData, coords);
	}

}
