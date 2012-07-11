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
			RemoveChild(child);
			child = nullptr;
		}
	}

	const Data::MapString Data::getProperties() const
	{
		return mProperties;
	}

	void Data::AddChild(Data* _child)
	{
		MYGUI_ASSERT(_child != nullptr, "Child is nullptr");
		MYGUI_ASSERT(_child->getParent() == nullptr, "Child already attached");

		mChilds.push_back(_child);
		_child->mParent = this;
	}
	
	void Data::RemoveChild(Data* _child)
	{
		MYGUI_ASSERT(_child->getParent() == this, "Child not found");

		mChilds.erase(std::remove(mChilds.begin(), mChilds.end(), _child), mChilds.end());
		_child->mParent = nullptr;
	}
}
