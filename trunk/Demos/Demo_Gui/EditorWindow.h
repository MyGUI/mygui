/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __EDITOR_WINDOW_H__
#define __EDITOR_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	class EditorWindow : public wraps::BaseLayout
	{
	public:
		EditorWindow();
		virtual void initialise();

		MyGUI::WidgetPtr getClient() { return mainWidget()->getClientWidget(); }

	};

} // namespace demo

#endif // __EDITOR_WINDOW_H__
