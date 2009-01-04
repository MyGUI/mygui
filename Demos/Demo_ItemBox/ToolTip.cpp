/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ToolTip.h"

namespace demo
{

	ToolTip::ToolTip() : BaseLayout2("ToolTip.layout")
	{
		assignWidget(mTextName, "text_Name");
		assignWidget(mTextCount, "text_Count");
		assignWidget(mTextDesc, "text_Desc");
		assignWidget(mImageInfo, "image_Info");

		const MyGUI::IntCoord & coord = mTextDesc->getTextCoord();
		mOffsetHeight = mMainWidget->getHeight() - coord.height;
	}

	void ToolTip::show(ItemData * _data, const MyGUI::IntPoint & _point)
	{
		const MyGUI::IntPoint offset(10, 10);

		if ((_data == null) || _data->isEmpty()) return;

		MyGUI::IntPoint point = _point + offset;
		MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();

		const MyGUI::IntSize & size = mMainWidget->getSize();

		if ((point.left + size.width) > gui->getViewWidth()) {
			point.left -= offset.left + offset.left + size.width;
		}
		if ((point.top + size.height) > gui->getViewHeight()) {
			point.top -= offset.top + offset.top + size.height;
		}

		mTextCount->setCaption(MyGUI::utility::toString(_data->getCount()));
		mTextName->setCaption(_data->getInfo()->getItemName());
		mTextDesc->setCaption(_data->getInfo()->getItemDescription());
		if (!_data->isEmpty()) {
			mImageInfo->setItemResource(_data->getImage(), "ToolTip", "Normal");
		}

		// вычисляем размер
		const MyGUI::IntSize & text_size = mTextDesc->getTextSize();
		mMainWidget->setSize(mMainWidget->getWidth(), mOffsetHeight + text_size.height);

		mMainWidget->setPosition(point);
		mMainWidget->show();

	}

	void ToolTip::hide()
	{
		mMainWidget->hide();
	}

} // namespace demo
