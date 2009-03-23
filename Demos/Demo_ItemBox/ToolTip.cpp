/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#include "precompiled.h"
#include "ToolTip.h"

namespace demo
{

	ToolTip::ToolTip() : BaseLayout("ToolTip.layout")
	{
		assignWidget(mTextName, "text_Name");
		assignWidget(mTextCount, "text_Count");
		assignWidget(mTextDesc, "text_Desc");
		assignWidget(mImageInfo, "image_Info");

		MyGUI::ISubWidgetText* text = mTextDesc->getSubWidgetText();
		const MyGUI::IntCoord& coord = text ? text->getCoord() : MyGUI::IntCoord();
		mOffsetHeight = mMainWidget->getHeight() - coord.height;
	}

	void ToolTip::show(ItemData * _data, const MyGUI::IntPoint & _point)
	{
		const MyGUI::IntPoint offset(10, 10);

		if ((_data == nullptr) || _data->isEmpty()) return;

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
			mImageInfo->setItemResourceInfo(_data->getImage(), "ToolTip", "Normal");
		}

		// вычисляем размер
		MyGUI::ISubWidgetText* text = mTextDesc->getSubWidgetText();
		const MyGUI::IntSize& text_size = text ? text->getTextSize() : MyGUI::IntSize();
		mMainWidget->setSize(mMainWidget->getWidth(), mOffsetHeight + text_size.height);

		mMainWidget->setPosition(point);
		mMainWidget->setVisible(true);

	}

	void ToolTip::hide()
	{
		mMainWidget->setVisible(false);
	}

} // namespace demo
