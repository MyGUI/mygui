/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataUtility.h"
#include "DataTypeManager.h"

namespace tools
{

	Data* DataUtility::getSelectedDataByType(const std::string& _type)
	{
		DataType* info = DataTypeManager::getInstance().getType(_type);

		return getSelectedDataByType(DataManager::getInstance().getRoot(), info);
	}

	Data* DataUtility::getSelectedDataByType(Data* _data, DataType* _info)
	{
		if (_data == nullptr)
			return nullptr;

		if (_data->getType() == _info)
			return _data;

		return getSelectedDataByType(_data->getChildSelected(), _info);
	}

	Data* DataUtility::getSelectedParentDataByType(const std::string& _type)
	{
		DataType* info = DataTypeManager::getInstance().getType(_type);

		return getSelectedParentDataByType(DataManager::getInstance().getRoot(), info);
	}

	Data* DataUtility::getSelectedParentDataByType(Data* _data, DataType* _info)
	{
		if (_data == nullptr)
			return nullptr;

		for (DataType::VectorString::const_iterator child = _data->getType()->getChilds().begin(); child != _data->getType()->getChilds().end(); child ++)
		{
			if ((*child) == _info->getName())
				return _data;
		}

		return getSelectedParentDataByType(_data->getChildSelected(), _info);
	}

}
