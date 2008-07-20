/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ToolTip.h"

ToolTipWindow::ToolTipWindow() :
	BaseLayout("ToolTip.layout")
{
}

void ToolTipWindow::initialise()
{
	loadLayout();

	assignWidget(mTextName, "text_Name");
	assignWidget(mTextCount, "text_Count");
	assignWidget(mTextDesc, "text_Desc");
	assignWidget(mImageInfo, "image_Info");
}

void ToolTipWindow::show(ItemData * _data, const MyGUI::IntPoint & _point)
{
	const MyGUI::IntPoint offset(10, 10);

	if (_data == null) return;
	if (_data->type == 0) return;

	MyGUI::IntPoint point = _point + offset;
	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();

	const MyGUI::IntSize & size = mMainWidget->getSize();

	if ((point.left + size.width) > gui->getViewWidth()) {
		point.left -= offset.left + offset.left + size.width;
	}
	if ((point.top + size.height) > gui->getViewHeight()) {
		point.top -= offset.top + offset.top + size.height;
	}

	mTextCount->setCaption(MyGUI::utility::toString(_data->count));
	mTextName->setCaption(_data->name);
	mTextDesc->setCaption(_data->description);

	mMainWidget->setPosition(point);
	mMainWidget->show();

}

void ToolTipWindow::hide()
{
	mMainWidget->hide();
}
