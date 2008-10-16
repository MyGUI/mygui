/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "DemoKeeper.h"
#include "CellView.h"

namespace demo
{

	void DemoKeeper::notifyStartDrop(BaseLayout * _sender, ItemDropInfo _info, bool & _result)
	{
		if (_info.sender_index != MyGUI::ITEM_NONE) {
			ItemData * data = *((BaseItemBox<CellView>*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			_result = !data->isEmpty();
		}
	}

	void DemoKeeper::notifyRequestDrop(BaseLayout * _sender, ItemDropInfo _info, bool & _result)
	{
		// не на айтем кидаем
		if (_info.reseiver_index == MyGUI::ITEM_NONE) {
			_result = false;
			return;
		}

		// на себя и на тотже айтем
		if ((_info.sender == _info.reseiver) && (_info.sender_index == _info.reseiver_index)) {
			_result = false;
			return;
		}

		ItemData * sender_data = *((BaseItemBox<CellView>*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
		ItemData * reseiver_data = *((BaseItemBox<CellView>*)_info.reseiver)->getItemDataAt<ItemData*>(_info.reseiver_index);

		_result = reseiver_data->isEmpty() || reseiver_data->compare(sender_data);
	}

	void DemoKeeper::notifyEndDrop(BaseLayout * _sender, ItemDropInfo _info, bool _result)
	{
		if (_result) {

			ItemData * sender_data = *((BaseItemBox<CellView>*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			ItemData * reseiver_data = *((BaseItemBox<CellView>*)_info.reseiver)->getItemDataAt<ItemData*>(_info.reseiver_index);

			reseiver_data->add(sender_data);
			sender_data->clear();

			
			((BaseItemBox<CellView>*)_info.reseiver)->setItemData(_info.reseiver_index, reseiver_data);
			((BaseItemBox<CellView>*)_info.sender)->setItemData(_info.sender_index, sender_data);
		}

	}

	void DemoKeeper::notifyNotifyItem(BaseLayout * _sender, const MyGUI::NotifyItemData & _info)
	{
		/*if (_info.index != MyGUI::ITEM_NONE) {
			if (_info.notify == MyGUI::NotifyItem::NOTIFY_MOUSE_RELEASED) {
				MyGUI::ItemBoxPtr sender = ((BaseItemBox<CellView>*)_sender)->getItemBox();
				//sender->
				sender->setPosition(sender->getCoord());
			}
		}*/
	}

	void DemoKeeper::notifyDropState(BaseLayout * _sender, MyGUI::DropItemState _state)
	{
		/*if (_state == MyGUI::DropItemState::Refuse) MyGUI::PointerManager::getInstance().setPointer("drop_refuse", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Accept) MyGUI::PointerManager::getInstance().setPointer("drop_accept", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Miss) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Start) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::End) MyGUI::PointerManager::getInstance().setDefaultPointer();*/
	}

	void DemoKeeper::createScene()
	{
		// регестрируем тип нашего ресурса
		demo::ResourceItemInfo::registryType();

		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_ItemBox");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Icons");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		MyGUI::Gui::getInstance().load("Crystal_Clear_Resources.xml");

		MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();
		int width = (int)gui->getViewWidth();
		int height = (int)gui->getViewHeight();


		mToolTip.initialise();
		mToolTip.hide();

		mItemBoxV.initialise();
		mItemBoxV.addItem(new ItemData());
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxV.addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxV.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxV.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxV.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxV.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxV.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxV.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);

		mItemBoxH.initialise();
		mItemBoxH.addItem(new ItemData());
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxH.addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxH.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxH.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxH.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxH.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxH.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxH.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);


	}

	void DemoKeeper::destroyScene()
	{
		// тип нашего ресурса
		demo::ResourceItemInfo::unregistryType();

		MyGUI::ItemBoxPtr box = mItemBoxH.getItemBox()->castType<MyGUI::ItemBox>();
		size_t count = box->getItemCount();
		for (size_t pos=0; pos<count; ++pos) {
			delete *box->getItemDataAt<ItemData*>(pos);
		}
		mItemBoxH.shutdown();

		box = mItemBoxV.getItemBox()->castType<MyGUI::ItemBox>();
		count = box->getItemCount();
		for (size_t pos=0; pos<count; ++pos) {
			delete *box->getItemDataAt<ItemData*>(pos);
		}
		mItemBoxV.shutdown();

		mToolTip.shutdown();
	}

	void DemoKeeper::notifyToolTip(BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemData * _data)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show) {
			mToolTip.show(_data, _info.point);
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide) {
			mToolTip.hide();
		}
	}

} // namespace demo
