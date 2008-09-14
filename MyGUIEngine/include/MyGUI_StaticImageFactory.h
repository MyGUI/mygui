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
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport StaticImageFactory : public IWidgetFactory
		{
		public:
			StaticImageFactory();
			~StaticImageFactory();

			// реализация интерфейса фабрики
			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);

			// методы для парсинга
			void Image_Texture(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Rect(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Tile(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Index(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_Name(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_MapNames(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Image_AddItemInfo(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_FACTORY_H__
