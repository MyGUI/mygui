#pragma once

#include <Ogre.h>
#include "Widget.h"
#include "SkinManager.h"

namespace widget
{

	class WidgetFactoryBase
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual Widget * createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, Widget * _parent) = 0;
	}; // WidgetFactoryBase

	template <class ClassName>
	class WidgetFactory : public WidgetFactoryBase
	{
	public:
		WidgetFactory() { WidgetManager::getInstance().registerFactory(this); }
		inline const Ogre::String & getType() {return ClassName::getType();};
		Widget * createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, Widget * _parent)
		{
			return new ClassName(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent);
		}
	}; // class WidgetFactory : public WidgetFactoryBase

} // namespace widget