
#include "ListFactory.h"
#include "List.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& ListFactory::getType()
		{
			static Ogre::String type("List");
			return type;
		}

		WidgetPtr ListFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new List(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI