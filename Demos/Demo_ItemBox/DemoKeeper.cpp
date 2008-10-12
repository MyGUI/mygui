/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"
#include "CellView.h"

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
		((BaseItemBox<CellView>*)_sender)->removeItem(_info.index);
	}*/
}

void DemoKeeper::notifyDropState(BaseLayout * _sender, MyGUI::DropState _state)
{
	if (_state == MyGUI::DROP_REFUSE) MyGUI::PointerManager::getInstance().setPointer("RF_drop_refuse", _sender->mainWidget());
	else if (_state == MyGUI::DROP_ACCEPT) MyGUI::PointerManager::getInstance().setPointer("RF_drop_accept", _sender->mainWidget());
	else if (_state == MyGUI::DROP_MISS) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender->mainWidget());
	else if (_state == MyGUI::DROP_START) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender->mainWidget());
	else if (_state == MyGUI::DROP_END) MyGUI::PointerManager::getInstance().setDefaultPointer();
}

void DemoKeeper::start()
{
	// регестрируем тип нашего ресурса
	demo::ResourceItemInfo::registryType();

	// загружаем ресурсы для демо
	// потом сделать и для мака
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_ItemBox", "FileSystem", "General");

	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();
	int width = (int)gui->getViewWidth();
	int height = (int)gui->getViewHeight();

	gui->load("RF.xml");
	
	MyGUI::StaticImagePtr back = gui->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, width, height), MyGUI::Align::Stretch, "Back");
	back->setImageTexture("RF.jpg");

	mToolTip.initialise();
	mToolTip.hide();

	mItemBoxV.initialise();
	mItemBoxV.addItem(new ItemData());
	mItemBoxV.addItem(new ItemData("info_RF_TypeRoll", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeCloth", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeTear", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeEye", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeEmerald", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeWings", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeIce", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeBoard", 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeBoots", 5));

	mItemBoxV.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
	mItemBoxV.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
	mItemBoxV.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
	mItemBoxV.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
	mItemBoxV.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
	mItemBoxV.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);

	mItemBoxH.initialise();
	mItemBoxH.addItem(new ItemData());
	mItemBoxH.addItem(new ItemData("info_RF_TypeRoll", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeCloth", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeTear", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeEye", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeEmerald", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeWings", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeIce", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeBoard", 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeBoots", 5));

	mItemBoxH.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
	mItemBoxH.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
	mItemBoxH.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
	mItemBoxH.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
	mItemBoxH.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
	mItemBoxH.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);


}

void DemoKeeper::end()
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
