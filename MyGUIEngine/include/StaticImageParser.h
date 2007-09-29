#ifndef _STATICIMAGEPARSER_H_
#define _STATICIMAGEPARSER_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"
#include "StaticImage.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport StaticImageParser
		{
		public:
			StaticImageParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ImageMaterial") = newDelegate(this, &StaticImageParser::ImageMaterial);
				parser.registerDelegate("ImageRect") = newDelegate(this, &StaticImageParser::ImageRect);
				parser.registerDelegate("ImageTile") = newDelegate(this, &StaticImageParser::ImageTile);
				parser.registerDelegate("ImageNum") = newDelegate(this, &StaticImageParser::ImageNum);
			}

			void ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageMaterial(_value);
			}

			void ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageRect(util::parseFloatRect(_value));
			}

			void ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageTile(util::parseFloatSize(_value));
			}

			void ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(StaticImagePtr, _widget);
				static_cast<StaticImagePtr>(_widget)->setImageNum(util::parseSizeT(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI

#endif
