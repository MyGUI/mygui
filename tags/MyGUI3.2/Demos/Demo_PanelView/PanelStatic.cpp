/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
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
