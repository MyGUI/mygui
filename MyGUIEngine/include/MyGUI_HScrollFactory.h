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
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport HScrollFactory : public IWidgetFactory
		{
		public:
			HScrollFactory();
			~HScrollFactory();

			// реализация интерфейса фабрики
			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_HSCROLL_FACTORY_H__
