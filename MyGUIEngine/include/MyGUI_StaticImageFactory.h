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
#ifndef __MYGUI_STATIC_IMAGE_FACTORY_H__
#define __MYGUI_STATIC_IMAGE_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_StaticImage.h"

namespace MyGUI
{
	namespace factory
	{

		//typedef std::map<std::string, ParseDelegate> MapDelegate;

		class MYGUI_EXPORT StaticImageFactory : public BaseWidgetFactory<StaticImage>
		{
		public:
			StaticImageFactory();
			~StaticImageFactory();
		private:
			// методы для парсинга
			void Image_Texture(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Tile(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Index(WidgetPtr _widget, const std::string &_key, const std::string &_value);

			void Image_Resource(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Group(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Name(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		/*private:
			MapDelegate mDelegates;*/
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_FACTORY_H__
