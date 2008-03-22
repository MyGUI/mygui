/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/

#include "MyGUI_ActionController.h"
#include "MyGUI_Widget.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	namespace action
	{

		void actionWidgetHide(WidgetPtr _widget)
		{
			_widget->hide();
		}

		void actionWidgetDestroy(WidgetPtr _widget)
		{
			WidgetManager::getInstance().destroyWidget(_widget);
		}

	} // namespace action

} // namespace MyGUI
