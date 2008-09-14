
#include "StrangeButtonFactory.h"
#include "StrangeButton.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StrangeButtonFactory::StrangeButtonFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("StrangeButton_Pressed") = newDelegate(this, &StrangeButtonFactory::ButtonPressed);
		}

		StrangeButtonFactory::~StrangeButtonFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("StrangeButton_Pressed");
		}

		// реализация интерфейса фабрики
		const std::string & StrangeButtonFactory::getTypeName()
		{
			return StrangeButton::getClassTypeName();
		}

		WidgetPtr StrangeButtonFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String& _name)
		{
			return new StrangeButton(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		// методы для парсинга
		void StrangeButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StrangeButtonPtr, _widget);
			static_cast<StrangeButtonPtr>(_widget)->setButtonPressed(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI