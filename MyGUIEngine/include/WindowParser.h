#ifndef __WINDOW_PARSER_H__
#define __WINDOW_PARSER_H__

#include "Prerequest.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	class ParserManager;

	namespace parser
	{

		class _MyGUIExport WindowParser
		{
			friend ParserManager;

		private:
			WindowParser();

			void WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		};

	} // namespace parser
} // namespace MyGUI

#endif // __WINDOW_PARSER_H__