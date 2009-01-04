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

		// ����� ��������� � ������
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
		// ��������� ������������ ������ ����� �����
		int height = 0;
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->getMainWidget();
			if (widget->isShow()) {
				height += widget->getHeight();
			}
		}
		// ������ ������ ������, � ���������� ������������ ������ �������
		mScrollView->setCanvasSize(0, height);
		// ������ ������� ����� ���������
		const MyGUI::IntSize & size = mScrollView->getClientCoord().size();
		mScrollView->setCanvasSize(size.width, height);

		bool change = false;
		if (mOldClientWidth != size.width) {
			mOldClientWidth = size.width;
			change = true;
		}

		// ����������� ��� ������
		int pos = 0;
		for (VectorCell::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
			MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->getMainWidget();
			if (widget->isShow()) {

				height = widget->getHeight();
				widget->setCoord(MyGUI::IntCoord(0, pos, size.width, height));

				// ���������, ��� �� �������� ������
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

		// ������� ������ ������� ������
		BasePanelViewCell * cell = createCell();
		cell->eventUpdatePanel = MyGUI::newDelegate(this, &BasePanelView::notifyUpdatePanel);

		// ������ �������� ������ ������
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
