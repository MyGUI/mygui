/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "CommandDestroyData.h"
#include "DataManager.h"

namespace tools
{
	CommandDestroyData::CommandDestroyData() :
		mData(nullptr)
	{
	}

	CommandDestroyData::~CommandDestroyData()
	{
		if (mData->getParent() == nullptr)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void CommandDestroyData::doCommand()
	{
		DataManager::getInstance().getRoot()->RemoveChild(mData);

		DataManager::getInstance().invalidateDatas();
	}

	void CommandDestroyData::undoCommand()
	{
		DataManager::getInstance().getRoot()->AddChild(mData);

		DataManager::getInstance().invalidateDatas();
	}

	void CommandDestroyData::setData(Data* _data)
	{
		mData = _data;
	}
}
