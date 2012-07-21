/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ChangeTextureValueAction.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "FactoryManager.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ChangeTextureValueAction)

	ChangeTextureValueAction::ChangeTextureValueAction()
	{
	}

	ChangeTextureValueAction::~ChangeTextureValueAction()
	{
	}

	void ChangeTextureValueAction::doAction()
	{
		mOldValue = getProperty()->getValue();
		getProperty()->setValue(getValue());

		if (SettingsManager::getInstance().getValue<bool>("Settings/SaveLastTexture"))
			SettingsManager::getInstance().setValue("Resources/LastTextureName", getValue());
	}

	void ChangeTextureValueAction::undoAction()
	{
		getProperty()->setValue(mOldValue);
	}

	bool ChangeTextureValueAction::doMerge(Action* _action)
	{
		ChangeTextureValueAction* action = dynamic_cast<ChangeTextureValueAction*>(_action);
		if (action != nullptr)
		{
			if (action->getProperty() == getProperty())
			{
				getProperty()->setValue(action->getValue());
				return true;
			}
		}

		return false;
	}

}
