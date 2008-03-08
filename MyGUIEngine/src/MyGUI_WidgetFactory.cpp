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
			manager.registerDelegate("Widget_Caption") = newDelegate(this, &WidgetFactory::Widget_Caption);
			manager.registerDelegate("Widget_Move") = newDelegate(this, &WidgetFactory::Widget_Move);
			manager.registerDelegate("Widget_Size") = newDelegate(this, &WidgetFactory::Widget_Size);
			manager.registerDelegate("Widget_Show") = newDelegate(this, &WidgetFactory::Widget_Show);
			manager.registerDelegate("Widget_Colour") = newDelegate(this, &WidgetFactory::Widget_Colour);
			manager.registerDelegate("Widget_FontName") = newDelegate(this, &WidgetFactory::Widget_FontName);
			manager.registerDelegate("Widget_FontHeight") = newDelegate(this, &WidgetFactory::Widget_FontHeight);
			manager.registerDelegate("Widget_Alpha") = newDelegate(this, &WidgetFactory::Widget_Alpha);
			manager.registerDelegate("Widget_State") = newDelegate(this, &WidgetFactory::Widget_State);
			manager.registerDelegate("Widget_NeedKey") = newDelegate(this, &WidgetFactory::Widget_NeedKey);
			manager.registerDelegate("Widget_AlignText") = newDelegate(this, &WidgetFactory::Widget_AlignText);
			manager.registerDelegate("Widget_Enabled") = newDelegate(this, &WidgetFactory::Widget_Enabled);
		}

		WidgetFactory::~WidgetFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Widget_Caption");
			manager.unregisterDelegate("Widget_Move");
			manager.unregisterDelegate("Widget_Size");
			manager.unregisterDelegate("Widget_Show");
			manager.unregisterDelegate("Widget_Colour");
			manager.unregisterDelegate("Widget_FontName");
			manager.unregisterDelegate("Widget_FontHeight");
			manager.unregisterDelegate("Widget_Alpha");
			manager.unregisterDelegate("Widget_State");
			manager.unregisterDelegate("Widget_NeedKey");
			manager.unregisterDelegate("Widget_AlignText");
			manager.unregisterDelegate("Widget_Enabled");
		}

		const Ogre::String& WidgetFactory::getType()
		{
			return Widget::_getType();
		}

		WidgetPtr WidgetFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new Widget(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void WidgetFactory::Widget_Caption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			// change '\n' on char 10
			size_t pos = _value.find("\\n");
			if (pos == std::string::npos) _widget->setCaption(_value);
			else {
				std::string value(_value);
				while (pos != std::string::npos) {
					value[pos++] = '\n';
					value.erase(pos, 1);
					pos = value.find("\\n");
				}
				_widget->setCaption(value);
			}
		}

		void WidgetFactory::Widget_Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = utility::split(_value);
			if (vec.size() == 4) _widget->setPosition(utility::parseInt(vec[0]), utility::parseInt(vec[1]), utility::parseInt(vec[2]), utility::parseInt(vec[3]) );
			else if (vec.size() == 2) _widget->setPosition( utility::parseInt(vec[0]), utility::parseInt(vec[1]) );
		}

		void WidgetFactory::Widget_Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = utility::split(_value);
			if (vec.size() == 2) _widget->setSize( utility::parseInt(vec[0]), utility::parseInt(vec[1]) );
		}

		void WidgetFactory::Widget_Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			if (utility::parseBool(_value)) _widget->show();
			else _widget->hide();
		}

		void WidgetFactory::Widget_Colour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setColour(utility::parseColour(_value));
		}

		void WidgetFactory::Widget_FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontName(_value);
		}

		void WidgetFactory::Widget_FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontHeight((Ogre::ushort)utility::parseFloat(_value));
		}

		void WidgetFactory::Widget_Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setAlpha(utility::parseFloat(_value));
		}

		void WidgetFactory::Widget_State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setState(_value);
		}

		void WidgetFactory::Widget_NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setNeedKeyFocus(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setTextAlign(SkinManager::getInstance().parseAlign(_value));
		}

		void WidgetFactory::Widget_Enabled(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setEnabled(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
