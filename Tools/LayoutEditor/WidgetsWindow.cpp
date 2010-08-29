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
#include "WidgetSelectorManager.h"

namespace tools
{
	const int MARGIN = 2;

	WidgetsWindow::WidgetsWindow() :
		BaseLayout("WidgetsWindow.layout"),
		mToolTip(nullptr),
		mCreatingStatus(0)
	{
		mCurrentWidget = nullptr;
		assignWidget(mTabSkins, "tabSkins");

		mWidgetsButtonWidth = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonWidth");
		mWidgetsButtonHeight = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonHeight");
		mWidgetsButtonsInOneLine = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue<int>("widgetsButtonsInOneLine");
		mSkinSheetName = SettingsManager::getInstance().getSector("WidgetsWindow")->getPropertyValue("lastSkinGroup");

		mToolTip = new EditorToolTip();

		setEdgeHideController();

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeSelectedWidget);
	}

	WidgetsWindow::~WidgetsWindow()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeSelectedWidget);

		delete mToolTip;
		mToolTip = nullptr;

		size_t sheet_index = mTabSkins->getIndexSelected();
		if (sheet_index != MyGUI::ITEM_NONE)
			mSkinSheetName = mTabSkins->getItemNameAt(sheet_index);
		else
			mSkinSheetName = "";

		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("mWidgetsButtonWidth", mWidgetsButtonWidth);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("mWidgetsButtonHeight", mWidgetsButtonHeight);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("mWidgetsButtonsInOneLine", mWidgetsButtonsInOneLine);
		SettingsManager::getInstance().getSector("WidgetsWindow")->setPropertyValue("lastSkinGroup", mSkinSheetName);
	}

	void WidgetsWindow::updateSize()
	{
		if (mTabSkins->getItemCount() == 0)
		{
			mMainWidget->setVisible(false);
			return;
		}

		int w = mWidgetsButtonWidth, h = mWidgetsButtonHeight;
		MyGUI::TabItem* sheet = mTabSkins->getItemAt(0);

		const MyGUI::IntSize& sheet_size = sheet->getSize();
		int width = mTabSkins->getWidth() - sheet_size.width;
		int height = mTabSkins->getHeight() - sheet_size.height;

		mTabSkins->setSize(width + mWidgetsButtonsInOneLine * w + 2*MARGIN, height + mMaxLines*h + 2*MARGIN);

		// выбрать вкладку с прошлого раза
		size_t index = mTabSkins->findItemIndexWith(mSkinSheetName);
		if (index != MyGUI::ITEM_NONE) mTabSkins->setIndexSelected(index);

		width = mMainWidget->getWidth() - mMainWidget->getClientCoord().width;
		height = mMainWidget->getHeight() - mMainWidget->getClientCoord().height;

		const MyGUI::IntSize& size = mMainWidget->getParentSize();
		mMainWidget->setCoord(0, size.height - (height + mTabSkins->getHeight()), width + mTabSkins->getWidth(), height + mTabSkins->getHeight());

		mMainWidget->setVisible(true);
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
					i%mWidgetsButtonsInOneLine * w + MARGIN, i/mWidgetsButtonsInOneLine * h + MARGIN, w, h,
					MyGUI::Align::Top|MyGUI::Align::Left, MyGUI::utility::toString(iterSkin->widget_type, iterSkin->widget_skin));
				button->setCaption(iterSkin->widget_button_name);
				button->setTextAlign(MyGUI::Align::Center);
				button->setUserString("widget", iterSkin->widget_type);
				button->setUserString("skin", iterSkin->widget_skin);

				MyGUI::ResourceSkin* skin_info = MyGUI::SkinManager::getInstance().getByName(iterSkin->widget_skin);
				MyGUI::IntSize skinDefaultSize;
				if (skin_info != nullptr) skinDefaultSize = skin_info->getSize();

				button->setUserString("width", MyGUI::utility::toString(skinDefaultSize.width));
				button->setUserString("height", MyGUI::utility::toString(skinDefaultSize.height));

				button->eventMouseButtonClick += MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetType);
				button->eventMouseButtonDoubleClick += MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetTypeDoubleclick);
				button->setNeedToolTip(true);
				button->eventToolTip += MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);
				i++;
			}
			mMaxLines = std::max((i+mWidgetsButtonsInOneLine-1)/mWidgetsButtonsInOneLine, mMaxLines);
		}

		updateSize();
	}

	void WidgetsWindow::clearNewWidget()
	{
		mNewWidgetType = "";
		mNewWidgetSkin = "";
		mCreatingStatus = 0;
	}

	void WidgetsWindow::startNewWidget(int _x1, int _y1, MyGUI::MouseButton _id)
	{
		mX1 = _x1;
		mY1 = _y1;
		if (_id == MyGUI::MouseButton::Left && !mCreatingStatus && mNewWidgetType != "")
			mCreatingStatus = 1;
	}

	void WidgetsWindow::createNewWidget(int _x2, int _y2)
	{
		mX2 = _x2;
		mY2 = _y2;
		MyGUI::IntCoord coord(std::min(mX1, mX2), std::min(mY1, mY2), abs(mX1 - mX2), abs(mY1 - mY2));
		if ((mCreatingStatus == 1) && ((mX1-mX2)*(mY1-mY2) != 0))
		{
			// тип виджета может отсутсвовать
			if (!MyGUI::WidgetManager::getInstance().isFactoryExist(mNewWidgetType))
			{
				mCreatingStatus = 0;
			}
			else
			{
				mCreatingStatus = 2;

				// внимание mCurrentWidget родитель и потом сразу же сын
				std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", mNewWidgetType, EditorWidgets::getInstance().getNextGlobalCounter());

				// пока не найдем ближайшего над нами способного быть родителем
				while (mCurrentWidget && !WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->parent)
					mCurrentWidget = mCurrentWidget->getParent();

				if (mCurrentWidget && WidgetTypes::getInstance().findWidgetStyle(mNewWidgetType)->child)
				{
					coord = coord - mCurrentWidget->getPosition();
					mCurrentWidget = mCurrentWidget->createWidgetT(mNewWidgetType, mNewWidgetSkin, coord, MyGUI::Align::Default, tmpname);
				}
				else
				{
					mCurrentWidget = MyGUI::Gui::getInstance().createWidgetT(mNewWidgetType, mNewWidgetSkin, coord, MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);
				}

				if (mCurrentWidget->isType<MyGUI::StaticText>())
					mCurrentWidget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", mNewWidgetSkin));
			}
		}
		else if (mCreatingStatus == 2)
		{
			coord = utility::convertCoordToParentCoord(coord, mCurrentWidget);
			mCurrentWidget->setCoord(coord);
		}
	}

	void WidgetsWindow::finishNewWidget(int _x2, int _y2)
	{
		mX2 = _x2;
		mY2 = _y2;
		if (mCreatingStatus > 0)
		{
			if ((mX1-mX2)*(mY1-mY2) != 0)
			{
				// создали виджет, все счастливы
				WidgetContainer * widgetContainer = new WidgetContainer(mNewWidgetType, mNewWidgetSkin, mCurrentWidget);
				EditorWidgets::getInstance().add(widgetContainer);
				mCurrentWidget = nullptr;

				WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);

				MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(mNewWidgetType, mNewWidgetSkin))->setStateSelected(false);
				mNewWidgetType = "";
				mNewWidgetSkin = "";
				UndoManager::getInstance().addValue();
			}
			else
			{
				// не удалось создать, т.к. размер нулевой
				if ((mCreatingStatus > 1) && mCurrentWidget) MyGUI::WidgetManager::getInstance().destroyWidget(mCurrentWidget);
				MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(mNewWidgetType, mNewWidgetSkin))->setStateSelected(false);
				mNewWidgetType = "";
				mNewWidgetSkin = "";
				//FIXME ваще не вкурил
				//if (mCreatingStatus == 2) EditorWidgets::getInstance().global_counter--;
			}
			mCreatingStatus = 0;
		}
	}

	void WidgetsWindow::notifySelectWidgetType(MyGUI::Widget* _sender)
	{
		mNewWidgetType = _sender->getUserString("widget");
		mNewWidgetSkin = _sender->getUserString("skin");
		_sender->castType<MyGUI::Button>()->setStateSelected(true);
	}

	void WidgetsWindow::notifySelectWidgetTypeDoubleclick(MyGUI::Widget* _sender)
	{
		mNewWidgetType = _sender->getUserString("widget");
		// тип виджета может отсутсвовать
		if (!MyGUI::WidgetManager::getInstance().isFactoryExist(mNewWidgetType))
		{
			return;
		}

		mNewWidgetSkin = _sender->getUserString("skin");
		int width = MyGUI::utility::parseInt(_sender->getUserString("width"));
		int height = MyGUI::utility::parseInt(_sender->getUserString("height"));

		std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", mNewWidgetType, EditorWidgets::getInstance().getNextGlobalCounter());

		while (mCurrentWidget && !WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->parent)
			mCurrentWidget = mCurrentWidget->getParent();

		MyGUI::IntSize parent_size;
		if (mCurrentWidget && WidgetTypes::getInstance().findWidgetStyle(mNewWidgetType)->child)
		{
			parent_size = mCurrentWidget->getSize();
			mCurrentWidget = mCurrentWidget->createWidgetT(mNewWidgetType, mNewWidgetSkin, MyGUI::IntCoord(), MyGUI::Align::Default, tmpname);
		}
		else
		{
			parent_size = MyGUI::RenderManager::getInstance().getViewSize();
			mCurrentWidget = MyGUI::Gui::getInstance().createWidgetT(mNewWidgetType, mNewWidgetSkin, MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);
		}
		// place in parent center
		const MyGUI::IntCoord size((parent_size.width - width)/2, (parent_size.height - height)/2, width, height);
		mCurrentWidget->setCoord(size);
		if (mCurrentWidget->isType<MyGUI::StaticText>())
			mCurrentWidget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", mNewWidgetSkin));

		WidgetContainer * widgetContainer = new WidgetContainer(mNewWidgetType, mNewWidgetSkin, mCurrentWidget);
		EditorWidgets::getInstance().add(widgetContainer);
		mCurrentWidget = nullptr;

		WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);

		MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(mNewWidgetType, mNewWidgetSkin))->setStateSelected(false);
		mNewWidgetType = "";
		mNewWidgetSkin = "";

		UndoManager::getInstance().addValue();
	}

	void WidgetsWindow::clearAllSheets()
	{
		mTabSkins->removeAllItems();
		updateSize();
	}

	void WidgetsWindow::notifyChangeSelectedWidget(MyGUI::Widget* _current_widget)
	{
		mCurrentWidget = _current_widget;
	}

	void WidgetsWindow::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo & _info)
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

	void WidgetsWindow::setEdgeHideController()
	{
		bool value = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<bool>("EdgeHide");
		if (value)
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerEdgeHide::getClassTypeName());
			MyGUI::ControllerEdgeHide* controller = item->castType<MyGUI::ControllerEdgeHide>();

			controller->setTime(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<float>("EdgeHideTime"));
			controller->setRemainPixels(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("EdgeHideRemainPixels"));
			controller->setShadowSize(SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("EdgeHideShadowSize"));

			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}
	}

} // namespace tools