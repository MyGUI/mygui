/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "PanelStatic.h"

namespace demo
{

	PanelStatic::PanelStatic() : BasePanelViewItem("PanelStatic.layout")
	{
	}

	void PanelStatic::initialise()
	{
		mPanelCell->setCaption("Static panel");
	}

	void PanelStatic::shutdown()
	{
	}

} // namespace demo
