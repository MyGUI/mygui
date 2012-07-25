/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyUtility.h"
#include "FactoryManager.h"
#include "ActionManager.h"
#include "ActionChangeDataProperty.h"
#include "DataManager.h"

namespace tools
{

	bool PropertyUtility::isUniqueName(DataPtr _data, const std::string& _propertyName)
	{
		DataPtr parent = _data->getParent();
		std::string name = _data->getPropertyValue(_propertyName);

		const Data::VectorData& childs = parent->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			if ((*child)->getPropertyValue(_propertyName) == name && (*child) != _data)
				return false;
		}

		return true;
	}

	void PropertyUtility::executeAction(PropertyPtr _property, const std::string& _value, bool _merge)
	{
		if (_property->getValue() == _value)
			return;

		std::string actionName = _property->getType()->getAction();

		ActionChangeDataProperty* action = components::FactoryManager::GetInstance().CreateItem<ActionChangeDataProperty>(actionName);
		if (action != nullptr)
		{
			action->setProperty(_property);
			action->setValue(_value);
			action->setMerge(_merge);

			ActionManager::getInstance().doAction(action);
		}
	}

	void PropertyUtility::storeUniqueNameProperty(const std::string& _propertyName, const std::string& _propertyUnique, DataPtr _parent, VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _parent->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = isUniqueName((*child), _propertyName);
			PropertyPtr property = (*child)->getProperty(_propertyUnique);

			if (property->getValue<bool>() != unique)
			{
				_store.push_back(std::make_pair(property, property->getValue()));
				property->setValue(unique);
			}
		}
	}

	void PropertyUtility::restoreUniqueNameProperty(VectorPairProperty& _store)
	{
		for (VectorPairProperty::iterator value = _store.begin(); value != _store.end(); value ++)
			(*value).first->setValue((*value).second);
		_store.clear();
	}

	PropertyPtr PropertyUtility::getPropertyByName(DataPtr _data, const std::string& _dataType, const std::string& _propertyName)
	{
		if (_data == nullptr)
			return nullptr;

		if (_data->getType()->getName() == _dataType)
			return _data->getProperty(_propertyName);

		return getPropertyByName(_data->getChildSelected(), _dataType, _propertyName);
	}

	PropertyPtr PropertyUtility::getPropertyByName(const std::string& _dataType, const std::string& _propertyName)
	{
		return getPropertyByName(DataManager::getInstance().getRoot(), _dataType, _propertyName);
	}

	bool PropertyUtility::isDataSelected(DataPtr _data)
	{
		return isDataSelected(DataManager::getInstance().getRoot(), _data);
	}

	bool PropertyUtility::isDataSelected(DataPtr _parent, DataPtr _data)
	{
		if (_parent == nullptr)
			return false;

		if (_parent == _data)
			return true;

		return isDataSelected(_parent->getChildSelected(), _data);
	}

}
