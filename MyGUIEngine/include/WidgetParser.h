#ifndef __WIDGET_PARSER_H__
#define __WIDGET_PARSER_H__

#include "Prerequest.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	class ParserManager;

	namespace parser
	{

		class _MyGUIExport WidgetParser
		{
			friend ParserManager;

		private:
			WidgetParser();

			void SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Color(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		};

	} // namespace parser
} // namespace MyGUI

#endif // __WIDGET_PARSER_H__