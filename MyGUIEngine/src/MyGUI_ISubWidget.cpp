/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ISubWidgetText.h"

namespace MyGUI
{

	MYGUI_RTTI_BASE_IMPLEMENT( ISubWidget );
	ISubWidget::~ISubWidget() { }

	MYGUI_RTTI_CHILD_IMPLEMENT( ISubWidgetRect, ISubWidget );
	ISubWidgetRect::~ISubWidgetRect() { }

	MYGUI_RTTI_CHILD_IMPLEMENT( ISubWidgetText, ISubWidget );
	ISubWidgetText::~ISubWidgetText() { }

} // namespace MyGUI
