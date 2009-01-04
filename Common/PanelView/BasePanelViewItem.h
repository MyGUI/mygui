/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_PANEL_VIEW_ITEM_H__
#define __BASE_PANEL_VIEW_ITEM_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewCell.h"

namespace wraps
{

	class BasePanelViewItem : public wraps::BaseLayout2
	{
	public:
		BasePanelViewItem(const std::string& _layout) :
			BaseLayout2("", null),
			mLayout(_layout)
		{
		}

		void _initialise(BasePanelViewCell * _cell)
		{
			mPanelCell = _cell;
			mWidgetClient = mPanelCell->getClient();

			if ( ! mLayout.empty()) {
				BaseLayout2::initialise(mLayout, mWidgetClient);
				mMainWidget->setCoord(0, 0, mWidgetClient->getWidth(), mMainWidget->getHeight());
				mPanelCell->setClientHeight(mMainWidget->getHeight(), false);
			}

			initialise();
		}

		void _shutdown()
		{
			shutdown();

			if ( ! mLayout.empty()) {
				BaseLayout2::shutdown();
			}

			mPanelCell = 0;
			mWidgetClient = null;
		}

		// реально изменилась ширина €чейки
		virtual void notifyChangeWidth(int _width)
		{
		}

		virtual void show()
		{
			mPanelCell->show();
			mPanelCell->eventUpdatePanel(mPanelCell);
		}

		virtual void hide()
		{
			mPanelCell->hide();
			mPanelCell->eventUpdatePanel(mPanelCell);
		}

		BasePanelViewCell * getPanelCell() { return mPanelCell; }

	protected:
		virtual void initialise() { }
		virtual void shutdown() { }

	protected:
		BasePanelViewCell * mPanelCell;
		MyGUI::WidgetPtr mWidgetClient;
		std::string mLayout;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_ITEM_H__
