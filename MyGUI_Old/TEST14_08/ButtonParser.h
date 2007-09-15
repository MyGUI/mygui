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
				// �������� ���� �������� �������
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ButtonPressed") = newDelegate(this, &ButtonParser::ButtonPressed);
			}

			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ButtonPtr, _widget);
				static_cast<ButtonPtr>(_widget)->setButtonPressed(_value == "true");
			}

		};

	} // namespace parser

} // namespace widget