
#include "StaticImageFactory.h"
#include "StaticImage.h"
#include "SkinManager.h"

namespace MyGUI
{
	namespace factory
	{

		const Ogre::String& StaticImageFactory::getType()
		{
			static Ogre::String type("StaticImage");
			return type;
		}

		WidgetPtr StaticImageFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new StaticImage(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI