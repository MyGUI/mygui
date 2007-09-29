#ifndef _WIDGETPARSER_H_
#define _WIDGETPARSER_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport WidgetParser
		{
		public:
			WidgetParser()
			{
				// менеджер всех парсеров свойств
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("Caption") = newDelegate(this, &WidgetParser::SetCaption);
				parser.registerDelegate("Move") = newDelegate(this, &WidgetParser::Move);
				parser.registerDelegate("Size") = newDelegate(this, &WidgetParser::Size);
				parser.registerDelegate("Show") = newDelegate(this, &WidgetParser::Show);
				parser.registerDelegate("Color") = newDelegate(this, &WidgetParser::Color);
				parser.registerDelegate("FontName") = newDelegate(this, &WidgetParser::FontName);
				parser.registerDelegate("FontHeight") = newDelegate(this, &WidgetParser::FontHeight);
				parser.registerDelegate("Alpha") = newDelegate(this, &WidgetParser::Alpha);
				parser.registerDelegate("State") = newDelegate(this, &WidgetParser::State);
				parser.registerDelegate("NeedKey") = newDelegate(this, &WidgetParser::NeedKey);
				parser.registerDelegate("AlignText") = newDelegate(this, &WidgetParser::AlignText);
			}

			void SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setCaption(_value);
			}

			void Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				const std::vector<std::string> & vec = util::split(_value);
				if (vec.size() == 4) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]), util::parseInt(vec[2]), util::parseInt(vec[3]) );
				else if (vec.size() == 2) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]) );
			}

			void Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				const std::vector<std::string> & vec = util::split(_value);
				if (vec.size() == 2) _widget->size( util::parseInt(vec[0]), util::parseInt(vec[1]) );
			}

			void Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->show(util::parseBool(_value));
			}

			void Color(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				const std::vector<std::string> & vec = util::split(_value);
				if (vec.size() > 2) _widget->setColour(Ogre::ColourValue(util::parseFloat(vec[0]), util::parseFloat(vec[1]), util::parseFloat(vec[2])) );
			}

			void FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setFontName(_value);
			}

			void FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setFontHeight(util::parseFloat(_value));
			}

			void Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setAlpha(util::parseFloat(_value));
			}

			void State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setState(_value);
			}

			void NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setNeedKeyFocus(util::parseBool(_value));
			}

			void AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setTextAlign(SkinManager::getInstance().parseAlign(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI

#endif
