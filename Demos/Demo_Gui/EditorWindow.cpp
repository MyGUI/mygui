/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "EditorWindow.h"

namespace demo
{

	EditorWindow::EditorWindow() :
		BaseLayout("EditorWindow.layout")
	{
	}

	void EditorWindow::initialise()
	{
		loadLayout();

	}

} // namespace demo
