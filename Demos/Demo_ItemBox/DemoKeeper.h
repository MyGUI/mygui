/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "ToolTip.h"
#include "ItemBoxWindow.h"
#include "CellView.h"
#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyStartDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool& _result);
		void notifyRequestDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool& _result);
		void notifyEndDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool _result);
		void notifyDropState(wraps::BaseLayout* _sender, MyGUI::DDItemState _state);
		void notifyNotifyItem(wraps::BaseLayout* _sender, const MyGUI::IBNotifyItemData& _info);

		void notifyToolTip(wraps::BaseLayout* _sender, const MyGUI::ToolTipInfo& _info, ItemData* _data);
		void setupResources() override;

	private:
		ToolTip* mToolTip{nullptr};
		ItemBoxWindow* mItemBoxV{nullptr};
		ItemBoxWindow* mItemBoxH{nullptr};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
