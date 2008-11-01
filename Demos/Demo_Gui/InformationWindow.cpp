/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "InformationWindow.h"

namespace demo
{

	InformationWindow::InformationWindow() :
		BaseLayout("InformationWindow.layout")
	{
	}

	void InformationWindow::initialise()
	{
		loadLayout();

	}

} // namespace demo
