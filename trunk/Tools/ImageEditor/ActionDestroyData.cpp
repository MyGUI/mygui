/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionDestroyData.h"
#include "DataManager.h"

namespace tools
{
	ActionDestroyData::ActionDestroyData() :
		mData(nullptr),
		mComplete(false),
		mIndex(MyGUI::ITEM_NONE)
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
		mIndex = DataManager::getInstance().getRoot()->getChildIndex(mData);
		DataManager::getInstance().getRoot()->removeChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = true;
	}

	void ActionDestroyData::undoAction()
	{
		DataManager::getInstance().getRoot()->insertChild(mIndex, mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = false;
	}

	void ActionDestroyData::setData(Data* _data)
	{
		mData = _data;
	}
}
