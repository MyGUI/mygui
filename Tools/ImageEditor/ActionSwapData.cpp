/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionSwapData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

namespace tools
{
	ActionSwapData::ActionSwapData() :
		mData1(nullptr),
		mData2(nullptr),
		mIndex(MyGUI::ITEM_NONE)
	{
	}

	ActionSwapData::~ActionSwapData()
	{
	}

	void ActionSwapData::doAction()
	{
		if (mData1 != mData2 && mData1->getParent() == mData2->getParent())
		{
			Data* parent = mData1->getParent();
			mIndex = parent->getChildIndex(mData1);
			size_t index2 = parent->getChildIndex(mData2);

			parent->removeChild(mData1);
			parent->insertChild(index2, mData1);

			DataSelectorManager::getInstance().changeParent(parent);
		}

		//DataManager::getInstance().invalidateDatas();
	}

	void ActionSwapData::undoAction()
	{
		if (mIndex != MyGUI::ITEM_NONE)
		{
			Data* parent = mData1->getParent();

			parent->removeChild(mData1);
			parent->insertChild(mIndex, mData1);

			DataSelectorManager::getInstance().changeParent(parent);
		}

		//DataManager::getInstance().invalidateDatas();
	}

	void ActionSwapData::setData1(Data* _data)
	{
		mData1 = _data;
	}

	void ActionSwapData::setData2(Data* _data)
	{
		mData2 = _data;
	}
}
