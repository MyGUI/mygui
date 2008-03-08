/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/
#ifndef __MYGUI_FOOBAR_FACTORY_H__
#define __MYGUI_FOOBAR_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport FooBarFactory : public WidgetFactoryInterface
		{
		public:
			FooBarFactory();
			~FooBarFactory();

			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			/*void Window_Snap(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Window_AutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Window_MinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);*/

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_FOOBAR_FACTORY_H__
