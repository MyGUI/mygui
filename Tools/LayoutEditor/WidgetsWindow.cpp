/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "Common.h"
#include "WidgetsWindow.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "SettingsManager.h"
#include "WidgetCreatorManager.h"

namespace tools
{
	const int MARGIN = 2;

	WidgetsWindow::WidgetsWindow(MyGUI::Widget* _parent) :
		BaseLayout("WidgetsWindow.layout", _parent),
		mTabSkins(nullptr),
		mWidgetsButtonWidth(0),
		mWidgetsButtonHeight(0),
		mToolTip(nullptr)
	{
		assignWidget(mTabSkins, "tabSkins");

		mWidgetsButtonWidth = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonWidth");
		mWidgetsButtonHeight = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonHeight");
		mSkinSheetName = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue("lastSkinGroup");

		mToolTip = new EditorToolTip();

		initialise();

		WidgetCreatorManager::getInstance().eventChangeCreatorMode += MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeCreatorMode);
	}

	WidgetsWindow::~WidgetsWindow()
	{
		WidgetCreatorManager::getInstance().eventChangeCreatorMode -= MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeCreatorMode);

		delete mToolTip;
		mToolTip = nullptr;

		size_t sheet_index = mTabSkins->getIndexSelected();
		if (sheet_index != MyGUI::ITEM_NONE)
			mSkinSheetName = mTabSkins->getItemNameAt(sheet_index);
		else
			mSkinSheetName = "";

		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("widgetsButtonWidth", mWidgetsButtonWidth);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("widgetsButtonHeight", mWidgetsButtonHeight);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("lastSkinGroup", mSkinSheetName);
	}

	void WidgetsWindow::initialise()
	{
		const SkinGroups& groups = WidgetTypes::getInstance().getSkinGroups();
		for (SkinGroups::const_iterator iter = groups.begin(); iter != groups.end(); ++iter)
		{
			MyGUI::TabItem* page = mTabSkins->addItem(iter->first);
			MyGUI::ItemBox* box = page->createWidget<MyGUI::ItemBox>("ItemBoxEmptyV", MyGUI::IntCoord(0, 0, page->getSize().width, page->getSize().height), MyGUI::Align::Stretch);
			mItemBoxs.push_back(box);

			box->requestCreateWidgetItem = MyGUI::newDelegate(this, &WidgetsWindow::requestCreateWidgetItem);
			box->requestCoordItem = MyGUI::newDelegate(this, &WidgetsWindow::requestCoordItem);
			box->requestDrawItem = MyGUI::newDelegate(this, &WidgetsWindow::requestDrawItem);

			for (VectorSkinInfo::const_iterator iterSkin = iter->second.begin(); iterSkin != iter->second.end(); ++iterSkin)
			{
				box->addItem(*iterSkin);
			}
		}
	}

	bool WidgetsWindow::getCellSelected(MyGUI::Widget* _widget)
	{
		MyGUI::Widget* container = _widget->getParent()->_getContainer();
		MyGUI::ItemBox* box = container->castType<MyGUI::ItemBox>();
		return box->getIndexSelected() == box->getIndexByWidget(_widget->getParent());
	}

	SkinInfo WidgetsWindow::getCellData(MyGUI::Widget* _widget)
	{
		MyGUI::Widget* container = _widget->getParent()->_getContainer();
		MyGUI::ItemBox* box = container->castType<MyGUI::ItemBox>();
		size_t index = box->getIndexByWidget(_widget->getParent());
		return *box->getItemDataAt<SkinInfo>(index);
	}

	void WidgetsWindow::notifySelectWidgetType(MyGUI::Widget* _sender)
	{
		SkinInfo data = getCellData(_sender);

		if (getCellSelected(_sender))
			WidgetCreatorManager::getInstance().resetCreatorInfo();
		else
			WidgetCreatorManager::getInstance().setCreatorInfo(data.widget_type, data.widget_skin);
	}

	void WidgetsWindow::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show)
		{
			SkinInfo data = getCellData(_sender);
			mToolTip->show(data);
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

	void WidgetsWindow::notifyChangeCreatorMode(bool _modeCreate)
	{
		const std::string& widgetType = WidgetCreatorManager::getInstance().getWidgetType();
		const std::string& widgetSkin = WidgetCreatorManager::getInstance().getWidgetSkin();

		for (VectorItemBox::iterator item = mItemBoxs.begin(); item != mItemBoxs.end(); ++item)
		{
			MyGUI::ItemBox* box = (*item);
			box->setIndexSelected(MyGUI::ITEM_NONE);
			for (size_t index = 0; index < box->getItemCount(); ++index)
			{
				SkinInfo* info = box->getItemDataAt<SkinInfo>(index);
				if (info->widget_skin == widgetSkin && info->widget_type == widgetType)
				{
					box->setIndexSelected(index);
					break;
				}
			}
		}
	}

	void WidgetsWindow::requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
	{
		MyGUI::Button* button = _item->createWidget<MyGUI::Button>("ButtonSmall", MyGUI::IntCoord(0, 0, _item->getWidth(), _item->getHeight()), MyGUI::Align::Stretch);

		button->setTextAlign(MyGUI::Align::Center);
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetType);
		button->setNeedToolTip(true);
		button->eventToolTip += MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);

		_item->setUserData(button);
	}

	void WidgetsWindow::requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
	{
		_coord.set(0, 0, mWidgetsButtonWidth, mWidgetsButtonHeight);
	}

	void WidgetsWindow::requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
	{
		MyGUI::Button* button = *_item->getUserData<MyGUI::Button*>();
		SkinInfo data = *_sender->getItemDataAt<SkinInfo>(_info.index);
		if (_info.update)
		{
			button->setCaption(data.widget_button_name);
		}

		button->setStateSelected(_info.select);
	}

} // namespace tools
