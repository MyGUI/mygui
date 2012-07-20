/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionCloneData.h"
#include "DataTypeManager.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "PropertyUtility.h"
#include "DataUtility.h"

namespace tools
{

	ActionCloneData::ActionCloneData() :
		mData(nullptr),
		mParent(nullptr),
		mPrototype(nullptr),
		mComplete(false)
	{
	}

	ActionCloneData::~ActionCloneData()
	{
		if (mData != nullptr && !mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void ActionCloneData::doAction()
	{
		if (mData == nullptr)
		{
			mData = new Data();
			mData->setType(DataTypeManager::getInstance().getType(mType));

			DataUtility::cloneData(mData, mPrototype);
		}

		mParent->addChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = true;

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void ActionCloneData::undoAction()
	{
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = false;

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void ActionCloneData::setPrototype(Data* _prototype)
	{
		mPrototype = _prototype;
		mParent = _prototype->getParent();
		mType = _prototype->getType()->getName();
	}

	void ActionCloneData::setUniqueProperty(const std::string& _value)
	{
		mUniqueProperty = _value;
	}

}
