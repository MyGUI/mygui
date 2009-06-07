/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __EDITOR_WINDOW_H__
#define __EDITOR_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class EditorWindow : public wraps::BaseLayout
	{
	public:
		EditorWindow();

		void clearView();
		MyGUI::WidgetPtr getView() { return mMainWidget; }

	};

} // namespace demo

#endif // __EDITOR_WINDOW_H__
