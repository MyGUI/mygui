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
		_result = (_info.sender_data != null) && (_info.sender_data->type != TypeNone);
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

	_result = (_info.reseiver_data->type == 0) || (_info.reseiver_data->type == _info.sender_data->type);
}

void DemoKeeper::notifyEndDrop(BaseLayout * _sender, ItemDropInfo _info, bool _result)
{
	if (_result) {
		_info.reseiver_data->type = _info.sender_data->type;
		_info.reseiver_data->count += _info.sender_data->count;

		_info.sender_data->type = TypeNone;
		_info.sender_data->count = 0;

		
		((BaseItemBox<CellView>*)_info.reseiver)->setItemData(_info.reseiver_index, _info.reseiver_data);
		((BaseItemBox<CellView>*)_info.sender)->setItemData(_info.sender_index, _info.sender_data);
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
	mItemBoxV.addItem(new ItemData("info_RF_TypeNone", TypeNone, 0));
	mItemBoxV.addItem(new ItemData("info_RF_TypeRoll", TypeRoll, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeCloth", TypeCloth, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeTear", TypeTear, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeEye", TypeEye, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeEmerald", TypeEmerald, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeWings", TypeWings, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeIce", TypeIce, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeBoard", TypeBoard, 5));
	mItemBoxV.addItem(new ItemData("info_RF_TypeBoots", TypeBoots, 5));

	mItemBoxV.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
	mItemBoxV.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
	mItemBoxV.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
	mItemBoxV.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
	mItemBoxV.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
	mItemBoxV.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);

	mItemBoxH.initialise();
	mItemBoxH.addItem(new ItemData("info_RF_TypeNone", TypeNone, 0));
	mItemBoxH.addItem(new ItemData("info_RF_TypeRoll", TypeRoll, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeCloth", TypeCloth, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeTear", TypeTear, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeEye", TypeEye, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeEmerald", TypeEmerald, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeWings", TypeWings, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeIce", TypeIce, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeBoard", TypeBoard, 5));
	mItemBoxH.addItem(new ItemData("info_RF_TypeBoots", TypeBoots, 5));

	mItemBoxH.eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
	mItemBoxH.eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
	mItemBoxH.eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
	mItemBoxH.eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
	mItemBoxH.eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
	mItemBoxH.eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);


          //ItemData data(TypeEmerald, 5, L"Изумруд", L"описание Изумруд");

}

void DemoKeeper::end()
{
	// тип нашего ресурса
	demo::ResourceItemInfo::unregistryType();
}

void DemoKeeper::notifyToolTip(BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemData * _data)
{
	if (_info.type == MyGUI::TOOLTIP_SHOW) {
		mToolTip.show(_data, _info.point);
	}
	else if (_info.type == MyGUI::TOOLTIP_HIDE) {
		mToolTip.hide();
	}
}
