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

	bool PropertyUtility::isUniqueName(DataPtr _data, std::string_view _propertyName)
	{
		DataPtr parent = _data->getParent();
		const std::string& name = _data->getPropertyValue(_propertyName);

		const Data::VectorData& childs = parent->getChilds();
		for (const auto& child : childs)
		{
			if (child->getPropertyValue(_propertyName) == name && child != _data)
				return false;
		}

		return true;
	}

	void PropertyUtility::executeAction(PropertyPtr _property, std::string_view _value, bool _merge)
	{
		if (_property->getValue() == _value)
			return;

		const std::string& actionName = _property->getType()->getAction();

		ActionChangeDataProperty* action =
			components::FactoryManager::GetInstance().CreateItem<ActionChangeDataProperty>(actionName);
		if (action != nullptr)
		{
			action->setProperty(_property);
			action->setValue(_value);
			action->setMerge(_merge);

			ActionManager::getInstance().doAction(action);
		}
	}

	void PropertyUtility::storeUniqueNameProperty(
		std::string_view _propertyName,
		std::string_view _propertyUnique,
		DataPtr _parent,
		VectorPairProperty& _store)
	{
		const Data::VectorData& childs = _parent->getChilds();
		for (const auto& child : childs)
		{
			bool unique = isUniqueName(child, _propertyName);
			PropertyPtr property = child->getProperty(_propertyUnique);

			if (property->getValue<bool>() != unique)
			{
				_store.emplace_back(property, property->getValue());
				property->setValue(unique);
			}
		}
	}

	void PropertyUtility::restoreUniqueNameProperty(VectorPairProperty& _store)
	{
		for (auto& value : _store)
			value.first->setValue(value.second);
		_store.clear();
	}

	PropertyPtr PropertyUtility::getPropertyByName(
		DataPtr _data,
		std::string_view _dataType,
		std::string_view _propertyName)
	{
		if (_data == nullptr)
			return nullptr;

		if (_data->getType()->getName() == _dataType)
			return _data->getProperty(_propertyName);

		return getPropertyByName(_data->getChildSelected(), _dataType, _propertyName);
	}

	PropertyPtr PropertyUtility::getPropertyByName(std::string_view _dataType, std::string_view _propertyName)
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
