/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "ToolTip.h"
#include "ItemBoxV.h"
#include "ItemBoxH.h"
#include "CellView.h"

class DemoKeeper
{

public:
	void start();
	void end();

private:

	void notifyStartDrop(BaseLayout * _sender, ItemDropInfo _info, bool & _result);
	void notifyRequestDrop(BaseLayout * _sender, ItemDropInfo _info, bool & _result);
	void notifyEndDrop(BaseLayout * _sender, ItemDropInfo _info, bool _result);
	void notifyDropState(BaseLayout * _sender, MyGUI::DropItemState _state);
	void notifyNotifyItem(BaseLayout * _sender, const MyGUI::NotifyItemData & _info);

	void notifyToolTip(BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemData * _data);

private:
	ToolTip mToolTip;
	ItemBoxV mItemBoxV;
	ItemBoxH mItemBoxH;

};

#endif // __DEMO_KEEPER_H__
