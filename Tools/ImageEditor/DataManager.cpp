/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataManager.h"
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
	
	Data* DataManager::getSelectedDataByType(const std::string& _type)
	{
		DataInfo* info = DataInfoManager::getInstance().getData(_type);

		return getSelectedDataByType(mRoot, info);
	}

	Data* DataManager::getSelectedDataByType(Data* _data, DataInfo* _info)
	{
		if (_data == nullptr)
			return nullptr;

		if (_data->getType() == _info)
			return _data;

		return getSelectedDataByType(_data->getChildSelected(), _info);
	}
}
