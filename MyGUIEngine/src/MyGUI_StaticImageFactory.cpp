/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImageFactory.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

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
			return StaticImage::getType();
		}

		WidgetPtr StaticImageFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new StaticImage(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void StaticImageFactory::ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageMaterial(_value);
		}

		void StaticImageFactory::ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageRect(FloatRect::parse(_value));
		}

		void StaticImageFactory::ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageTile(FloatSize::parse(_value));
		}

		void StaticImageFactory::ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StaticImagePtr, _widget);
			static_cast<StaticImagePtr>(_widget)->setImageNum(util::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI