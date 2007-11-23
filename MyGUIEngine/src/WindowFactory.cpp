
#include "WindowFactory.h"
#include "Window.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& WindowFactory::getType()
		{
			static Ogre::String type("Window");
			return type;
		}

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Window(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI