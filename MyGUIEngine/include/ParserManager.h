#ifndef __PARSER_MANAGER_H__
#define __PARSER_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "WidgetDefines.h"
#include "delegate.h"

#include "WidgetParser.h"
#include "WindowParser.h"

namespace MyGUI
{

	typedef CDelegate3<WidgetPtr,  const Ogre::String &, const Ogre::String &> ParseDelegate;
	typedef std::map<Ogre::String, ParseDelegate> MapDelegate;

	class _MyGUIExport ParserManager
	{
		INSTANCE_HEADER(ParserManager);

	public:
		void initialise();
		void shutdown();

		void parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		ParseDelegate & registerDelegate(const Ogre::String & _key);
		void unregisterDelegate(const Ogre::String & _key);

	private:
		MapDelegate mDelegates;

		// парсеры
		parser::WidgetParser * mWidgetParser;
		parser::WindowParser * mWindowParser;

	};

} // namespace MyGUI

#endif // __PARSER_MANAGER_H__
