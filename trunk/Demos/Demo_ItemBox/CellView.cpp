/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#include "Precompiled.h"
#include "CellView.h"

namespace demo
{

	void CellView::getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop)
	{
		// размеры ячеек для контейнера и для перетаскивания могут различаться
		/*if (_drop)
			_coord.set(0, 0, 90, 74);
		else
			_coord.set(0, 0, 90, 74);*/

		_coord.set(0, 0, 90, 74);
	}

	CellView::CellView(MyGUI::Widget* _parent) :
		wraps::BaseCellView<ItemData*>("CellView.layout", _parent)
	{
		assignWidget(mImageBack, "image_Back");
		assignWidget(mImageBorder, "image_Border");
		assignWidget(mImageItem, "image_Item");
		assignWidget(mTextBack, "text_Back");
		assignWidget(mTextFront, "text_Front");
	}

	void CellView::update(const MyGUI::IBDrawItemInfo& _info, ItemData* _data)
	{
		if (_info.update)
		{
			if (!_data->isEmpty())
			{
				mImageItem->setItemResourcePtr(_data->getImage());
				mImageItem->setItemGroup("States");
				mImageItem->setVisible(true);
			}
			else
			{
				mImageItem->setVisible(false);
			}
			mTextBack->setCaption(((_data->getCount() > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->getCount()) : "");
			mTextFront->setCaption(((_data->getCount() > 1) && ( ! _info.drag)) ? MyGUI::utility::toString(_data->getCount()) : "");

			static MyGUI::ResourceImageSetPtr resource_back = nullptr;
			static MyGUI::ResourceImageSetPtr resource_select = nullptr;
			if (resource_back == nullptr)
				resource_back = MyGUI::ResourceManager::getInstance().getByName("pic_ItemBackImage")->castType<MyGUI::ResourceImageSet>();
			if (resource_select == nullptr)
				resource_select = MyGUI::ResourceManager::getInstance().getByName("pic_ItemSelectImage")->castType<MyGUI::ResourceImageSet>();

			mImageBack->setItemResourcePtr(resource_back);
			mImageBack->setItemGroup("States");
			mImageBorder->setItemResourcePtr(resource_select);
			mImageBorder->setItemGroup("States");
		}

		if (_info.drag)
		{
			mImageBack->setItemName("None");
			mImageBorder->setItemName("None");

			if (!_data->isEmpty())
			{
				if (_info.drop_refuse)
					mImageItem->setItemName("Refuse");
				else if (_info.drop_accept)
					mImageItem->setItemName("Accept");
				else
					mImageItem->setItemName("Normal");
				mImageItem->setVisible(true);
			}
			else
			{
				mImageItem->setVisible(false);
			}
		}
		else
		{
			if (_info.active)
			{
				if (_info.select)
					mImageBack->setItemName("Select");
				else
					mImageBack->setItemName("Active");
			}
			else if (_info.select)
				mImageBack->setItemName("Pressed");
			else
				mImageBack->setItemName("Normal");

			if (_info.drop_refuse)
			{
				mImageBorder->setItemName("Refuse");
				mTextFront->setTextColour(MyGUI::Colour::Red);
			}
			else if (_info.drop_accept)
			{
				mImageBorder->setItemName("Accept");
				mTextFront->setTextColour(MyGUI::Colour::Green);
			}
			else
			{
				mImageBorder->setItemName("Normal");
				mTextFront->setTextColour(MyGUI::Colour::White);
			}

			if (!_data->isEmpty())
			{
				mImageItem->setItemName("Normal");
				mImageItem->setVisible(true);
			}
			else
			{
				mImageItem->setVisible(false);
			}
		}
	}

} // namespace demo
