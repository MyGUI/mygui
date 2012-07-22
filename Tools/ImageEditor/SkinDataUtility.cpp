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

}
