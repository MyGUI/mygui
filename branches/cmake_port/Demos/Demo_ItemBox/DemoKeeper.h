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
#include "ItemBoxWindow.h"
#include "CellView.h"
#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:

		void notifyStartDrop(wraps::BaseLayout * _sender, wraps::DDItemInfo _info, bool & _result);
		void notifyRequestDrop(wraps::BaseLayout * _sender, wraps::DDItemInfo _info, bool & _result);
		void notifyEndDrop(wraps::BaseLayout * _sender, wraps::DDItemInfo _info, bool _result);
		void notifyDropState(wraps::BaseLayout * _sender, MyGUI::DDItemState _state);
		void notifyNotifyItem(wraps::BaseLayout * _sender, const MyGUI::IBNotifyItemData & _info);

		void notifyToolTip(wraps::BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemData * _data);

	private:
		ToolTip * mToolTip;
		ItemBoxWindow * mItemBoxV;
		ItemBoxWindow * mItemBoxH;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
