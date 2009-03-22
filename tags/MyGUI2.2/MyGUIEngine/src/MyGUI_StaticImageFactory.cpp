/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_StaticImageFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StaticImageFactory::StaticImageFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
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
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Image_Texture");
			manager.unregisterDelegate("Image_Coord");
			manager.unregisterDelegate("Image_Tile");
			manager.unregisterDelegate("Image_Index");

			manager.unregisterDelegate("Image_Resource");
			manager.unregisterDelegate("Image_Group");
			manager.unregisterDelegate("Image_Name");
		}

		void StaticImageFactory::Image_Texture(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setImageTexture(_value);
		}

		void StaticImageFactory::Image_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setImageCoord(IntCoord::parse(_value));
		}

		void StaticImageFactory::Image_Tile(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setImageTile(IntSize::parse(_value));
		}

		void StaticImageFactory::Image_Index(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setItemSelect(utility::parseSizeT(_value));
		}

		void StaticImageFactory::Image_Resource(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setItemResource(_value);
		}

		void StaticImageFactory::Image_Group(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setItemGroup(_value);
		}

		void StaticImageFactory::Image_Name(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticImagePtr>(_widget)->setItemName(_value);
		}

	} // namespace factory
} // namespace MyGUI
