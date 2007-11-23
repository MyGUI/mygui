#ifndef __STRANGE_BUTTON_PARSER_H__
#define __STRANGE_BUTTON_PARSER_H__

#include "Prerequest.h"
#include "WidgetDefines.h"

class TestPlugin;

namespace MyGUI
{
	namespace parser
	{

		class StrangeButtonParser
		{
			friend TestPlugin;

		private:
			StrangeButtonParser();

			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		};

	} // namespace parser
} // namespace MyGUI

#endif // __STRANGE_BUTTON_PARSER_H__