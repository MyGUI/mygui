/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_MULTI_LIST_FACTORY_H__
#define __MYGUI_MULTI_LIST_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_MultiList.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT MultiListFactory : public BaseWidgetFactory<MultiList>
		{
		public:
			MultiListFactory();
			~MultiListFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_FACTORY_H__
