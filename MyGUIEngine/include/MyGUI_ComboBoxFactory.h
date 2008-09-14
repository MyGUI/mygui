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
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport ComboBoxFactory : public IWidgetFactory
		{
		public:
			ComboBoxFactory();
			~ComboBoxFactory();

			// реализация интерфейса фабрики
			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);

			// методы для парсинга
			void ComboBox_ModeDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void ComboBox_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_FACTORY_H__
