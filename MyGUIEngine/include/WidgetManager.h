#ifndef _WIDGETMANAGER_H_
#define _WIDGETMANAGER_H_

#include "Prerequest.h"
#include "Instance.h"
#include "Gui.h"
#include "Widget.h"
#include "WidgetFactory.h"
#include "LoggingOut.h"

namespace MyGUI
{

	class _MyGUIExport WidgetManager
	{
		INSTANCE_HEADER(WidgetManager);

	public:
		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name);

		void destroyWidget();
		void destroyWidget(WidgetPtr & _widget);

		inline void registerFactory(WidgetFactoryBase * _factory)
		{
			m_factoryList.push_back(_factory);

			//LOG_MESSAGE("* Register widget factory '" + _factory->getType() + "'");
		}

		WidgetPtr findWidget(const Ogre::String & _name);
		// преобразует изначальное смещение, в текущее, так как будто скин был создан изначально
		static FloatRect convertOffset(const FloatRect & _offset, char _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight);

		//	private:
		void clearName(WidgetPtr _widget);

		WidgetManager();

	protected:
		std::list<WidgetFactoryBase*> m_factoryList;
		mapWidgetPtr m_widgets;
	};

} // namespace MyGUI

#endif
