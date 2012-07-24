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
		mParent(nullptr),
		mComplete(false)
	{
	}

	CreateSkinDataAction::~CreateSkinDataAction()
	{
		if (mData != nullptr && !mComplete)
		{
			delete mData;
			mData = nullptr;
		}
	}

	void CreateSkinDataAction::doAction()
	{
		if (mData == nullptr)
		{
			mData = new Data();
			mData->setType(DataTypeManager::getInstance().getType(mType));
			SkinDataUtility::CreateSkinData(mData);
			SkinDataUtility::ShowRegions(mData);
		}

		mParent->addChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = true;

		if (!mUniqueProperty.empty())
			PropertyUtility::storeUniqueNameProperty("Name", mUniqueProperty, mParent, mOldValues);
	}

	void CreateSkinDataAction::undoAction()
	{
		mParent->removeChild(mData);

		DataSelectorManager::getInstance().changeParent(mParent);

		mComplete = false;

		PropertyUtility::restoreUniqueNameProperty(mOldValues);
	}

	void CreateSkinDataAction::setParent(Data* _parent)
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
