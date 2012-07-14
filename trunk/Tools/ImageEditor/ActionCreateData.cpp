/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionCreateData.h"
#include "DataInfoManager.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

namespace tools
{
	ActionCreateData::ActionCreateData() :
		mData(nullptr),
		mComplete(false),
		mParent(nullptr)
	{
	}

	ActionCreateData::~ActionCreateData()
	{
		if (mData != nullptr && !mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void ActionCreateData::doAction()
	{
		if (mData == nullptr)
		{
			mData = new Data();
			mData->setType(DataInfoManager::getInstance().getData("Image"));
			mData->setPropertyValue("Name", mName);
		}

		mParent->addChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = true;
	}

	void ActionCreateData::undoAction()
	{
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = false;
	}

	void ActionCreateData::setName(const std::string& _value)
	{
		mName = _value;
	}

	void ActionCreateData::setParent(Data* _parent)
	{
		mParent = _parent;
	}
}
