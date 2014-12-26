/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef EDITOR_WINDOW_H_
#define EDITOR_WINDOW_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class EditorWindow :
		public wraps::BaseLayout
	{
	public:
		EditorWindow();

		void clearView();

		MyGUI::Widget* getView();
	};

} // namespace demo

#endif // EDITOR_WINDOW_H_
