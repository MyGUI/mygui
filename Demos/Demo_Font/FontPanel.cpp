/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "FontPanel.h"

FontPanel::FontPanel() :
	BaseLayout("FontPanel.layout")
{
}

void FontPanel::initialise()
{
	loadLayout();
}

void FontPanel::show()
{
	mMainWidget->show();
}

void FontPanel::hide()
{
	mMainWidget->hide();
}
