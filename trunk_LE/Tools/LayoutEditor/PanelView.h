/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include "PanelView/BasePanelView.h"
#include "PanelViewCell.h"

namespace tools
{
	class PanelView :
		public wraps::BasePanelView<PanelViewCell>
	{
	public:
		PanelView(MyGUI::Widget* _parent) :
			wraps::BasePanelView<PanelViewCell>("", _parent)
		{
		}
	};

} // namespace tools

#endif // __PANEL_VIEW_H__
