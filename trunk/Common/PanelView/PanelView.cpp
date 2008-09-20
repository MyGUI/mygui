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
	mScrollView->setCanvasAlign(MyGUI::Align::HCenter | MyGUI::Align::Top);
	mScrollView->showHScroll(false);
	mNeedUpdate = false;

	// лейаут базовой €чейки панели
	//mPanelCellLayout = "PanelCell.layout";

}

void PanelView::notifyUpdatePanel(PanelCell * _panel)
{
	setNeedUpdate();
}

void PanelView::updateView()
{
	// вычисл€ем максимальную высоту всего добра
	int height = 0;
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->mainWidget();
		if (widget->isShow()) {
			height += widget->getHeight();
		}
	}
	// ставим высоту холста, и спрашиваем получившуюс€ ширину клиента
	mScrollView->setCanvasSize(0, height);
	// ширина клиента могла помен€тс€
	MyGUI::IntCoord coord = mScrollView->getClientCoord();
	mScrollView->setCanvasSize(coord.width, height);

	// выравниваем все панели
	int pos = 0;
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		MyGUI::WidgetPtr widget = (*iter)->getPanelCell()->mainWidget();
		if (widget->isShow()) {
			height = widget->getHeight();
			widget->setPosition(MyGUI::IntCoord(0, pos, coord.width, height));
			pos += height;
		}
	}

	mNeedUpdate = false;
	MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &PanelView::frameEntered);
}

void PanelView::setNeedUpdate(){
	if (!mNeedUpdate)
	{
		mNeedUpdate = true;
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &PanelView::frameEntered);
	}
}

void PanelView::insertItem(size_t _index, PanelBase * _item)
{
	MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "PanelView::insertItem");
	if (_index == MyGUI::ITEM_NONE) _index = mItems.size();
	MYGUI_ASSERT(findItem(_item) == MyGUI::ITEM_NONE, "panel allready exist");

	// создаем лейаут базовой €чейки
	PanelCell * cell = new PanelCell();
	cell->initialise(mScrollView);
	cell->eventUpdatePanel = MyGUI::newDelegate(this, &PanelView::notifyUpdatePanel);

	// теперь основной лейаут €чейки
	_item->initialiseCell(cell);

	mItems.insert(mItems.begin() + _index, _item);
	setNeedUpdate();
}

PanelBase * PanelView::getItem(size_t _index)
{
	MYGUI_ASSERT_RANGE(_index, mItems.size(), "PanelView::getItem");
	return mItems[_index];
}

size_t PanelView::findItem(PanelBase * _item)
{
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		if ((*iter) == _item) return iter - mItems.begin();
	}
	return MyGUI::ITEM_NONE;
}

void PanelView::removeItemAt(size_t _index)
{
	MYGUI_ASSERT_RANGE(_index, mItems.size(), "PanelView::removeItemAt");

	PanelCell * cell = mItems[_index]->getPanelCell();
	mItems[_index]->shutdownCell();
	delete cell;

	mItems.erase(mItems.begin() + _index);
	setNeedUpdate();
}

void PanelView::removeItem(PanelBase * _item)
{
	size_t index = findItem(_item);
	MYGUI_ASSERT(index != MyGUI::ITEM_NONE, "item is not found");
	removeItemAt(index);
}

void PanelView::removeAllItems()
{
	for (VectorPanel::iterator iter=mItems.begin(); iter!=mItems.end(); ++iter) {
		PanelCell * cell = (*iter)->getPanelCell();
		(*iter)->shutdownCell();
		delete cell;
	}
	mItems.clear();
	setNeedUpdate();
}
