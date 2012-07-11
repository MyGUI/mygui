/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "CommandCreateData.h"
#include "DataManager.h"
#include "DataInfoManager.h"

namespace tools
{
	CommandCreateData::CommandCreateData() :
		mData(nullptr),
		mComplete(false)
	{
	}

	CommandCreateData::~CommandCreateData()
	{
		if (mData != nullptr && !mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void CommandCreateData::doCommand()
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

	void CommandCreateData::undoCommand()
	{
		DataManager::getInstance().getRoot()->removeChild(mData);
		DataManager::getInstance().invalidateDatas();

		mComplete = false;
	}

	void CommandCreateData::setName(const std::string& _value)
	{
		mName = _value;
	}
}
