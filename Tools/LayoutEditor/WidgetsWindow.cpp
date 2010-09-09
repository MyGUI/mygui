/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
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
		mWidgetsButtonsInOneLine(0),
		mToolTip(nullptr),
		mMaxLines(0)
	{
		assignWidget(mTabSkins, "tabSkins");

		mWidgetsButtonWidth = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonWidth");
		mWidgetsButtonHeight = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonHeight");
		mWidgetsButtonsInOneLine = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonsInOneLine");
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
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("widgetsButtonsInOneLine", mWidgetsButtonsInOneLine);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("lastSkinGroup", mSkinSheetName);
	}

	void WidgetsWindow::initialise()
	{
		int w = mWidgetsButtonWidth, h = mWidgetsButtonHeight;

		MyGUI::TabItem* sheet = nullptr;
		mMaxLines = 0;

		const SkinGroups& groups = WidgetTypes::getInstance().getSkinGroups();
		for (SkinGroups::const_iterator iter = groups.begin(); iter != groups.end(); ++iter)
		{
			sheet = mTabSkins->addItem(iter->first);
			int i = 0;
			for (VectorSkinInfo::const_iterator iterSkin = iter->second.begin(); iterSkin != iter->second.end(); ++iterSkin)
			{
				MyGUI::Button* button = sheet->createWidget<MyGUI::Button>("ButtonSmall",
					i % mWidgetsButtonsInOneLine * w + MARGIN, i / mWidgetsButtonsInOneLine * h + MARGIN, w, h,
					MyGUI::Align::Top | MyGUI::Align::Left, MyGUI::utility::toString(iterSkin->widget_type, iterSkin->widget_skin));
				button->setCaption(iterSkin->widget_button_name);
				button->setTextAlign(MyGUI::Align::Center);
				button->setUserString("widget", iterSkin->widget_type);
				button->setUserString("skin", iterSkin->widget_skin);
				mWidgets.push_back(button);

				MyGUI::ResourceSkin* skin_info = MyGUI::SkinManager::getInstance().getByName(iterSkin->widget_skin);
				MyGUI::IntSize skinDefaultSize;
				if (skin_info != nullptr) skinDefaultSize = skin_info->getSize();

				button->setUserString("width", MyGUI::utility::toString(skinDefaultSize.width));
				button->setUserString("height", MyGUI::utility::toString(skinDefaultSize.height));

				button->eventMouseButtonClick += MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetType);
				button->setNeedToolTip(true);
				button->eventToolTip += MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);
				i++;
			}
			mMaxLines = std::max((i + mWidgetsButtonsInOneLine - 1) / mWidgetsButtonsInOneLine, mMaxLines);
		}
	}

	void WidgetsWindow::notifySelectWidgetType(MyGUI::Widget* _sender)
	{
		if (_sender->castType<MyGUI::Button>()->getStateSelected())
			WidgetCreatorManager::getInstance().resetCreatorInfo();
		else
			WidgetCreatorManager::getInstance().setCreatorInfo(_sender->getUserString("widget"), _sender->getUserString("skin"));
	}

	void WidgetsWindow::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show)
		{
			mToolTip->show(_sender);
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
		if (_modeCreate)
		{
			for (MyGUI::VectorWidgetPtr::iterator item = mWidgets.begin(); item != mWidgets.end(); ++item)
			{
				bool selected = 
					(*item)->getUserString("widget") == WidgetCreatorManager::getInstance().getWidgetType() &&
					(*item)->getUserString("skin") == WidgetCreatorManager::getInstance().getWidgetSkin();
				(*item)->castType<MyGUI::Button>()->setStateSelected(selected);
			}
		}
		else
		{
			for (MyGUI::VectorWidgetPtr::iterator item = mWidgets.begin(); item != mWidgets.end(); ++item)
			{
				(*item)->castType<MyGUI::Button>()->setStateSelected(false);
			}
		}
	}

} // namespace tools
