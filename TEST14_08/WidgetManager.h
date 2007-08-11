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

		Widget * createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, Widget * _parent=0)
		{
			for (std::list<WidgetFactoryBase*>::iterator factory = m_factoryList.begin(); factory != m_factoryList.end(); factory++) {
				if ((*factory)->getType() == _type) return (*factory)->createWidget(_skin, _x, _y, _cx, _cy, _align, _parent);
			}
			assert(0 && "no find factory WidgetFactory");
			return 0;
		}

		inline void registerFactory(WidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);
		}

	protected:
		WidgetManager() {};
		std::list<WidgetFactoryBase*> m_factoryList;
	};

} // namespace widget