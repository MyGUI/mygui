/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_STATIC_IMAGE_FACTORY_H__
#define __MYGUI_STATIC_IMAGE_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport StaticImageFactory : public WidgetFactoryInterface
		{
		public:
			StaticImageFactory();
			~StaticImageFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга
			void Image_Texture(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Image_Rect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Image_Tile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Image_Num(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_FACTORY_H__
