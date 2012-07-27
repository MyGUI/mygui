/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionChangePositionData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

namespace tools
{

	ActionChangePositionData::ActionChangePositionData() :
		mData1(nullptr),
		mData2(nullptr),
		mIndex(MyGUI::ITEM_NONE)
	{
	}

	ActionChangePositionData::~ActionChangePositionData()
	{
	}

	void ActionChangePositionData::doAction()
	{
		if (mData1 != mData2 && mData1->getParent() == mData2->getParent())
		{
			DataPtr parent = mData1->getParent();
			mIndex = parent->getChildIndex(mData1);
			size_t index2 = parent->getChildIndex(mData2);

			parent->removeChild(mData1);
			parent->insertChild(index2, mData1);

			DataSelectorManager::getInstance().changeParent(parent);
		}
	}

	void ActionChangePositionData::undoAction()
	{
		if (mIndex != MyGUI::ITEM_NONE)
		{
			DataPtr parent = mData1->getParent();

			parent->removeChild(mData1);
			parent->insertChild(mIndex, mData1);

			DataSelectorManager::getInstance().changeParent(parent);
		}
	}

	void ActionChangePositionData::setData1(DataPtr _data)
	{
		mData1 = _data;
	}

	void ActionChangePositionData::setData2(DataPtr _data)
	{
		mData2 = _data;
	}

}
