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
		mPrototype(nullptr)
	{
	}

	ActionCloneData::~ActionCloneData()
	{
	}

	void ActionCloneData::doAction()
	{
		if (mData == nullptr)
		{
			mData = Data::CreateInstance();
			mData->setType(DataTypeManager::getInstance().getType(mType));

			DataUtility::cloneData(mData, mPrototype);

			if (!mUniqueProperty.empty())
				mData->setPropertyValue("Name", DataUtility::getUniqueName(mParent, mPrototype->getPropertyValue("Name") + "_"));
		}

		mParent->addChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void ActionCloneData::undoAction()
	{
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void ActionCloneData::setPrototype(DataPtr _prototype)
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
