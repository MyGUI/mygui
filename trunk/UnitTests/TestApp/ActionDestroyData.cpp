/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "ActionDestroyData.h"
#include "DataManager.h"

namespace tools
{
	ActionDestroyData::ActionDestroyData() :
		mData(nullptr),
		mComplete(false)
	{
	}

	ActionDestroyData::~ActionDestroyData()
	{
		if (mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void ActionDestroyData::doAction()
	{
		DataManager::getInstance().getRoot()->removeChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = true;
	}

	void ActionDestroyData::undoAction()
	{
		DataManager::getInstance().getRoot()->addChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = false;
	}

	void ActionDestroyData::setData(Data* _data)
	{
		mData = _data;
	}
}
