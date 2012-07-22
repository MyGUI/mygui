/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SkinDataUtility.h"
#include "DataTypeManager.h"

namespace tools
{

	void SkinDataUtility::CreateSkinData(Data* _skinData)
	{
		CreateStates(_skinData);
		CreateSeparators(_skinData);
		CreateRegions(_skinData);
	}

	void SkinDataUtility::CreateStates(Data* _skinData)
	{
		Data* state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Disabled");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Normal");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Over");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Pressed");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Disabled");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Normal");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Over");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Pressed");
		_skinData->addChild(state);
	}

	void SkinDataUtility::CreateSeparators(Data* _skinData)
	{
		Data* separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Left");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Top");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Right");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Bottom");
		_skinData->addChild(separator);
	}

	void SkinDataUtility::CreateRegions(Data* _skinData)
	{
		Data* region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Left Top");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Top");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Top Right");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Right");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Right Bottom");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Bottom");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Left Bottom");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Left");
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", "Center");
		_skinData->addChild(region);
	}

}
