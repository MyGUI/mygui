/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LIST_FACTORY_H__
#define __MYGUI_LIST_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_List.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ListFactory : public BaseWidgetFactory<List>
		{
		public:
			ListFactory();
			~ListFactory();
		private:
			// методы для парсинга
			void List_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_LIST_FACTORY_H__
