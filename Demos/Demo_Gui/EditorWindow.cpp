/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#include "Precompiled.h"
#include "EditorWindow.h"

namespace demo
{

	EditorWindow::EditorWindow() : BaseLayout("EditorWindow.layout")
	{
	}

	void EditorWindow::clearView()
	{
		MyGUI::WidgetManager::getInstance().destroyWidgets(mMainWidget->getEnumerator());
	}


} // namespace demo
