/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WidgetFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WidgetFactory::WidgetFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("Caption") = newDelegate(this, &WidgetFactory::SetCaption);
			manager.registerDelegate("Move") = newDelegate(this, &WidgetFactory::Move);
			manager.registerDelegate("Size") = newDelegate(this, &WidgetFactory::Size);
			manager.registerDelegate("Show") = newDelegate(this, &WidgetFactory::Show);
			manager.registerDelegate("Color") = newDelegate(this, &WidgetFactory::Color);
			manager.registerDelegate("FontName") = newDelegate(this, &WidgetFactory::FontName);
			manager.registerDelegate("FontHeight") = newDelegate(this, &WidgetFactory::FontHeight);
			manager.registerDelegate("Alpha") = newDelegate(this, &WidgetFactory::Alpha);
			manager.registerDelegate("State") = newDelegate(this, &WidgetFactory::State);
			manager.registerDelegate("NeedKey") = newDelegate(this, &WidgetFactory::NeedKey);
			manager.registerDelegate("AlignText") = newDelegate(this, &WidgetFactory::AlignText);
		}

		WidgetFactory::~WidgetFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Caption");
			manager.unregisterDelegate("Move");
			manager.unregisterDelegate("Size");
			manager.unregisterDelegate("Show");
			manager.unregisterDelegate("Color");
			manager.unregisterDelegate("FontName");
			manager.unregisterDelegate("FontHeight");
			manager.unregisterDelegate("Alpha");
			manager.unregisterDelegate("State");
			manager.unregisterDelegate("NeedKey");
			manager.unregisterDelegate("AlignText");
		}

		const Ogre::String& WidgetFactory::getType()
		{
			static Ogre::String type("Widget");
			return type;
		}

		WidgetPtr WidgetFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Widget(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WidgetFactory::SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setCaption(_value);
		}

		void WidgetFactory::Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 4) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]), util::parseInt(vec[2]), util::parseInt(vec[3]) );
			else if (vec.size() == 2) _widget->move( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetFactory::Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 2) _widget->size( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetFactory::Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->show(util::parseBool(_value));
		}

		void WidgetFactory::Color(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() > 2) _widget->setColour(Ogre::ColourValue(util::parseFloat(vec[0]), util::parseFloat(vec[1]), util::parseFloat(vec[2])) );
		}

		void WidgetFactory::FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontName(_value);
		}

		void WidgetFactory::FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontHeight(util::parseFloat(_value));
		}

		void WidgetFactory::Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setAlpha(util::parseFloat(_value));
		}

		void WidgetFactory::State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setState(_value);
		}

		void WidgetFactory::NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setNeedKeyFocus(util::parseBool(_value));
		}

		void WidgetFactory::AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setTextAlign(SkinManager::getInstance().parseAlign(_value));
		}

	} // namespace factory
} // namespace MyGUI