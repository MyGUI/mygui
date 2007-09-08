#pragma once

#include <Ogre.h>
#include "Gui.h"
#include "Widget.h"
#include "WidgetFactory.h"
#include "Instance.h"

namespace widget
{

	class WidgetManager
	{
	public:
		INSTANCE(WidgetManager)

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name);

		void destroyWidget();
		void destroyWidget(WidgetPtr & _widget);

		inline void registerFactory(WidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);
		}

		WidgetPtr findWidget(const Ogre::String & _name);

//	private:
		inline void clearName(WidgetPtr _widget)
		{
			if (_widget == null) return;
			mapWidgetPtr::iterator iter = m_widgets.find(_widget->getName());
			if (iter != m_widgets.end()) m_widgets.erase(iter);
		}

	protected:
		WidgetManager() {};
		std::list<WidgetFactoryBase*> m_factoryList;
		mapWidgetPtr m_widgets;
	};

} // namespace widget