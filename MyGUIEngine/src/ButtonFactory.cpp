
#include "ButtonFactory.h"
#include "Button.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ButtonFactory::ButtonFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ButtonPressed") = newDelegate(this, &ButtonFactory::ButtonPressed);
		}

		ButtonFactory::~ButtonFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("ButtonPressed");
		}

		const Ogre::String& ButtonFactory::getType()
		{
			static Ogre::String type("Button");
			return type;
		}

		WidgetPtr ButtonFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Button(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ButtonPtr, _widget);
			static_cast<ButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI