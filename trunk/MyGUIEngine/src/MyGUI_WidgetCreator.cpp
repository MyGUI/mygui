/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#include "MyGUI_WidgetCreator.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	void WidgetCreator::_deleteWidget(WidgetPtr _widget)
	{
		delete _widget;
	}

} // namespace MyGUI
