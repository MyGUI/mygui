/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelView.h"

void PanelView::attach(MyGUI::ScrollViewPtr _widget)
{
	mScrollView = _widget;

	// потом перенести в лейаут
	mScrollView->setCanvasAlign(MyGUI::ALIGN_HCENTER | MyGUI::ALIGN_TOP);
	mScrollView->showHScroll(false);

}

void PanelView::notifyUpdatePanel(Panel * _panel)
{
	updateView();
}

void PanelView::updateView()
{
	// вычисляем максимальную высоту всего добра
	int height = 0;
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		height += (*iter)->mainWidget()->getHeight();
	}
	// ставим высоту холста, и спрашиваем получившуюся ширину клиента
	mScrollView->setCanvasSize(0, height);
	// ширина клиента могла поменятся
	MyGUI::IntCoord coord = mScrollView->getClientCoord();
	mScrollView->setCanvasSize(coord.width, height);

	// выравниваем все панели
	int pos = 0;
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		MyGUI::WidgetPtr widget = (*iter)->mainWidget();
		height = widget->getHeight();
		widget->setPosition(MyGUI::IntCoord(0, pos, coord.width, height));
		pos += height;
	}
}

void PanelView::insertItem(size_t _index, Panel * _item)
{
	MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "PanelView::insertItem");
	if (_index == MyGUI::ITEM_NONE) _index = mItems.size();
	MYGUI_ASSERT(findItem(_item) == MyGUI::ITEM_NONE, "panel allready exist");

	_item->initialise(mScrollView);
	_item->eventUpdatePanel = MyGUI::newDelegate(this, &PanelView::notifyUpdatePanel);

	mItems.insert(mItems.begin() + _index, _item);
	updateView();
}

Panel * PanelView::getItem(size_t _index)
{
	MYGUI_ASSERT_RANGE(_index, mItems.size(), "PanelView::getItem");
	return mItems[_index];
}

size_t PanelView::findItem(Panel * _item)
{
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		if ((*iter) == _item) return iter - mItems.begin();
	}
	return MyGUI::ITEM_NONE;
}

void PanelView::removeItemAt(size_t _index)
{
	MYGUI_ASSERT_RANGE(_index, mItems.size(), "PanelView::removeItemAt");

	mItems[_index]->shutdown();
	mItems.erase(mItems.begin() + _index);
	updateView();
}

void PanelView::removeItem(Panel * _item)
{
	size_t index = findItem(_item);
	MYGUI_ASSERT(index != MyGUI::ITEM_NONE, "item is not found");
	removeItemAt(index);
}

void PanelView::removeAllItems()
{
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		(*iter)->shutdown();
	}
	mItems.clear();
	updateView();
}
