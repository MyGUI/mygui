#ifndef _WIDGETFACTORY_H_
#define _WIDGETFACTORY_H_

#include "Prerequest.h"
#include <Ogre.h>
#include "Widget.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace widget
{

	class _MyGUIExport WidgetFactoryBase
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual WidgetPtr createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name) = 0;
	}; // WidgetFactoryBase

	template <class ClassName>
	class _MyGUIExport WidgetFactory : public WidgetFactoryBase
	{
	public:
		WidgetFactory() { WidgetManager::getInstance().registerFactory(this); }
		inline const Ogre::String & getType() {return ClassName::getType();};
		WidgetPtr createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name)
		{
			return new ClassName(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}
	}; // class WidgetFactory : public WidgetFactoryBase

} // namespace widget

#endif
