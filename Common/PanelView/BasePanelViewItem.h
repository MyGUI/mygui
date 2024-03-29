/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#ifndef BASE_PANEL_VIEW_ITEM_H_
#define BASE_PANEL_VIEW_ITEM_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewCell.h"

namespace wraps
{

	class BasePanelViewItem : public wraps::BaseLayout
	{
	public:
		BasePanelViewItem(std::string_view _layout) :
			BaseLayout({}, nullptr),
			mLayout(_layout)
		{
		}

		void _initialise(BasePanelViewCell* _cell)
		{
			mPanelCell = _cell;
			mWidgetClient = mPanelCell->getClient();

			if (!mLayout.empty())
			{
				BaseLayout::initialise(mLayout, mWidgetClient);
				mMainWidget->setCoord(0, 0, mWidgetClient->getWidth(), mMainWidget->getHeight());
				mPanelCell->setClientHeight(mMainWidget->getHeight(), false);
			}

			initialise();
		}

		void _shutdown()
		{
			shutdown();

			if (!mLayout.empty())
			{
				BaseLayout::shutdown();
			}

			mPanelCell = nullptr;
			mWidgetClient = nullptr;
		}

		// реально изменилась ширина ячейки
		virtual void notifyChangeWidth(int _width)
		{
		}

		virtual void setVisible(bool _visible)
		{
			mPanelCell->setVisible(_visible);
			mPanelCell->eventUpdatePanel(mPanelCell);
		}

		bool getVisible()
		{
			return mPanelCell->getVisible();
		}

		BasePanelViewCell* getPanelCell()
		{
			return mPanelCell;
		}

	protected:
		virtual void initialise()
		{
		}
		virtual void shutdown()
		{
		}

	protected:
		BasePanelViewCell* mPanelCell{nullptr};
		MyGUI::Widget* mWidgetClient{nullptr};
		std::string mLayout;
	};

} // namespace wraps

#endif // BASE_PANEL_VIEW_ITEM_H_
