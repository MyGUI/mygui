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

	void DataUtility::cloneData(Data* _target, Data* _prototype)
	{
		MYGUI_ASSERT(_target != _prototype, "Error clone self");
		MYGUI_ASSERT(_target->getType() == _prototype->getType(), "Error clone different types");
		MYGUI_ASSERT(_target->getChilds().size() == 0, "Target not empty");

		copyProperty(_target, _prototype);
		
		for (Data::VectorData::const_iterator child = _prototype->getChilds().begin(); child != _prototype->getChilds().end(); child ++)
		{
			Data* data = new Data();
			data->setType((*child)->getType());

			_target->addChild(data);

			cloneData(data, *child);
		}
	}

	void DataUtility::copyProperty(Data* _target, Data* _prototype)
	{
		for (Data::MapString::const_iterator property = _prototype->getProperties().begin(); property != _prototype->getProperties().end(); property ++)
			_target->setPropertyValue((*property).first, (*property).second->getValue());
	}

	std::string DataUtility::getUniqueName(Data* _parent, const std::string& _pattern)
	{
		std::string result = _pattern;

		for (size_t index = 1; index < (std::numeric_limits<size_t>::max)(); index ++)
		{
			std::string name = MyGUI::utility::toString(_pattern, index);
			bool unique = checkUniqueName(_parent, name);
			if (unique)
			{
				result = name;
				break;
			}
		}

		return result;
	}

	bool DataUtility::checkUniqueName(Data* _parent, const std::string& _name)
	{
		for (Data::VectorData::const_iterator child = _parent->getChilds().begin(); child != _parent->getChilds().end(); child ++)
		{
			if ((*child)->getPropertyValue("Name") == _name)
				return false;
		}

		return true;
	}

}
