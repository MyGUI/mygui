/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "EditorWindow.h"

namespace demo
{

	EditorWindow::EditorWindow() : BaseLayout2("EditorWindow.layout")
	{
	}

	void EditorWindow::clearView()
	{
		MyGUI::WidgetManager::getInstance().destroyWidgets(mMainWidget->getEnumerator());
	}


} // namespace demo
