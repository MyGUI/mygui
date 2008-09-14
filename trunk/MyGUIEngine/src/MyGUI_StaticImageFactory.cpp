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
			manager.registerDelegate("Image_Coord") = newDelegate(this, &StaticImageFactory::Image_Coord);
			manager.registerDelegate("Image_Rect") = newDelegate(this, &StaticImageFactory::Image_Rect);
			manager.registerDelegate("Image_Tile") = newDelegate(this, &StaticImageFactory::Image_Tile);
			manager.registerDelegate("Image_Num") = newDelegate(this, &StaticImageFactory::Image_Index);
			manager.registerDelegate("Image_Index") = newDelegate(this, &StaticImageFactory::Image_Index);
			manager.registerDelegate("Image_Name") = newDelegate(this, &StaticImageFactory::Image_Name);
			manager.registerDelegate("Image_MapNames") = newDelegate(this, &StaticImageFactory::Image_MapNames);
			manager.registerDelegate("Image_AddItemInfo") = newDelegate(this, &StaticImageFactory::Image_AddItemInfo);
		}

		StaticImageFactory::~StaticImageFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Image_Texture");
			manager.unregisterDelegate("Image_Coord");
			manager.unregisterDelegate("Image_Rect");
			manager.unregisterDelegate("Image_Tile");
			manager.unregisterDelegate("Image_Num");
			manager.unregisterDelegate("Image_Index");
			manager.unregisterDelegate("Image_Name");
			manager.unregisterDelegate("Image_MapNames");
			manager.unregisterDelegate("Image_AddItemInfo");
		}

		const std::string & StaticImageFactory::getTypeName()
		{
			return StaticImage::getClassTypeName();
		}

		WidgetPtr StaticImageFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new StaticImage(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void StaticImageFactory::Image_Texture(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageTexture(_value);
		}

		void StaticImageFactory::Image_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageCoord(IntCoord::parse(_value));
		}

		void StaticImageFactory::Image_Rect(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageRect(IntRect::parse(_value));
		}

		void StaticImageFactory::Image_Tile(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setImageTile(IntSize::parse(_value));
		}

		void StaticImageFactory::Image_Index(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setItemSelect(utility::parseSizeT(_value));
		}

		void StaticImageFactory::Image_Name(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setItemSelect(_value);
		}

		void StaticImageFactory::Image_MapNames(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->addItemNames(_value);
		}

		void StaticImageFactory::Image_AddItemInfo(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->addItemInfo(_value);
		}

	} // namespace factory
} // namespace MyGUI
