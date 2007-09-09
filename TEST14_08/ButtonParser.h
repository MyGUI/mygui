#pragma once

#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"
#include "Button.h"

namespace widget
{

	namespace parser
	{

		class ButtonParser
		{
		public:
			ButtonParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

/*				parser.registerDelegate("ImageMaterial") = newDelegate(this, &StaticImageParser::ImageMaterial);
				parser.registerDelegate("ImageRect") = newDelegate(this, &StaticImageParser::ImageRect);
				parser.registerDelegate("ImageTile") = newDelegate(this, &StaticImageParser::ImageTile);
				parser.registerDelegate("ImageNum") = newDelegate(this, &StaticImageParser::ImageNum);*/
			}

			/*void ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageMaterial(_value);
			}

			void ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageRect(floatRect::parse(_value));
			}

			void ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageTile(floatSize::parse(_value));
			}

			void ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageNum(parseValue<size_t>(_value));
			}*/

		};

	} // namespace parser

} // namespace widget