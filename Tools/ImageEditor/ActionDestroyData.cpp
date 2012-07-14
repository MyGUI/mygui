/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionDestroyData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

namespace tools
{
	ActionDestroyData::ActionDestroyData() :
		mData(nullptr),
		mComplete(false),
		mIndex(MyGUI::ITEM_NONE),
		mParent(nullptr)
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
		mParent = mData->getParent();
		mIndex = mParent->getChildIndex(mData);
		mParent->removeChild(mData);

		//DataManager::getInstance().invalidateDatas();
		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = true;
	}

	void ActionDestroyData::undoAction()
	{
		mParent->insertChild(mIndex, mData);

		//DataManager::getInstance().invalidateDatas();
		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = false;
	}

	void ActionDestroyData::setData(Data* _data)
	{
		mData = _data;
	}
}
