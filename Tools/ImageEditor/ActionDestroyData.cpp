/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionDestroyData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "PropertyUtility.h"

namespace tools
{

	ActionDestroyData::ActionDestroyData() :
		mData(nullptr),
		mParent(nullptr),
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
		mParent = mData->getParent();
		mIndex = mParent->getChildIndex(mData);
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = true;

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void ActionDestroyData::undoAction()
	{
		mParent->insertChild(mIndex, mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = false;

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void ActionDestroyData::setData(Data* _data)
	{
		mData = _data;
	}

	void ActionDestroyData::setUniqueProperty(const std::string& _value)
	{
		mUniqueProperty = _value;
	}

}
