/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "CellView.h"

CellView::CellView() :
	BaseLayout("CellView.layout")
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

void CellView::update(const MyGUI::ItemInfo& _info, ItemData * _data)
{

	if (_info.update) {
		mImageItem->setImageRect(MyGUI::IntRect((int)_data->type * 68 + 68*2, 0, (int)_data->type * 68 + 68*3, 68*3));
		mTextBack->setCaption(((_data->count > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->count) : "");
		mTextFront->setCaption(((_data->count > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->count) : "");
	}

	if (_info.drag) {
		mImageBack->setAlpha(0.8);
		mImageBack->setImageNum(5);
		mImageBorder->setImageNum(4);
		if (_info.drag_refuse) mImageItem->setImageNum(1);
		else if (_info.drag_accept) mImageItem->setImageNum(2);
		else mImageItem->setImageNum(0);
	}
	else {
		mImageBack->setAlpha(1);
		if (_info.active) {
			if (_info.select) mImageBack->setImageNum(2);
			else mImageBack->setImageNum(3);
		}
		else if (_info.select) mImageBack->setImageNum(1);
		else mImageBack->setImageNum(0);

		if (_info.drag_refuse) {
			mImageBorder->setImageNum(1);
			mTextFront->setColour(Ogre::ColourValue::Red);
		}
		else if (_info.drag_accept) {
			mImageBorder->setImageNum(2);
			mTextFront->setColour(Ogre::ColourValue::Green);
		}
		else {
			mImageBorder->setImageNum(0);
			mTextFront->setColour(Ogre::ColourValue::White);
		}

		mImageItem->setImageNum(0);
	}

}
