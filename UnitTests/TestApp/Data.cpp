/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Data.h"
#include "MyGUI.h"

namespace tools
{
	Data::Data() :
		mType(nullptr),
		mParent(nullptr)
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

	const MyGUI::MapString Data::getProperties() const
	{
		return mProperties;
	}

	void Data::addChild(Data* _child)
	{
		MYGUI_ASSERT(_child != nullptr, "Child is nullptr");
		MYGUI_ASSERT(_child->getParent() == nullptr, "Child already attached");

		mChilds.push_back(_child);
		_child->mParent = this;
	}
	
	void Data::removeChild(Data* _child)
	{
		MYGUI_ASSERT(_child->getParent() == this, "Child not found");

		mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), _child), mChilds.end());
		_child->mParent = nullptr;
	}

	const std::string& Data::getPropertyValue(const std::string& _name) const
	{
		MyGUI::MapString::const_iterator property = mProperties.find(_name);
		MYGUI_ASSERT(property != mProperties.end(), "Property " << _name << " not found");

		return (*property).second;
	}
	
	void Data::setPropertyValue(const std::string& _name, const std::string& _value)
	{
		MyGUI::MapString::iterator property = mProperties.find(_name);
		MYGUI_ASSERT(property != mProperties.end(), "Property " << _name << " not found");

		mProperties[_name] = _value;
	}
}
