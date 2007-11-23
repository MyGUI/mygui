
#include "VScrollFactory.h"
#include "VScroll.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& VScrollFactory::getType()
		{
			static Ogre::String type("VScroll");
			return type;
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new VScroll(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI