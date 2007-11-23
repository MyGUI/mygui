
#include "ButtonFactory.h"
#include "Button.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& ButtonFactory::getType()
		{
			static Ogre::String type("Button");
			return type;
		}

		WidgetPtr ButtonFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Button(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI