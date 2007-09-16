#ifndef _WIDGETFACTORY_H_
#define _WIDGETFACTORY_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include "Widget.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{

	class _MyGUIExport WidgetFactoryBase
	{
	public:
		//virtual Ogre::String getType() = 0;
		virtual const Ogre::String& getType(){ return mType; };
		virtual WidgetPtr createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name) = 0;

		Ogre::String mType;
	}; // WidgetFactoryBase

	template <class ClassName>
	class _MyGUIExport WidgetFactory : public WidgetFactoryBase
	{
	public:
		WidgetFactory(Ogre::String type) { mType = type; WidgetManager::getInstance().registerFactory(this); }
		//inline Ogre::String  getType() {return type;/*ClassName::getType();*/};
		WidgetPtr createWidget(const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent, const Ogre::String & _name)
		{
			return new ClassName(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}
	}; // class WidgetFactory : public WidgetFactoryBase

} // namespace MyGUI

#endif
