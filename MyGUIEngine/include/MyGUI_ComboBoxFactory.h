/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_FACTORY_H__
#define __MYGUI_COMBO_BOX_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_ComboBox.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ComboBoxFactory : public BaseWidgetFactory<ComboBox>
		{
		public:
			ComboBoxFactory();
			~ComboBoxFactory();
		private:
			// методы для парсинга
			void ComboBox_ModeDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void ComboBox_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_FACTORY_H__
