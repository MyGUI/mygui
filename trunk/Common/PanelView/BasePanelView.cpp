/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "BasePanelView.h"

namespace wraps
{

	BasePanelView::BasePanelView(const std::string & _layout, MyGUI::WidgetPtr _parent) :
		BaseLayout2(_layout, _parent)
	{
		mScrollView = mMainWidget->castType<MyGUI::ScrollView>();

		// потом перенести в лейаут
		mScrollView->setCanvasAlign(MyGUI::Align::HCenter | MyGUI::Align::Top);
		mScrollView->showHScroll(false);
		mNeedUpdate = false;

		mOldClientWidth = mScrollView->getClientCoord().width;
	}

	BasePanelView::~BasePanelView()
	{
		removeAllItems();
	}

	void BasePanelView::notifyUpdatePanel(BasePanelViewCell * _panel)
	{
		setNeedUpdate();
	}

	void BasePanelView::updateView()
	{
		// вычисляем максимальную высоту всего добра
		int height = 0;
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->getMainWidget();
			if (widget->isShow()) {
				height += widget->getHeight();
			}
		}
		// ставим высоту холста, и спрашиваем получившуюся ширину клиента
		mScrollView->setCanvasSize(0, height);
		// ширина клиента могла поменятся
		const MyGUI::IntSize & size = mScrollView->getClientCoord().size();
		mScrollView->setCanvasSize(size.width, height);

		bool change = false;
		if (mOldClientWidth != size.width) {
			mOldClientWidth = size.width;
			change = true;
		}

		// выравниваем все панели
		int pos = 0;
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->getMainWidget();
			if (widget->isShow()) {

				height = widget->getHeight();
				widget->setCoord(MyGUI::IntCoord(0, pos, size.width, height));

				// оповещаем, что мы обновили ширину
				if (change) (*iter)->notifyChangeWidth(size.width);

				pos += height;
			}
		}

		mNeedUpdate = false;
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BasePanelView::frameEntered);
	}

	void BasePanelView::setNeedUpdate()
	{
		if (!mNeedUpdate) {
			mNeedUpdate = true;
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BasePanelView::frameEntered);
		}
	}

	void BasePanelView::insertItem(size_t _index, BasePanelViewItem * _item)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "BasePanelView::insertItem");
		if (_index == MyGUI::ITEM_NONE) _index = mItems.size();
		MYGUI_ASSERT(findItem(_item) == MyGUI::ITEM_NONE, "panel allready exist");

		// создаем лейаут базовой ячейки
		BasePanelViewCell * cell = createCell();
		cell->eventUpdatePanel = MyGUI::newDelegate(this, &BasePanelView::notifyUpdatePanel);

		// теперь основной лейаут ячейки
		_item->_initialise(cell);

		mItems.insert(mItems.begin() + _index, _item);
		setNeedUpdate();
	}

	BasePanelViewItem * BasePanelView::getItem(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "BasePanelView::getItem");
		return mItems[_index];
	}

	size_t BasePanelView::findItem(BasePanelViewItem * _item)
	{
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			if ((*iter) == _item) return iter - mItems.begin();
		}
		return MyGUI::ITEM_NONE;
	}

	void BasePanelView::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "BasePanelView::removeItemAt");

		BasePanelViewCell * cell = mItems[_index]->getPanelCell();
		mItems[_index]->_shutdown();
		delete cell;

		mItems.erase(mItems.begin() + _index);
		setNeedUpdate();
	}

	void BasePanelView::removeItem(BasePanelViewItem * _item)
	{
		size_t index = findItem(_item);
		MYGUI_ASSERT(index != MyGUI::ITEM_NONE, "item is not found");
		removeItemAt(index);
	}

	void BasePanelView::removeAllItems()
	{
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			BasePanelViewCell * cell = (*iter)->getPanelCell();
			(*iter)->_shutdown();
			delete cell;
		}
		mItems.clear();
		setNeedUpdate();
	}

} // namespace wraps
