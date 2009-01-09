/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_STATIC_TEXT_FACTORY_H__
#define __MYGUI_STATIC_TEXT_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_StaticText.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT StaticTextFactory : public BaseWidgetFactory<StaticText>
		{
		public:
			StaticTextFactory();
			~StaticTextFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_FACTORY_H__
