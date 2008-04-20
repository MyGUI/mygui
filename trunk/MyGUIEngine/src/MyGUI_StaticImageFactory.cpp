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
			manager.registerDelegate("Image_Texture") = newDelegate(this, &StaticImageFactory::Image_Texture);
			manager.registerDelegate("Image_Rect") = newDelegate(this, &StaticImageFactory::Image_Rect);
			manager.registerDelegate("Image_Tile") = newDelegate(this, &StaticImageFactory::Image_Tile);
			manager.registerDelegate("Image_Num") = newDelegate(this, &StaticImageFactory::Image_Num);
		}

		StaticImageFactory::~StaticImageFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Image_Texture");
			manager.unregisterDelegate("Image_Rect");
			manager.unregisterDelegate("Image_Tile");
			manager.unregisterDelegate("Image_Num");
		}

		const Ogre::String& StaticImageFactory::getType()
		{
			return StaticImage::_getType();
		}

		WidgetPtr StaticImageFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new StaticImage(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void StaticImageFactory::Image_Texture(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageTexture(_value);
		}

		void StaticImageFactory::Image_Rect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageRect(IntRect::parse(_value));
		}

		void StaticImageFactory::Image_Tile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageTile(IntSize::parse(_value));
		}

		void StaticImageFactory::Image_Num(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageNum(utility::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI
