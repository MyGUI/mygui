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

	WidgetsWindow::WidgetsWindow(MyGUI::Widget* _parent) :
		BaseLayout("WidgetsWindow.layout", _parent),
		mTabSkins(nullptr),
		mPopupMode(nullptr),
		mWidgetsButtonWidth(0),
		mWidgetsButtonHeight(0),
		mWidgetsButtonOffset(0)
	{
		assignWidget(mTabSkins, "tabSkins");
		assignWidget(mPopupMode, "PopupMode");

		mPopupMode->eventMouseButtonClick += MyGUI::newDelegate(this, &WidgetsWindow::notifyMouseButtonClickPopupMode);

		mWidgetsButtonWidth = SettingsManager::getInstance().getValue<int>("Controls/WidgetsWindow/WidgetsButtonWidth");
		mWidgetsButtonHeight = SettingsManager::getInstance().getValue<int>("Controls/WidgetsWindow/WidgetsButtonHeight");
		mWidgetsButtonOffset = SettingsManager::getInstance().getValue<int>("Controls/WidgetsWindow/WidgetsButtonOffset");
		mSkinSheetName = SettingsManager::getInstance().getValue("Controls/WidgetsWindow/LastSkinGroup");

		if (!SettingsManager::getInstance().tryGetValue("Controls/WidgetsWindow/ButtonSkinName", mButtonSkinName))
			mButtonSkinName = "Button";

		initialise();

		WidgetCreatorManager::getInstance().eventChangeCreatorMode += MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeCreatorMode);
	}

	WidgetsWindow::~WidgetsWindow()
	{
		WidgetCreatorManager::getInstance().eventChangeCreatorMode -= MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeCreatorMode);

		size_t sheet_index = mTabSkins->getIndexSelected();
		if (sheet_index != MyGUI::ITEM_NONE)
			mSkinSheetName = mTabSkins->getItemNameAt(sheet_index);
		else
			mSkinSheetName = "";

		SettingsManager::getInstance().setValue("Controls/WidgetsWindow/LastSkinGroup", mSkinSheetName);

		mPopupMode->eventMouseButtonClick -= MyGUI::newDelegate(this, &WidgetsWindow::notifyMouseButtonClickPopupMode);
	}

	void WidgetsWindow::initialise()
	{
		const SkinGroups& groups = WidgetTypes::getInstance().getSkinGroups();
		for (SkinGroups::const_iterator iter = groups.begin(); iter != groups.end(); ++iter)
		{
			MyGUI::TabItem* page = mTabSkins->addItem(iter->first);
			MyGUI::ItemBox* box = page->createWidget<MyGUI::ItemBox>("ItemBoxEmpty", MyGUI::IntCoord(0, 0, page->getSize().width, page->getSize().height), MyGUI::Align::Stretch);
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
			EditorToolTip::getInstancePtr()->show(data);
			EditorToolTip::getInstancePtr()->move(_info.point);
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide)
		{
			EditorToolTip::getInstancePtr()->hide();
		}
		else if (_info.type == MyGUI::ToolTipInfo::Move)
		{
			EditorToolTip::getInstancePtr()->move(_info.point);
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

		mPopupMode->setStateSelected(WidgetCreatorManager::getInstance().getPopupMode());
	}

	void WidgetsWindow::requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
	{
		MyGUI::Button* button = _item->createWidget<MyGUI::Button>(mButtonSkinName, MyGUI::IntCoord(
			mWidgetsButtonOffset,
			mWidgetsButtonOffset,
			_item->getWidth() - mWidgetsButtonOffset - mWidgetsButtonOffset,
			_item->getHeight() - mWidgetsButtonOffset - mWidgetsButtonOffset),
			MyGUI::Align::Stretch);

		button->setTextAlign(MyGUI::Align::Center);
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetType);
		button->setNeedToolTip(true);
		button->eventToolTip += MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);

		_item->setUserData(button);
	}

	void WidgetsWindow::requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
	{
		_coord.set(0, 0, mWidgetsButtonWidth + mWidgetsButtonOffset + mWidgetsButtonOffset, mWidgetsButtonHeight + mWidgetsButtonOffset + mWidgetsButtonOffset);
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

	void WidgetsWindow::notifyMouseButtonClickPopupMode(MyGUI::Widget* _sender)
	{
		if (mPopupMode->getStateSelected())
			WidgetCreatorManager::getInstance().setPopupMode(false);
		else
			WidgetCreatorManager::getInstance().setPopupMode(true);
	}

}
