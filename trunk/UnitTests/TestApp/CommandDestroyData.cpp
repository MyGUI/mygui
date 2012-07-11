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
		mData(nullptr),
		mComplete(false)
	{
	}

	CommandDestroyData::~CommandDestroyData()
	{
		if (mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void CommandDestroyData::doCommand()
	{
		DataManager::getInstance().getRoot()->removeChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = true;
	}

	void CommandDestroyData::undoCommand()
	{
		DataManager::getInstance().getRoot()->addChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = false;
	}

	void CommandDestroyData::setData(Data* _data)
	{
		mData = _data;
	}
}
