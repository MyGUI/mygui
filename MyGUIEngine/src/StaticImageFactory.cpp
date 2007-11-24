
#include "StaticImageFactory.h"
#include "StaticImage.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StaticImageFactory::StaticImageFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ImageMaterial") = newDelegate(this, &StaticImageFactory::ImageMaterial);
			manager.registerDelegate("ImageRect") = newDelegate(this, &StaticImageFactory::ImageRect);
			manager.registerDelegate("ImageTile") = newDelegate(this, &StaticImageFactory::ImageTile);
			manager.registerDelegate("ImageNum") = newDelegate(this, &StaticImageFactory::ImageNum);
		}

		StaticImageFactory::~StaticImageFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("ImageMaterial");
			manager.unregisterDelegate("ImageRect");
			manager.unregisterDelegate("ImageTile");
			manager.unregisterDelegate("ImageNum");
		}

		const Ogre::String& StaticImageFactory::getType()
		{
			static Ogre::String type("StaticImage");
			return type;
		}

		WidgetPtr StaticImageFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new StaticImage(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void StaticImageFactory::ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageMaterial(_value);
		}

		void StaticImageFactory::ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageRect(util::parseFloatRect(_value));
		}

		void StaticImageFactory::ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageTile(util::parseFloatSize(_value));
		}

		void StaticImageFactory::ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageNum(util::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI