/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/

#ifndef _ae8e1480_accf_4372_bb29_1349c604d2f7_
#define _ae8e1480_accf_4372_bb29_1349c604d2f7_

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

}

#endif
