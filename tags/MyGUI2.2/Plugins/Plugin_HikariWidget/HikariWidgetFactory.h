/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#ifndef __HIKARI_WIDGET_FACTORY_H__
#define __HIKARI_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "FlashControl.h"

namespace Hikari
{
	namespace factory
	{

		class HikariWidgetFactory : public MyGUI::IWidgetFactory
		{
		public:
			HikariWidgetFactory();
			virtual ~HikariWidgetFactory();

			const std::string& getTypeName();
			MyGUI::WidgetPtr createWidget(MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle* _croppedParent, MyGUI::IWidgetCreator* _creator, const std::string& _name);
			bool isFalseType(MyGUI::WidgetPtr _ptr, const std::string &_key);

			static HMODULE getLibHandle() { return msFlashLib; }

			void Hikari_Source(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Hikari_Transparent(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value);

		private:
			static HMODULE msFlashLib;
		};

	} // namespace factory

} // namespace Hikari

#endif // __HIKARI_WIDGET_FACTORY_H__
