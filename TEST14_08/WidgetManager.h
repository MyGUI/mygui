#pragma once

#include <Ogre.h>
#include "Widget.h"
#include "WidgetFactory.h"

namespace widget
{

	class WidgetManager
	{
	public:
		static WidgetManager & getInstance()
		{
			static WidgetManager instance;
			return instance;
		}

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name)
		{
			Ogre::String name;
			if (!_name.empty()) {
				mapWidgetPtr::iterator iter = m_widgets.find(_name);
				if (iter != m_widgets.end()) assert(0 && "name widget is exist");
				name = _name;
			} else {
				static long num=0;
				name = Ogre::StringConverter::toString(num++) + "_" + _type;
			}

			for (std::list<WidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
				if ((*factory)->getType() == _type) {
					WidgetPtr widget = (*factory)->createWidget(_skin, _x, _y, _cx, _cy, _align, _parent, name);
					m_widgets[name] = widget;
					return widget;
				}
			}
			assert(0 && "no find factory WidgetFactory");
			return 0;
		}

		void destroyWidget(WidgetPtr & _widget)
		{
			delete _widget;
			_widget = 0;
		}

		inline void registerFactory(WidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);
		}

		WidgetPtr findWidget(const Ogre::String & _name)
		{
			mapWidgetPtr::iterator iter = m_widgets.find(_name);
			if (iter == m_widgets.end()) return 0;
			return iter->second;
		}

	protected:
		WidgetManager() {};
		std::list<WidgetFactoryBase*> m_factoryList;
		mapWidgetPtr m_widgets;
	};

} // namespace widget