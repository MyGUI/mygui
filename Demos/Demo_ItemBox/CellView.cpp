/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "CellView.h"

void CellView::getCellDimension(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(0, 0, 74, 74);
	else _coord.set(0, 0, 74, 74);
}

CellView::CellView() :
	wraps::BaseCellView<ItemData>("CellView.layout")
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

void CellView::update(const MyGUI::ItemInfo & _info, ItemData * _data)
{

	if (_info.update) {
		if (!_data->isEmpty()) {
			mImageItem->setItemResource(_data->getImage());
			mImageItem->setItemGroup("States");
			mImageItem->show();
		}
		else {
			mImageItem->hide();
		}
		mTextBack->setCaption(((_data->getCount() > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->getCount()) : "");
		mTextFront->setCaption(((_data->getCount() > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->getCount()) : "");

		static MyGUI::ResourceImageSetPtr resource_back = null;
		static MyGUI::ResourceImageSetPtr resource_select = null;
		if (resource_back == null) resource_back = MyGUI::ResourceManager::getInstance().getResource("pic_ItemBackImage")->castType<MyGUI::ResourceImageSet>();
		if (resource_select == null) resource_select = MyGUI::ResourceManager::getInstance().getResource("pic_ItemSelectImage")->castType<MyGUI::ResourceImageSet>();

		mImageBack->setItemResource(resource_back);
		mImageBack->setItemGroup("States");
		mImageBorder->setItemResource(resource_select);
		mImageBorder->setItemGroup("States");
	}

	if (_info.drag) {
		mImageBack->hide();
		mImageBorder->hide();

		if (!_data->isEmpty()) {
			if (_info.drag_refuse) mImageItem->setItemName("Refuse");
			else if (_info.drag_accept) mImageItem->setItemName("Accept");
			else mImageItem->setItemName("Normal");
			mImageItem->show();
		}
		else {
			mImageItem->hide();
		}
	}
	else {
		mImageBack->show();
		mImageBorder->show();
		if (_info.active) {
			if (_info.select) mImageBack->setItemName("Select");
			else mImageBack->setItemName("Active");
		}
		else if (_info.select) mImageBack->setItemName("Pressed");
		else mImageBack->setItemName("Normal");

		if (_info.drag_refuse) {
			mImageBorder->setItemName("Refuse");
			mTextFront->setColour(Ogre::ColourValue::Red);
		}
		else if (_info.drag_accept) {
			mImageBorder->setItemName("Accept");
			mTextFront->setColour(Ogre::ColourValue::Green);
		}
		else {
			mImageBorder->setItemName("Normal");
			mTextFront->setColour(Ogre::ColourValue::White);
		}

		if (!_data->isEmpty()) {
			mImageItem->setItemName("Normal");
			mImageItem->show();
		}
		else {
			mImageItem->hide();
		}
	}

}
