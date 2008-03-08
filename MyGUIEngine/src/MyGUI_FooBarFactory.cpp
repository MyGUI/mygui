/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/

#include "MyGUI_FooBarFactory.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_FooBar.h"

namespace MyGUI
{
	namespace factory
	{

		FooBarFactory::FooBarFactory()
		{
			// register itself in manager
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// register all parsers
			/*manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);*/
		}

		FooBarFactory::~FooBarFactory()
		{
			// unregister itself in manager
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// remove all parsers
			/*manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_Snap");*/
		}

		const Ogre::String& FooBarFactory::getType()
		{
			return FooBar::_getType();
		}

		WidgetPtr FooBarFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new FooBar(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	/*	void WindowFactory::Window_Snap(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setSnap(utility::parseBool(_value));
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(utility::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}*/

	} // namespace factory
} // namespace MyGUI
