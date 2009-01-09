/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
