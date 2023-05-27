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

	void ActionDestroyData::doAction()
	{
		mParent = mData->getParent();
		mIndex = mParent->getChildIndex(mData);
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void ActionDestroyData::undoAction()
	{
		mParent->insertChild(mIndex, mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void ActionDestroyData::setData(DataPtr _data)
	{
		mData = _data;
	}

	void ActionDestroyData::setUniqueProperty(std::string_view _value)
	{
		mUniqueProperty = _value;
	}

}
