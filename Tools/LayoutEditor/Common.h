#ifndef _d90c3428_278f_44c7_9400_f8b7e1bff0bd_
#define _d90c3428_278f_44c7_9400_f8b7e1bff0bd_

#include <MyGUI.h>

namespace tools::utility
{

	// это можно в методы гуи занести
	inline MyGUI::IntCoord convertCoordToParentCoord(const MyGUI::IntCoord& _coord, MyGUI::Widget* _widget)
	{
		MyGUI::IntCoord coord = _coord;
		MyGUI::Widget* parent = _widget->getParent();
		while (nullptr != parent)
		{
			coord = coord - parent->getPosition();
			// а может у нас и дедушка есть? а может и прадед...
			parent = parent->getParent();
		}
		return coord;
	}

	inline void mapSet(MyGUI::VectorStringPairs& _map, std::string_view _key, std::string_view _value)
	{
		for (auto& iter : _map)
		{
			if (iter.first == _key)
			{
				iter.second = _value;
				return;
			}
		}
		_map.emplace_back(_key, _value);
	}

	inline MyGUI::VectorStringPairs::iterator mapFind(MyGUI::VectorStringPairs& _map, std::string_view _key)
	{
		for (MyGUI::VectorStringPairs::iterator iter = _map.begin(); iter != _map.end(); ++iter)
		{
			if (iter->first == _key)
				return iter;
		}
		return _map.end();
	}

	inline void mapErase(MyGUI::VectorStringPairs& _map, std::string_view _key)
	{
		for (MyGUI::VectorStringPairs::iterator iter = _map.begin(); iter != _map.end(); ++iter)
		{
			if (iter->first == _key)
			{
				_map.erase(iter);
				return;
			}
		}
	}

}

#endif
