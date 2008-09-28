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
			manager.registerDelegate("Image_Tile") = newDelegate(this, &StaticImageFactory::Image_Tile);
			manager.registerDelegate("Image_Index") = newDelegate(this, &StaticImageFactory::Image_Index);

			manager.registerDelegate("Image_Resource") = newDelegate(this, &StaticImageFactory::Image_Resource);
			manager.registerDelegate("Image_Group") = newDelegate(this, &StaticImageFactory::Image_Group);
			manager.registerDelegate("Image_Name") = newDelegate(this, &StaticImageFactory::Image_Name);
		}

		StaticImageFactory::~StaticImageFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Image_Texture");
			manager.unregisterDelegate("Image_Coord");
			manager.unregisterDelegate("Image_Tile");
			manager.unregisterDelegate("Image_Index");

			manager.unregisterDelegate("Image_Resource");
			manager.unregisterDelegate("Image_Group");
			manager.unregisterDelegate("Image_Name");
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

		void StaticImageFactory::Image_Resource(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setItemResource(_value);
		}

		void StaticImageFactory::Image_Group(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setItemGroup(_value);
		}

		void StaticImageFactory::Image_Name(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(StaticImage, _widget, _key);
			static_cast<StaticImagePtr>(_widget)->setItemName(_value);
		}

	} // namespace factory
} // namespace MyGUI
