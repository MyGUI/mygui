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
		mData(nullptr)
	{
	}

	CommandCreateData::~CommandCreateData()
	{
	}

	void CommandCreateData::doCommand()
	{
		mData = new Data();
		mData->setType(DataInfoManager::getInstance().getData("ResourceImageSet"));
		mData->setPropertyValue("Name", mName);

		DataManager::getInstance().getRoot()->AddChild(mData);
		DataManager::getInstance().invalidateDatas();
	}

	void CommandCreateData::undoCommand()
	{
		DataManager::getInstance().getRoot()->RemoveChild(mData);
		DataManager::getInstance().invalidateDatas();

		delete mData;
		mData = nullptr;
	}

	void CommandCreateData::setName(const std::string& _value)
	{
		mName = _value;
	}
}
