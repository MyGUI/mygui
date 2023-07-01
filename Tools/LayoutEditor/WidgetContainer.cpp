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

	WidgetContainer::WidgetContainer(
		std::string_view _type,
		std::string_view _skin,
		MyGUI::Widget* _widget,
		std::string_view _name) :
		mRelativeMode(false),
		mType(_type),
		mSkin(_skin),
		mName(_name),
		mWidget(_widget)
	{
	}

	WidgetContainer::~WidgetContainer()
	{
		for (auto& childContainer : childContainers)
			delete childContainer;
		childContainers.clear();

		for (auto& iter : mController)
			delete iter;
		mController.clear();
	}

	std::string WidgetContainer::position(bool _percent)
	{
		if (mRelativeMode)
		{
			MyGUI::DoubleCoord coord =
				MyGUI::CoordConverter::convertToRelativeD(mWidget->getCoord(), mWidget->getParentSize());
			std::ostringstream stream;
			stream.precision(17);
			if (_percent)
				stream << coord.left * 100 << " " << coord.top * 100 << " " << coord.width * 100 << " "
					   << coord.height * 100;
			else
				stream << coord.left << " " << coord.top << " " << coord.width << " " << coord.height;
			return stream.str();
		}
		return mWidget->getCoord().print();
	}

	void WidgetContainer::setUserData(std::string_view _key, std::string_view _value)
	{
		bool found = false;

		for (auto& item : mUserString)
		{
			if (item.first == _key)
			{
				found = true;
				item.second = _value;
				break;
			}
		}

		if (!found)
			mUserString.emplace_back(_key, _value);
	}

	std::string_view WidgetContainer::getUserData(std::string_view _key)
	{
		for (const auto& item : mUserString)
		{
			if (item.first == _key)
				return item.second;
		}
		return {};
	}

	void WidgetContainer::clearUserData(std::string_view _key)
	{
		for (MyGUI::VectorStringPairs::iterator item = mUserString.begin(); item != mUserString.end(); ++item)
		{
			if ((*item).first == _key)
			{
				mUserString.erase(item);
				break;
			}
		}
	}

	bool WidgetContainer::existUserData(std::string_view _key) const
	{
		for (const auto& item : mUserString)
		{
			if (item.first == _key)
				return true;
		}
		return false;
	}

	WidgetContainer::UserDataEnumerator WidgetContainer::getUserDataEnumerator() const
	{
		return UserDataEnumerator(mUserString);
	}

	void WidgetContainer::setProperty(std::string_view _key, std::string_view _value, bool _eraseExist)
	{
		bool found = false;

		if (_eraseExist)
		{
			for (auto& item : mProperty)
			{
				if (item.first == _key)
				{
					found = true;
					item.second = _value;
					break;
				}
			}
		}

		if (!found)
			mProperty.emplace_back(_key, _value);
	}

	std::string_view WidgetContainer::getProperty(std::string_view _key)
	{
		for (const auto& item : mProperty)
		{
			if (item.first == _key)
				return item.second;
		}
		return {};
	}

	void WidgetContainer::clearProperty(std::string_view _key)
	{
		for (MyGUI::VectorStringPairs::iterator item = mProperty.begin(); item != mProperty.end(); ++item)
		{
			if ((*item).first == _key)
			{
				mProperty.erase(item);
				break;
			}
		}
	}

	bool WidgetContainer::existProperty(std::string_view _key) const
	{
		for (const auto& item : mProperty)
		{
			if (item.first == _key)
				return true;
		}
		return false;
	}

	WidgetContainer::PropertyEnumerator WidgetContainer::getPropertyEnumerator() const
	{
		return PropertyEnumerator(mProperty);
	}

	void WidgetContainer::setLayerName(std::string_view _value)
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

	void WidgetContainer::setName(std::string_view _value)
	{
		mName = _value;
	}

	const std::string& WidgetContainer::getSkin() const
	{
		return mSkin;
	}

	void WidgetContainer::setSkin(std::string_view _value)
	{
		mSkin = _value;
	}

	const std::string& WidgetContainer::getType() const
	{
		return mType;
	}

	void WidgetContainer::setType(std::string_view _value)
	{
		mType = _value;
	}

	const std::string& WidgetContainer::getAlign() const
	{
		return mAlign;
	}

	void WidgetContainer::setAlign(std::string_view _value)
	{
		mAlign = _value;
	}

	const std::string& WidgetContainer::getStyle() const
	{
		return mStyle;
	}

	void WidgetContainer::setStyle(std::string_view _value)
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

	void WidgetContainer::setPropertyByIndex(size_t _index, std::string_view _key, std::string_view _value)
	{
		for (auto& item : mProperty)
		{
			if (item.first == _key)
			{
				if (_index == 0)
				{
					item.second = _value;
					break;
				}
				--_index;
			}
		}
	}

	void WidgetContainer::clearPropertyByIndex(size_t _index, std::string_view _key)
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
				--_index;
			}
		}
	}

}
