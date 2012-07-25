/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "CreateSkinDataAction.h"
#include "DataTypeManager.h"
#include "DataManager.h"
#include "DataSelectorManager.h"
#include "PropertyUtility.h"
#include "SkinDataUtility.h"

namespace tools
{

	CreateSkinDataAction::CreateSkinDataAction() :
		mData(nullptr),
		mParent(nullptr)
	{
	}

	CreateSkinDataAction::~CreateSkinDataAction()
	{
	}

	void CreateSkinDataAction::doAction()
	{
		if (mData == nullptr)
		{
			mData = Data::CreateInstance();
			mData->setType(DataTypeManager::getInstance().getType(mType));
			SkinDataUtility::CreateSkinData(mData);
			SkinDataUtility::ShowRegions(mData);
		}

		mParent->addChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void CreateSkinDataAction::undoAction()
	{
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void CreateSkinDataAction::setParent(DataPtr _parent)
	{
		mParent = _parent;
	}

	void CreateSkinDataAction::setType(const std::string& _value)
	{
		mType = _value;
	}

	void CreateSkinDataAction::setUniqueProperty(const std::string& _value)
	{
		mUniqueProperty = _value;
	}

}
