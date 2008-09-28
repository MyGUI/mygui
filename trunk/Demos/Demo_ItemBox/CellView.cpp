/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "CellView.h"

CellView::CellView() :
	BaseCellView<ItemData>("CellView.layout")
{
}

void CellView::initialise(MyGUI::WidgetPtr _parent)
{
	loadLayout(_parent);

	assignWidget(mImageBack, "image_Back");
	assignWidget(mImageBorder, "image_Border");
	assignWidget(mImageItem, "image_Item");
	assignWidget(mTextBack, "text_Back");
	assignWidget(mTextFront, "text_Front");
}

/*void CellView::getSizeCell(WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop)
{
}*/

void CellView::update(const MyGUI::ItemInfo& _info, ItemData * _data)
{
	//ItemData * data = (ItemData *)_data;

	if (_info.update) {
		mImageItem->setImageCoord(MyGUI::IntCoord((int)_data->type * 68 + 68*2, 0, 68, 68));
		mTextBack->setCaption(((_data->count > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->count) : "");
		mTextFront->setCaption(((_data->count > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->count) : "");
	}

	if (_info.drag) {
		mImageBack->setAlpha(0.8);
		mImageBack->setImageIndex(5);
		mImageBorder->setImageIndex(4);
		if (_info.drag_refuse) mImageItem->setImageIndex(1);
		else if (_info.drag_accept) mImageItem->setImageIndex(2);
		else mImageItem->setImageIndex(0);
	}
	else {
		mImageBack->setAlpha(1);
		if (_info.active) {
			if (_info.select) mImageBack->setImageIndex(2);
			else mImageBack->setImageIndex(3);
		}
		else if (_info.select) mImageBack->setImageIndex(1);
		else mImageBack->setImageIndex(0);

		if (_info.drag_refuse) {
			mImageBorder->setImageIndex(1);
			mTextFront->setColour(Ogre::ColourValue::Red);
		}
		else if (_info.drag_accept) {
			mImageBorder->setImageIndex(2);
			mTextFront->setColour(Ogre::ColourValue::Green);
		}
		else {
			mImageBorder->setImageIndex(0);
			mTextFront->setColour(Ogre::ColourValue::White);
		}

		mImageItem->setImageIndex(0);
	}

}
