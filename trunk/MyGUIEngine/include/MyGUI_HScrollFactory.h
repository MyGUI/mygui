/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_HSCROLL_FACTORY_H__
#define __MYGUI_HSCROLL_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT HScrollFactory : public BaseWidgetFactory<HScroll>
		{
		public:
			HScrollFactory();
			~HScrollFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_HSCROLL_FACTORY_H__
