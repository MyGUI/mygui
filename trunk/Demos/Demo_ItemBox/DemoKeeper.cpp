/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "CellView.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mToolTip(nullptr),
		mItemBoxV(nullptr),
		mItemBoxH(nullptr)
	{
	}

	void DemoKeeper::notifyStartDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool& _result)
	{
		if (_info.sender_index != MyGUI::ITEM_NONE)
		{
			ItemData* data = *static_cast<ItemBox*>(_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			_result = !data->isEmpty();
		}
	}

	void DemoKeeper::notifyRequestDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool& _result)
	{
		// не на айтем кидаем
		if (_info.receiver_index == MyGUI::ITEM_NONE)
		{
			_result = false;
			return;
		}

		// на себя и на тотже айтем
		if ((_info.sender == _info.receiver) && (_info.sender_index == _info.receiver_index))
		{
			_result = false;
			return;
		}

		ItemData* sender_data = *static_cast<ItemBox*>(_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
		ItemData* receiver_data = *static_cast<ItemBox*>(_info.receiver)->getItemDataAt<ItemData*>(_info.receiver_index);

		_result = receiver_data->isEmpty() || receiver_data->compare(sender_data);
	}

	void DemoKeeper::notifyEndDrop(wraps::BaseLayout* _sender, wraps::DDItemInfo _info, bool _result)
	{
		if (_result)
		{
			ItemData* sender_data = *static_cast<ItemBox*>(_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			ItemData* receiver_data = *static_cast<ItemBox*>(_info.receiver)->getItemDataAt<ItemData*>(_info.receiver_index);

			receiver_data->add(sender_data);
			sender_data->clear();

			static_cast<ItemBox*>(_info.receiver)->setItemData(_info.receiver_index, receiver_data);
			static_cast<ItemBox*>(_info.sender)->setItemData(_info.sender_index, sender_data);
		}
	}

	void DemoKeeper::notifyNotifyItem(wraps::BaseLayout* _sender, const MyGUI::IBNotifyItemData& _info)
	{
		/*if (_info.index != MyGUI::ITEM_NONE)
		{
			if (_info.notify == MyGUI::NotifyItem::NOTIFY_MOUSE_RELEASED)
			{
				MyGUI::ItemBox* sender = ((BaseItemBox<CellView>*)_sender)->getItemBox();
				//sender->
				sender->setPosition(sender->getCoord());
			}
		}*/
	}

	void DemoKeeper::notifyDropState(wraps::BaseLayout* _sender, MyGUI::DDItemState _state)
	{
		/*if (_state == MyGUI::DDItemState::Refuse) MyGUI::PointerManager::getInstance().setPointer("drop_refuse", _sender->mainWidget());
		else if (_state == MyGUI::DDItemState::Accept) MyGUI::PointerManager::getInstance().setPointer("drop_accept", _sender->mainWidget());
		else if (_state == MyGUI::DDItemState::Miss) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DDItemState::Start) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DDItemState::End) MyGUI::PointerManager::getInstance().setDefaultPointer();*/
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_ItemBox");
		addResourceLocation(getRootMedia() + "/Icons");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("You can drag and drop items from one ItemBox to another. Hold mouse over item to see tool tip. Resize windows to see vertical and horizontal ItebBox alignments.");

		// регестрируем тип нашего ресурса
		std::string resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ResourceItemInfo>(resourceCategory);

		MyGUI::ResourceManager::getInstance().load("Resources.xml");
		//MyGUI::ResourceManager::getInstance().load("ItemBox_skin.xml");

		mToolTip = new ToolTip();
		mToolTip->hide();

		mItemBoxV = new ItemBoxWindow("ItemBoxV.layout");
		mItemBoxV->getItemBox()->addItem(new ItemData());
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxV->getItemBox()->eventStartDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxV->getItemBox()->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxV->getItemBox()->eventDropResult = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxV->getItemBox()->eventChangeDDState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxV->getItemBox()->eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxV->getItemBox()->eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);

		mItemBoxH = new ItemBoxWindow("ItemBoxH.layout");
		mItemBoxH->getItemBox()->addItem(new ItemData());
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxH->getItemBox()->eventStartDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxH->getItemBox()->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxH->getItemBox()->eventDropResult = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxH->getItemBox()->eventChangeDDState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxH->getItemBox()->eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxH->getItemBox()->eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);
	}

	void DemoKeeper::destroyScene()
	{
		// тип нашего ресурса
		std::string resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceItemInfo>(resourceCategory);

		delete mItemBoxH;
		mItemBoxH = nullptr;
		delete mItemBoxV;
		mItemBoxV = nullptr;
		delete mToolTip;
		mToolTip = nullptr;
	}

	void DemoKeeper::notifyToolTip(wraps::BaseLayout* _sender, const MyGUI::ToolTipInfo& _info, ItemData* _data)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show)
		{
			mToolTip->show(_data);
			mToolTip->move(_info.point);
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide)
		{
			mToolTip->hide();
		}
		else if (_info.type == MyGUI::ToolTipInfo::Move)
		{
			mToolTip->move(_info.point);
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
