/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include "PanelView/BasePanelView.h"
#include "PanelViewCell.h"

//namespace demo
//{

	class PanelView : public wraps::BasePanelView<PanelViewCell>
	{
	public:
		PanelView(MyGUI::WidgetPtr _parent) : wraps::BasePanelView<PanelViewCell>("", _parent) { }

	};

//} // namespace demo

#endif // __PANEL_VIEW_H__
