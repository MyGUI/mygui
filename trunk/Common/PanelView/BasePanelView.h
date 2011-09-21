/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef __BASE_PANEL_VIEW_H__
#define __BASE_PANEL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

namespace wraps
{

	template <typename TypeCell>
	class BasePanelView :
		public BaseLayout
	{
	public:
		typedef std::vector<BasePanelViewItem*> VectorCell;

	public:
		BasePanelView(const std::string& _layout, MyGUI::Widget* _parent) :
			BaseLayout(_layout, _parent),
			mNeedUpdate(false),
			mOldClientWidth(0),
			mFirstInitialise(false)
		{
			mScrollView = mMainWidget->castType<MyGUI::ScrollView>();

			// потом перенести в лейаут
			mScrollView->setCanvasAlign(MyGUI::Align::HCenter | MyGUI::Align::Top);
			mScrollView->setVisibleHScroll(false);
			mNeedUpdate = false;

			mOldClientWidth = mScrollView->getViewCoord().width;

			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BasePanelView::frameEnteredCheck);
		}

		virtual ~BasePanelView()
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BasePanelView::frameEnteredCheck);

			removeAllItems();

			if (mNeedUpdate)
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BasePanelView::frameEntered);
		}

		//! Get number of items
		size_t getItemCount()
		{
			return mItems.size();
		}

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, BasePanelViewItem* _item)
		{
			MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "BasePanelView::insertItem");

			if (_index == MyGUI::ITEM_NONE)
				_index = mItems.size();
			MYGUI_ASSERT(findItem(_item) == MyGUI::ITEM_NONE, "panel allready exist");

			// создаем лейаут базовой ячейки
			BasePanelViewCell* cell = new TypeCell(mScrollView);
			cell->eventUpdatePanel = MyGUI::newDelegate(this, &BasePanelView::notifyUpdatePanel);

			// теперь основной лейаут ячейки
			_item->_initialise(cell);

			mItems.insert(mItems.begin() + _index, _item);
			setNeedUpdate();
			mFirstInitialise = true;
		}

		//! Add an item to the end of a list
		void addItem(BasePanelViewItem* _item)
		{
			insertItem(MyGUI::ITEM_NONE, _item);
		}

		//! Get item from specified position
		BasePanelViewItem* getItem(size_t _index)
		{
			MYGUI_ASSERT_RANGE(_index, mItems.size(), "BasePanelView::getItem");
			return mItems[_index];
		}

		//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
		size_t findItem(BasePanelViewItem* _item)
		{
			for (VectorCell::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
			{
				if ((*iter) == _item)
					return iter - mItems.begin();
			}
			return MyGUI::ITEM_NONE;
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index)
		{
			MYGUI_ASSERT_RANGE(_index, mItems.size(), "BasePanelView::removeItemAt");

			BasePanelViewCell* cell = mItems[_index]->getPanelCell();
			mItems[_index]->_shutdown();
			delete cell;

			mItems.erase(mItems.begin() + _index);
			setNeedUpdate();
		}

		//! Remove item at a specified position
		void removeItem(BasePanelViewItem* _item)
		{
			size_t index = findItem(_item);
			MYGUI_ASSERT(index != MyGUI::ITEM_NONE, "item is not found");
			removeItemAt(index);
		}

		//! Remove all items
		void removeAllItems()
		{
			for (VectorCell::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
			{
				BasePanelViewCell* cell = (*iter)->getPanelCell();
				(*iter)->_shutdown();
				delete cell;
			}
			mItems.clear();
			setNeedUpdate();
		}

		void updateView()
		{
			// вычисляем максимальную высоту всего добра
			int height = 0;
			for (VectorCell::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
			{
				MyGUI::Widget* widget = (*iter)->getPanelCell()->getMainWidget();
				if (widget->getVisible())
				{
					height += widget->getHeight();
				}
			}
			// ставим высоту холста, и спрашиваем получившуюся ширину клиента
			mScrollView->setCanvasSize(0, height);
			// ширина клиента могла поменятся
			const MyGUI::IntSize& size = mScrollView->getViewCoord().size();
			mScrollView->setCanvasSize(size.width, height);

			bool change = mFirstInitialise;
			if (mOldClientWidth != size.width)
			{
				mOldClientWidth = size.width;
				change = true;
			}

			// выравниваем все панели
			int pos = 0;
			for (VectorCell::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
			{
				MyGUI::Widget* widget = (*iter)->getPanelCell()->getMainWidget();
				if (widget->getVisible() || mFirstInitialise)
				{
					height = widget->getHeight();
					widget->setCoord(MyGUI::IntCoord(0, pos, size.width, height));

					// оповещаем, что мы обновили ширину
					if (change)
						(*iter)->notifyChangeWidth(size.width);

					pos += height;
				}
			}

			mNeedUpdate = false;
			mFirstInitialise = false;
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BasePanelView::frameEntered);
		}

		// изменились размеры
		// необходимо обновить все панели
		void setNeedUpdate()
		{
			if (!mNeedUpdate)
			{
				mNeedUpdate = true;
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BasePanelView::frameEntered);
			}
		}

	private:
		void notifyUpdatePanel(BasePanelViewCell* _panel)
		{
			setNeedUpdate();
		}

		void frameEntered(float _time)
		{
			updateView();
		}

		void frameEnteredCheck(float _time)
		{
			const MyGUI::IntSize& size = mMainWidget->getSize();
			if (size != mOldSize)
			{
				mOldSize = size;
				setNeedUpdate();
			}
		}

	protected:
		MyGUI::ScrollView* mScrollView;

	private:
		VectorCell mItems;
		bool mNeedUpdate;
		int mOldClientWidth;
		MyGUI::IntSize mOldSize;
		bool mFirstInitialise;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_H__
