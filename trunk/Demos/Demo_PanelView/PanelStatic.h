/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_STATIC_H__
#define __PANEL_STATIC_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

namespace demo
{

	class PanelStatic : public wraps::BaseLayout, public wraps::PanelBase
	{
	public:

		PanelStatic();

		virtual void initialiseCell(wraps::PanelCell * _cell);
		virtual void shutdownCell();

	};

} // namespace demo

#endif // __PANEL_STATIC_H__
