/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionCreateData.h"
#include "DataInfoManager.h"
#include "DataManager.h"

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
			mData->setType(DataInfoManager::getInstance().getData("ResourceImageSet"));
			mData->setPropertyValue("Name", mName);
		}

		mParent->addChild(mData);

		DataManager::getInstance().invalidateDatas();

		mComplete = true;
	}

	void ActionCreateData::undoAction()
	{
		mParent->removeChild(mData);

		DataManager::getInstance().invalidateDatas();

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
