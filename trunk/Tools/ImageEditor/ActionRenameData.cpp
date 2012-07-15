/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionRenameData.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

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

		// FIXME тут нужно обновлять через систему свойств
		DataSelectorManager::getInstance().changeParent(mData->getParent());
	}

	void ActionRenameData::undoAction()
	{
		mData->setPropertyValue("Name", mOldName);

		// FIXME тут нужно обновлять через систему свойств
		DataSelectorManager::getInstance().changeParent(mData->getParent());
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
