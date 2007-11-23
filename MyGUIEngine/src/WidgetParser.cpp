
#include "WidgetParser.h"
#include "SkinManager.h"
#include "Widget.h"
#include "delegate.h"
#include "ParserManager.h"

namespace MyGUI
{
	namespace parser
	{

		WidgetParser::WidgetParser()
		{
			// менеджер всех парсеров свойств
			MyGUI::ParserManager & parser = MyGUI::ParserManager::getInstance();

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

		void WidgetParser::SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setCaption(_value);
		}

		void WidgetParser::Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 4) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]), util::parseInt(vec[2]), util::parseInt(vec[3]) );
			else if (vec.size() == 2) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetParser::Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 2) _widget->size( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetParser::Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->show(util::parseBool(_value));
		}

		void WidgetParser::Color(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() > 2) _widget->setColour(Ogre::ColourValue(util::parseFloat(vec[0]), util::parseFloat(vec[1]), util::parseFloat(vec[2])) );
		}

		void WidgetParser::FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontName(_value);
		}

		void WidgetParser::FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontHeight(util::parseFloat(_value));
		}

		void WidgetParser::Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setAlpha(util::parseFloat(_value));
		}

		void WidgetParser::State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setState(_value);
		}

		void WidgetParser::NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setNeedKeyFocus(util::parseBool(_value));
		}

		void WidgetParser::AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setTextAlign(SkinManager::getInstance().parseAlign(_value));
		}

	} // namespace parser
} // namespace MyGUI