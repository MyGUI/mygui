/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MYGUI_DDCONTAINER_FACTORY_H__
#define __MYGUI_DDCONTAINER_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_DDContainer.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT DDContainerFactory : public BaseWidgetFactory<DDContainer>
		{
		public:
			DDContainerFactory();
			~DDContainerFactory();

		private:
			// методы для парсинга
			void DDContainer_NeedDragDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_DDCONTAINER_FACTORY_H__
