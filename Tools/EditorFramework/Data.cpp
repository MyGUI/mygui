/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "Data.h"
#include "MyGUI.h"

namespace tools
{

	Data::Data() :
		mType(nullptr),
		mParent(nullptr),
		mIndexSelected(MyGUI::ITEM_NONE)
	{
	}

	Data::~Data()
	{
		clear();
	}

	void Data::setType(DataTypePtr _value)
	{
		mType = _value;

		mProperties.clear();

		if (mType != nullptr)
		{
			const DataType::VectorProperty& properties = mType->getProperties();
			for (DataType::VectorProperty::const_iterator property = properties.begin(); property != properties.end(); property++)
			{
				PropertyPtr data = Property::CreateInstance(*property, mWeakThis.lock());
				data->initialise();

				mProperties[(*property)->getName()] = data;
			}
		}
	}

	DataTypePtr Data::getType() const
	{
		return mType;
	}

	DataPtr Data::getParent()
	{
		return mParent;
	}

	const Data::VectorData& Data::getChilds() const
	{
		return mChilds;
	}

	void Data::clear()
	{
		while (!mChilds.empty())
		{
			DataPtr child = mChilds.back();
			removeChild(child);
		}
	}

	const Data::MapProperty& Data::getProperties() const
	{
		return mProperties;
	}

	void Data::insertChild(size_t _index, DataPtr _child)
	{
		MYGUI_ASSERT(_child != nullptr, "Child is nullptr");
		MYGUI_ASSERT(_child->getParent() == nullptr, "Child already attached");
		MYGUI_ASSERT(_child->getType() != nullptr, "Type not found");
		MYGUI_ASSERT(getType() != nullptr, "Type not found");
		MYGUI_ASSERT(getType()->isChild(_child->getType()->getName()), "Type is not child");

		MYGUI_ASSERT_RANGE_INSERT(_index, mChilds.size(), "Data::insertChild");

		if (_index == MyGUI::ITEM_NONE)
			_index = mChilds.size();

		mChilds.insert(mChilds.begin() + _index, _child);
		_child->mParent = mWeakThis.lock();
	}

	void Data::addChild(DataPtr _child)
	{
		insertChild(MyGUI::ITEM_NONE, _child);
	}
	
	void Data::removeChild(DataPtr _child)
	{
		MYGUI_ASSERT(_child->getParent() == mWeakThis.lock(), "Child not found");

		if (_child == getChildSelected())
			mIndexSelected = MyGUI::ITEM_NONE;

		mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), _child), mChilds.end());
		_child->mParent = nullptr;
	}

	const std::string& Data::getPropertyValue(const std::string& _name) const
	{
		return getProperty(_name)->getValue();
	}
	
	void Data::setPropertyValue(const std::string& _name, const std::string& _value) const
	{
		getProperty(_name)->setValue(_value);
	}

	size_t Data::getChildIndex(DataPtr _child)
	{
		if (_child == nullptr)
			return MyGUI::ITEM_NONE;

		for (size_t index = 0; index < mChilds.size(); index ++)
		{
			if (_child == mChilds[index])
				return index;
		}

		MYGUI_ASSERT(false, "Child data not found");
	}

	DataPtr Data::getChildByIndex(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mChilds.size(), "Data::getChildSelected");

		if (_index == MyGUI::ITEM_NONE)
			return nullptr;
		return mChilds[_index];
	}

	DataPtr Data::getChildSelected()
	{
		return getChildByIndex(mIndexSelected);
	}

	void Data::setChildSelected(DataPtr _child)
	{
		mIndexSelected = getChildIndex(_child);
	}

	PropertyPtr Data::getProperty(const std::string& _name) const
	{
		MapProperty::const_iterator property = mProperties.find(_name);
		MYGUI_ASSERT(property != mProperties.end(), "Property " << _name << " not found");

		return (*property).second;
	}

	DataPtr Data::CreateInstance()
	{
		DataPtr result = DataPtr(new Data());
		result->mWeakThis = DataWeak(result);
		return result;
	}

}
