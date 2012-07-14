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

	void Data::setType(DataInfo* _value)
	{
		mType = _value;

		mProperties.clear();

		if (mType != nullptr)
		{
			const DataInfo::VectorProperty& properties = mType->getProperties();
			for (DataInfo::VectorProperty::const_iterator property = properties.begin(); property != properties.end(); property++)
				mProperties[(*property)->getName()] = "";
		}
	}

	DataInfo* Data::getType()
	{
		return mType;
	}

	Data* Data::getParent()
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
			Data* child = mChilds.back();
			removeChild(child);
			delete child;
		}
	}

	const Data::MapString Data::getProperties() const
	{
		return mProperties;
	}

	void Data::insertChild(size_t _index, Data* _child)
	{
		MYGUI_ASSERT(_child != nullptr, "Child is nullptr");
		MYGUI_ASSERT(_child->getParent() == nullptr, "Child already attached");

		MYGUI_ASSERT_RANGE_INSERT(_index, mChilds.size(), "Data::insertChild");

		if (_index == MyGUI::ITEM_NONE)
			_index = mChilds.size();

		mChilds.insert(mChilds.begin() + _index, _child);
		_child->mParent = this;
	}

	void Data::addChild(Data* _child)
	{
		insertChild(MyGUI::ITEM_NONE, _child);
	}
	
	void Data::removeChild(Data* _child)
	{
		MYGUI_ASSERT(_child->getParent() == this, "Child not found");

		if (_child == getChildSelected())
			mIndexSelected = MyGUI::ITEM_NONE;

		mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), _child), mChilds.end());
		_child->mParent = nullptr;
	}

	const std::string& Data::getPropertyValue(const std::string& _name) const
	{
		MapString::const_iterator property = mProperties.find(_name);
		MYGUI_ASSERT(property != mProperties.end(), "Property " << _name << " not found");

		return (*property).second;
	}
	
	void Data::setPropertyValue(const std::string& _name, const std::string& _value)
	{
		MapString::iterator property = mProperties.find(_name);
		MYGUI_ASSERT(property != mProperties.end(), "Property " << _name << " not found");

		mProperties[_name] = _value;
	}

	size_t Data::getChildIndex(Data* _child)
	{
		if (_child == nullptr)
			return MyGUI::ITEM_NONE;

		for (size_t index = 0; index < mChilds.size(); index ++)
		{
			if (_child == mChilds[index])
				return index;
		}

		MYGUI_ASSERT(false, "Child data not found");
		return MyGUI::ITEM_NONE;
	}

	Data* Data::getChildByIndex(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mChilds.size(), "Data::getChildSelected");

		if (_index == MyGUI::ITEM_NONE)
			return nullptr;
		return mChilds[_index];
	}

	Data* Data::getChildSelected()
	{
		return getChildByIndex(mIndexSelected);
	}

	void Data::setChildSelected(Data* _child)
	{
		mIndexSelected = getChildIndex(_child);
	}
}
