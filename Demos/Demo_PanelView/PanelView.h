/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef PANEL_VIEW_H_
#define PANEL_VIEW_H_

#include "PanelView/BasePanelView.h"
#include "PanelViewCell.h"

namespace demo
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

} // namespace demo

#endif // PANEL_VIEW_H_
