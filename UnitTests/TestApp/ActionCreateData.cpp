/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "ActionCreateData.h"
#include "DataManager.h"
#include "DataInfoManager.h"

namespace tools
{
	ActionCreateData::ActionCreateData() :
		mData(nullptr),
		mComplete(false)
	{
	}

	ActionCreateData::~ActionCreateData()
	{
		delete mData;
	}

	void ActionCreateData::doAction()
	{
		if (mData == nullptr)
		{
			mData = new Data();
			mData->setType(DataInfoManager::getInstance().getData("ResourceImageSet"));
			mData->setPropertyValue("Name", mName);
		}

		DataManager::getInstance().getRoot()->addChild(mData);
		DataManager::getInstance().invalidateDatas();

		mComplete = true;
	}

	void ActionCreateData::undoAction()
	{
		DataManager::getInstance().getRoot()->removeChild(mData);
		DataManager::getInstance().invalidateDatas();

		mComplete = false;
	}

	void ActionCreateData::setName(std::string_view _value)
	{
		mName = _value;
	}
}
