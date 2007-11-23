
#include "EditFactory.h"
#include "Edit.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& EditFactory::getType()
		{
			static Ogre::String type("Edit");
			return type;
		}

		WidgetPtr EditFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Edit(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI