/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "ColourWindowCellView.h"

namespace demo
{

	void ColourWindowCellView::getCellDimension(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
	{
		_coord.set(0, 0, _sender->getClientCoord().width, 26);
	}

	ColourWindowCellView::ColourWindowCellView() :
		wraps::BaseCellView<ColourWindowCellData>("ColourWindowCellView.layout")
	{
	}

	void ColourWindowCellView::initialise(MyGUI::WidgetPtr _parent)
	{
		loadLayout(_parent);

		mMainWidget->setCoord(0, 0, _parent->getWidth(), _parent->getHeight());
		mMainWidget->setAlign(MyGUI::Align::Stretch);

		assignWidget(mText, "Text");
		assignWidget(mColour, "Colour");

		mRawColourView = mColour->getSubWidgetMain()->castType<MyGUI::RawRect>();
	}

	void ColourWindowCellView::update(const MyGUI::ItemInfo & _info, ColourWindowCellData * _data)
	{
		
		if (_info.update) {
			mText->setCaption(_data->getName());
			mRawColourView->setRectColour(_data->getColour(), _data->getColour(), _data->getColour(), _data->getColour());
		}

		if (_info.select) {
			mColour->show();
		}
		else {
			mColour->hide();
		}

	}

} // namespace demo
