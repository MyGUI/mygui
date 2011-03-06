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
		widget(nullptr),
		relative_mode(false)
	{
	}

	WidgetContainer::WidgetContainer(const std::string& _type, const std::string& _skin, MyGUI::Widget* _widget, const std::string& _name):
		widget(_widget),
		type(_type),
		skin(_skin),
		name(_name),
		relative_mode(false)
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
		if (relative_mode)
		{
			MyGUI::FloatCoord coord = MyGUI::CoordConverter::convertToRelative(widget->getCoord(), widget->getParentSize());
			std::ostringstream stream;
			if (_percent)
				stream << coord.left * 100 << " " << coord.top * 100 << " " << coord.width * 100 << " " << coord.height * 100;
			else
				stream << coord.left << " " << coord.top << " " << coord.width << " " << coord.height;
			return stream.str();
		}
		return widget->getCoord().print();
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

	void WidgetContainer::setLayerName(const std::string& _layerName)
	{
		layer = _layerName;
	}

	const std::string& WidgetContainer::getLayerName() const
	{
		return layer;
	}
}