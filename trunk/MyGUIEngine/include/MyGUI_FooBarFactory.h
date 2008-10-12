/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/
#ifndef __MYGUI_FOOBAR_FACTORY_H__
#define __MYGUI_FOOBAR_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_FooBar.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport FooBarFactory : public BaseWidgetFactory<FooBar>
		{
		public:
			FooBarFactory();
			~FooBarFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_FOOBAR_FACTORY_H__
