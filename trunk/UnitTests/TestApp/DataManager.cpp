/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "DataManager.h"
#include "pugixml.hpp"
#include "MyGUI_DataManager.h"
#include "DataInfoManager.h"

namespace tools
{
	DataManager* DataManager::mInstance = nullptr;

	DataManager::DataManager() :
		mRoot(nullptr)
	{
		mInstance = this;
	}

	DataManager::~DataManager()
	{
		mInstance = nullptr;
	}

	DataManager& DataManager::getInstance()
	{
		return *mInstance;
	}

	DataManager* DataManager::getInstancePtr()
	{
		return mInstance;
	}

	void DataManager::initialise()
	{
		mRoot = new Data();
		mRoot->setType(DataInfoManager::getInstance().getData("Root"));
	}

	void DataManager::shutdown()
	{
		delete mRoot;
		mRoot = nullptr;
	}

	void DataManager::clear()
	{
		while (!mRoot->getChilds().empty())
		{
			Data* child = mRoot->getChilds().back();
			mRoot->removeChild(child);
			delete child;
		}
	}

	Data* DataManager::getRoot()
	{
		return mRoot;
	}
	
	void DataManager::invalidateDatas()
	{
		eventChangeData();
	}
}
