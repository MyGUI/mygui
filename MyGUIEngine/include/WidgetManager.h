#ifndef _WIDGETMANAGER_H_
#define _WIDGETMANAGER_H_

#include "Prerequest.h"
#include <Ogre.h>
#include "Gui.h"
#include "Widget.h"
#include "Instance.h"
#include "WidgetFactory.h"

namespace widget
{

	

	class _MyGUIExport WidgetManager
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
		void clearName(WidgetPtr _widget);

	protected:
		WidgetManager() {};
		std::list<WidgetFactoryBase*> m_factoryList;
		mapWidgetPtr m_widgets;
	};

} // namespace widget

#endif
