/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __PANEL_VIEW_CELL_H__
#define __PANEL_VIEW_CELL_H__

#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelViewCell : public wraps::BasePanelViewCell
	{
	public:
		PanelViewCell(MyGUI::WidgetPtr _parent);

	private:
		void notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonDoubleClick(MyGUI::WidgetPtr _sender);

	private:
		MyGUI::ButtonPtr mButtonMinimize;

	};

} // namespace demo

#endif // __PANEL_VIEW_CELL_H__
