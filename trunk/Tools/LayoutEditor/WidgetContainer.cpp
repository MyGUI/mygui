/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#include "Precompiled.h"
#include "WidgetContainer.h"

namespace tools
{

	WidgetContainer::WidgetContainer() :
		mRelativeMode(false),
		mWidget(nullptr)
	{
	}

	WidgetContainer::WidgetContainer(const std::string& _type, const std::string& _skin, MyGUI::Widget* _widget, const std::string& _name):
		mRelativeMode(false),
		mType(_type),
		mSkin(_skin),
		mName(_name),
		mWidget(_widget)
	{
	}

	WidgetContainer::~WidgetContainer()
	{
		for (std::vector<WidgetContainer*>::iterator iter = childContainers.begin(); iter != childContainers.end(); ++iter)
			delete *iter;
		childContainers.clear();

		for (std::vector<ControllerInfo*>::iterator iter = mController.begin(); iter != mController.end(); ++iter)
			delete *iter;
		mController.clear();
	}

	std::string WidgetContainer::position(bool _percent)
	{
		if (mRelativeMode)
		{
			MyGUI::FloatCoord coord = MyGUI::CoordConverter::convertToRelative(mWidget->getCoord(), mWidget->getParentSize());
			std::ostringstream stream;
			if (_percent)
				stream << coord.left * 100 << " " << coord.top * 100 << " " << coord.width * 100 << " " << coord.height * 100;
			else
				stream << coord.left << " " << coord.top << " " << coord.width << " " << coord.height;
			return stream.str();
		}
		return mWidget->getCoord().print();
	}

	void WidgetContainer::setUserData(const std::string& _key, const std::string& _value)
	{
		bool found = false;

		for (MyGUI::VectorStringPairs::iterator item = mUserString.begin(); item != mUserString.end(); ++ item)
		{
			if ((*item).first == _key)
			{
				found = true;
				(*item).second = _value;
				break;
			}
		}

		if (!found)
			mUserString.push_back(MyGUI::PairString(_key, _value));
	}

	std::string WidgetContainer::getUserData(const std::string& _key)
	{
		for (MyGUI::VectorStringPairs::const_iterator item = mUserString.begin(); item != mUserString.end(); ++ item)
		{
			if ((*item).first == _key)
				return (*item).second;
		}
		return "";
	}

	void WidgetContainer::clearUserData(const std::string& _key)
	{
		for (MyGUI::VectorStringPairs::iterator item = mUserString.begin(); item != mUserString.end(); ++ item)
		{
			if ((*item).first == _key)
			{
				mUserString.erase(item);
				break;
			}
		}
	}

	bool WidgetContainer::existUserData(const std::string& _key) const
	{
		for (MyGUI::VectorStringPairs::const_iterator item = mUserString.begin(); item != mUserString.end(); ++ item)
		{
			if ((*item).first == _key)
				return true;
		}
		return false;
	}

	WidgetContainer::UserDataEnumerator WidgetContainer::getUserDataEnumerator() const
	{
		return UserDataEnumerator(mUserString);
	}

	void WidgetContainer::setProperty(const std::string& _key, const std::string& _value, bool _eraseExist)
	{
		bool found = false;

		if (_eraseExist)
		{
			for (MyGUI::VectorStringPairs::iterator item = mProperty.begin(); item != mProperty.end(); ++ item)
			{
				if ((*item).first == _key)
				{
					found = true;
					(*item).second = _value;
					break;
				}
			}
		}

		if (!found)
			mProperty.push_back(MyGUI::PairString(_key, _value));
	}

	std::string WidgetContainer::getProperty(const std::string& _key)
	{
		for (MyGUI::VectorStringPairs::const_iterator item = mProperty.begin(); item != mProperty.end(); ++ item)
		{
			if ((*item).first == _key)
				return (*item).second;
		}
		return "";
	}

	void WidgetContainer::clearProperty(const std::string& _key)
	{
		for (MyGUI::VectorStringPairs::iterator item = mProperty.begin(); item != mProperty.end(); ++ item)
		{
			if ((*item).first == _key)
			{
				mProperty.erase(item);
				break;
			}
		}
	}

	bool WidgetContainer::existProperty(const std::string& _key) const
	{
		for (MyGUI::VectorStringPairs::const_iterator item = mProperty.begin(); item != mProperty.end(); ++ item)
		{
			if ((*item).first == _key)
				return true;
		}
		return false;
	}

	WidgetContainer::PropertyEnumerator WidgetContainer::getPropertyEnumerator() const
	{
		return PropertyEnumerator(mProperty);
	}

	void WidgetContainer::setLayerName(const std::string& _value)
	{
		mLayer = _value;
	}

	const std::string& WidgetContainer::getLayerName() const
	{
		return mLayer;
	}

	MyGUI::Widget* WidgetContainer::getWidget()
	{
		return mWidget;
	}

	void WidgetContainer::setWidget(MyGUI::Widget* _value)
	{
		mWidget = _value;
	}

	const std::string& WidgetContainer::getName() const
	{
		return mName;
	}

	void WidgetContainer::setName(const std::string& _value)
	{
		mName = _value;
	}

	const std::string& WidgetContainer::getSkin() const
	{
		return mSkin;
	}

	void WidgetContainer::setSkin(const std::string& _value)
	{
		mSkin = _value;
	}

	const std::string& WidgetContainer::getType() const
	{
		return mType;
	}

	void WidgetContainer::setType(const std::string& _value)
	{
		mType = _value;
	}

	const std::string& WidgetContainer::getAlign() const
	{
		return mAlign;
	}

	void WidgetContainer::setAlign(const std::string& _value)
	{
		mAlign = _value;
	}

	const std::string& WidgetContainer::getStyle() const
	{
		return mStyle;
	}

	void WidgetContainer::setStyle(const std::string& _value)
	{
		mStyle = _value;
	}

	bool WidgetContainer::getRelativeMode() const
	{
		return mRelativeMode;
	}

	void WidgetContainer::setRelativeMode(bool _value)
	{
		mRelativeMode = _value;
	}

	void WidgetContainer::setPropertyByIndex(size_t _index, const std::string& _key, const std::string& _value)
	{
		for (MyGUI::VectorStringPairs::iterator item = mProperty.begin(); item != mProperty.end(); ++item)
		{
			if ((*item).first == _key)
			{
				if (_index == 0)
				{
					(*item).second = _value;
					break;
				}
				-- _index;
			}
		}
	}

	void WidgetContainer::clearPropertyByIndex(size_t _index, const std::string& _key)
	{
		for (MyGUI::VectorStringPairs::iterator item = mProperty.begin(); item != mProperty.end(); ++item)
		{
			if ((*item).first == _key)
			{
				if (_index == 0)
				{
					mProperty.erase(item);
					break;
				}
				-- _index;
			}
		}
	}

}
