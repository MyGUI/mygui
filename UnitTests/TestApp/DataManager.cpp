/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "DataManager.h"
#include "pugixml.hpp"
#include "MyGUI.h"

namespace tools
{
	DataManager* DataManager::mInstance = nullptr;

	DataManager::DataManager()
	{
		mInstance = this;
	}

	DataManager::~DataManager()
	{
		mInstance = nullptr;

		clearDataInfo();
	}

	DataManager& DataManager::getInstance()
	{
		return *mInstance;
	}

	DataManager* DataManager::getInstancePtr()
	{
		return mInstance;
	}

	void DataManager::loadDataInfo(const std::string& _fileName)
	{
		std::string fileName = MyGUI::DataManager::getInstance().getDataPath(_fileName);
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(fileName.c_str());
		if (result)
		{
			pugi::xpath_node_set nodes = doc.select_nodes("Document/DataInfos/DataInfo");
			for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			{
				DataInfo* data = new DataInfo();
				data->deserialization((*node).node());
				mDataInfos.push_back(data);
			}
		}
	}

	void DataManager::clearDataInfo()
	{
		for (VectorDataInfo::iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
			delete *data;
		mDataInfos.clear();
	}

	DataInfo* DataManager::getData(const std::string& _type)
	{
		for (VectorDataInfo::const_iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
		{
			if ((*data)->getType() == _type)
				return *data;
		}

		return nullptr;
	}
}
