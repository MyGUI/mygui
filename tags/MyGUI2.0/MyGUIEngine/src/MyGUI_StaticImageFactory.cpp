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
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Image_Material") = newDelegate(this, &StaticImageFactory::Image_Material);
			manager.registerDelegate("Image_Rect") = newDelegate(this, &StaticImageFactory::Image_Rect);
			manager.registerDelegate("Image_Tile") = newDelegate(this, &StaticImageFactory::Image_Tile);
			manager.registerDelegate("Image_Num") = newDelegate(this, &StaticImageFactory::Image_Num);
		}

		StaticImageFactory::~StaticImageFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Image_Material");
			manager.unregisterDelegate("Image_Rect");
			manager.unregisterDelegate("Image_Tile");
			manager.unregisterDelegate("Image_Num");
		}

		const Ogre::String& StaticImageFactory::getType()
		{
			return StaticImage::_getType();
		}

		WidgetPtr StaticImageFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new StaticImage(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void StaticImageFactory::Image_Material(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageMaterial(_value);
		}

		void StaticImageFactory::Image_Rect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageRect(FloatRect::parse(_value));
		}

		void StaticImageFactory::Image_Tile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageTile(FloatSize::parse(_value));
		}

		void StaticImageFactory::Image_Num(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImagePtr, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageNum(utility::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI
