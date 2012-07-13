/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionRenameData.h"
#include "DataManager.h"

namespace tools
{
	ActionRenameData::ActionRenameData() :
		mData(nullptr)
	{
	}

	ActionRenameData::~ActionRenameData()
	{
	}

	void ActionRenameData::doAction()
	{
		mOldName = mData->getPropertyValue("Name");
		mData->setPropertyValue("Name", mName);

		DataManager::getInstance().invalidateDatas();
	}

	void ActionRenameData::undoAction()
	{
		mData->setPropertyValue("Name", mOldName);

		DataManager::getInstance().invalidateDatas();
	}

	void ActionRenameData::setData(Data* _data)
	{
		mData = _data;
	}

	void ActionRenameData::setName(const std::string& _value)
	{
		mName = _value;
	}
}
