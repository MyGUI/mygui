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

		private:
			void Text_TextColour(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Text_TextAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Text_FontName(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Text_FontHeight(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_FACTORY_H__
