/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_ITEM_BOX_FACTORY_H__
#define __MYGUI_ITEM_BOX_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_ItemBox.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ItemBoxFactory : public BaseWidgetFactory<ItemBox>
		{
		public:
			ItemBoxFactory();
			~ItemBoxFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_FACTORY_H__
