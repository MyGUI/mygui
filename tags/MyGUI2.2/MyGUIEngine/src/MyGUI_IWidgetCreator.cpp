/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#include "MyGUI_IWidgetCreator.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	void IWidgetCreator::_deleteWidget(WidgetPtr _widget)
	{
		delete _widget;
	}

} // namespace MyGUI
