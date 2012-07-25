/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ChangeSeparatorVisibleAction.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"
#include "PropertyUtility.h"
#include "SkinDataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ChangeSeparatorVisibleAction)

	ChangeSeparatorVisibleAction::ChangeSeparatorVisibleAction()
	{
	}

	ChangeSeparatorVisibleAction::~ChangeSeparatorVisibleAction()
	{
	}

	void ChangeSeparatorVisibleAction::doAction()
	{
		storeOldValues();
		setNewValues();
	}

	void ChangeSeparatorVisibleAction::undoAction()
	{
		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void ChangeSeparatorVisibleAction::storeRegionValues(DataPtr _skinData, VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _skinData->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			if ((*child)->getType()->getName() != "Region")
				continue;

			PropertyPtr property = (*child)->getProperty("Enable");
			_store.push_back(std::make_pair(property, property->getValue()));

			property = (*child)->getProperty("Coord");
			_store.push_back(std::make_pair(property, property->getValue()));
		}
	}

	void ChangeSeparatorVisibleAction::storeOldValues()
	{
		mOldValues.push_back(std::make_pair(getProperty(), getProperty()->getValue()));

		DataPtr skinData = getProperty()->getOwner()->getParent();
		storeRegionValues(skinData, mOldValues);
	}

	void ChangeSeparatorVisibleAction::setNewValues()
	{
		getProperty()->setValue(getValue());

		DataPtr skinData = getProperty()->getOwner()->getParent();

		SkinDataUtility::RectVisible visible = SkinDataUtility::getSeparatorsVisible(skinData);
		SkinDataUtility::fillRegionEnable(skinData, visible);

		MyGUI::IntSize size = SkinDataUtility::getSkinSize(skinData);
		MyGUI::IntRect separators = SkinDataUtility::getSeparatorsOffset(skinData);
		SkinDataUtility::VectorCoord coords = SkinDataUtility::getRegions(size, separators);
		SkinDataUtility::fillRegionCoords(skinData, coords);
	}

}
