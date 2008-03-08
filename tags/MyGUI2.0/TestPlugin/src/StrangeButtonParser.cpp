
#include "StrangeButtonParser.h"
#include "StrangeButton.h"
#include "delegate.h"
#include "ParserManager.h"

namespace MyGUI
{
	namespace parser
	{

		/*StrangeButtonParser::StrangeButtonParser()
		{
			// менеджер всех парсеров свойств
			MyGUI::ParserManager & parser = MyGUI::ParserManager::getInstance();

			parser.registerDelegate("StrangeButtonPressed") = newDelegate(this, &StrangeButtonParser::ButtonPressed);
		}

		StrangeButtonParser::~StrangeButtonParser()
		{
			// менеджер всех парсеров свойств
			MyGUI::ParserManager & parser = MyGUI::ParserManager::getInstance();

			parser.unregisterDelegate("StrangeButtonPressed");
		}

		void StrangeButtonParser::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StrangeButtonPtr, _widget);
			static_cast<StrangeButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
		}*/

	} // namespace parser
} // namespace MyGUI