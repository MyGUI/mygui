/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "WidgetsWindow.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"

const int MARGIN = 2;

WidgetsWindow::WidgetsWindow() :
	BaseLayout("WidgetsWindow.layout")
{
	current_widget = nullptr;
	assignWidget(mTabSkins, "tabSkins");

	widgetsButtonWidth = MyGUI::utility::parseInt(tools::SettingsManager::getInstance().getProperty("WidgetsWindow", "widgetsButtonWidth"));
	widgetsButtonHeight = MyGUI::utility::parseInt(tools::SettingsManager::getInstance().getProperty("WidgetsWindow", "widgetsButtonHeight"));
	widgetsButtonsInOneLine = MyGUI::utility::parseInt(tools::SettingsManager::getInstance().getProperty("WidgetsWindow", "widgetsButtonsInOneLine"));
	skinSheetName = tools::SettingsManager::getInstance().getProperty("WidgetsWindow", "lastSkinGroup");

	tools::WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeSelectedWidget);
}

WidgetsWindow::~WidgetsWindow()
{
	tools::WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WidgetsWindow::notifyChangeSelectedWidget);

	size_t sheet_index = mTabSkins->getIndexSelected();
	if (sheet_index != MyGUI::ITEM_NONE)
		skinSheetName = mTabSkins->getItemNameAt(sheet_index);
	else
		skinSheetName = "";

	tools::SettingsManager::getInstance().setProperty("WidgetsWindow", "widgetsButtonWidth", MyGUI::utility::toString(widgetsButtonWidth));
	tools::SettingsManager::getInstance().setProperty("WidgetsWindow", "widgetsButtonHeight", MyGUI::utility::toString(widgetsButtonHeight));
	tools::SettingsManager::getInstance().setProperty("WidgetsWindow", "widgetsButtonsInOneLine", MyGUI::utility::toString(widgetsButtonsInOneLine));
	tools::SettingsManager::getInstance().setProperty("WidgetsWindow", "lastSkinGroup", skinSheetName);
}

void WidgetsWindow::updateSize()
{
	if (mTabSkins->getItemCount() == 0)
	{
		mMainWidget->setVisible(false);
		return;
	}

	int w = widgetsButtonWidth, h = widgetsButtonHeight;
	MyGUI::TabItem* sheet = mTabSkins->getItemAt(0);

	const MyGUI::IntSize& sheet_size = sheet->getSize();
	int width = mTabSkins->getWidth() - sheet_size.width;
	int height = mTabSkins->getHeight() - sheet_size.height;

	mTabSkins->setSize(width + widgetsButtonsInOneLine * w + 2*MARGIN, height + mMaxLines*h + 2*MARGIN);

	// выбрать вкладку с прошлого раза
	size_t index = mTabSkins->findItemIndexWith(skinSheetName);
	if (index != MyGUI::ITEM_NONE) mTabSkins->setIndexSelected(index);

	width = mMainWidget->getWidth() - mMainWidget->getClientCoord().width;
	height = mMainWidget->getHeight() - mMainWidget->getClientCoord().height;

	const MyGUI::IntSize& size = mMainWidget->getParentSize();
	mMainWidget->setCoord(0, size.height - (height + mTabSkins->getHeight()), width + mTabSkins->getWidth(), height + mTabSkins->getHeight());

	mMainWidget->setVisible(true);
}

void WidgetsWindow::initialise()
{
	int w = widgetsButtonWidth, h = widgetsButtonHeight;

	MyGUI::TabItem* sheet = nullptr;
	mMaxLines = 0;
	for (SkinGroups::iterator iter = WidgetTypes::getInstance().skin_groups.begin(); iter != WidgetTypes::getInstance().skin_groups.end(); ++iter)
	{
		sheet = mTabSkins->addItem(iter->first);
		int i = 0;
		for (VectorSkinInfo::iterator iterSkin = iter->second.begin(); iterSkin != iter->second.end(); ++iterSkin)
		{
			MyGUI::Button* button = sheet->createWidget<MyGUI::Button>("ButtonSmall",
				i%widgetsButtonsInOneLine * w + MARGIN, i/widgetsButtonsInOneLine * h + MARGIN, w, h,
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
		mMaxLines = std::max((i+widgetsButtonsInOneLine-1)/widgetsButtonsInOneLine, mMaxLines);
	}

	updateSize();
}

void WidgetsWindow::clearNewWidget()
{
	new_widget_type = "";
	new_widget_skin = "";
	creating_status = 0;
}

void WidgetsWindow::startNewWidget(int _x1, int _y1, MyGUI::MouseButton _id)
{
	x1 = _x1;
	y1 = _y1;
	if (_id == MyGUI::MouseButton::Left && !creating_status && new_widget_type != "")
		creating_status = 1;
}

void WidgetsWindow::createNewWidget(int _x2, int _y2)
{
	x2 = _x2;
	y2 = _y2;
	MyGUI::IntCoord coord(std::min(x1, x2), std::min(y1, y2), abs(x1 - x2), abs(y1 - y2));
	if ((creating_status == 1) && ((x1-x2)*(y1-y2) != 0))
	{
		// тип виджета может отсутсвовать
		if (!MyGUI::WidgetManager::getInstance().isFactoryExist(new_widget_type))
		{
			creating_status = 0;
		}
		else
		{
			creating_status = 2;

			// внимание current_widget родитель и потом сразу же сын
			std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", new_widget_type, EditorWidgets::getInstance().global_counter);
			EditorWidgets::getInstance().global_counter++;
			// пока не найдем ближайшего над нами способного быть родителем
			while (current_widget && !WidgetTypes::getInstance().find(current_widget->getTypeName())->parent) current_widget = current_widget->getParent();
			if (current_widget && WidgetTypes::getInstance().find(new_widget_type)->child)
			{
				coord = coord - current_widget->getPosition();
				current_widget = current_widget->createWidgetT(new_widget_type, new_widget_skin, coord, MyGUI::Align::Default, tmpname);
			}
			else
			{
				current_widget = MyGUI::Gui::getInstance().createWidgetT(new_widget_type, new_widget_skin, coord, MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);
			}

			if (current_widget->isType<MyGUI::StaticText>())
				current_widget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", new_widget_skin));
		}
	}
	else if (creating_status == 2)
	{
		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setCoord(coord);
	}
}

void WidgetsWindow::finishNewWidget(int _x2, int _y2)
{
	x2 = _x2;
	y2 = _y2;
	if (creating_status > 0)
	{
		if ((x1-x2)*(y1-y2) != 0)
		{
			// создали виджет, все счастливы
			WidgetContainer * widgetContainer = new WidgetContainer(new_widget_type, new_widget_skin, current_widget);
			EditorWidgets::getInstance().add(widgetContainer);
			current_widget = nullptr;

			tools::WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);

			MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setStateSelected(false);
			new_widget_type = "";
			new_widget_skin = "";
			UndoManager::getInstance().addValue();
		}
		else
		{
			// не удалось создать, т.к. размер нулевой
			if ((creating_status > 1) && current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
			MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setStateSelected(false);
			new_widget_type = "";
			new_widget_skin = "";
			if (creating_status == 2) EditorWidgets::getInstance().global_counter--;
		}
		creating_status = 0;
	}
}

void WidgetsWindow::notifySelectWidgetType(MyGUI::Widget* _sender)
{
	new_widget_type = _sender->getUserString("widget");
	new_widget_skin = _sender->getUserString("skin");
	_sender->castType<MyGUI::Button>()->setStateSelected(true);
}

void WidgetsWindow::notifySelectWidgetTypeDoubleclick(MyGUI::Widget* _sender)
{
	new_widget_type = _sender->getUserString("widget");
	// тип виджета может отсутсвовать
	if (!MyGUI::WidgetManager::getInstance().isFactoryExist(new_widget_type))
	{
		return;
	}

	new_widget_skin = _sender->getUserString("skin");
	int width = MyGUI::utility::parseInt(_sender->getUserString("width"));
	int height = MyGUI::utility::parseInt(_sender->getUserString("height"));

	std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", new_widget_type, EditorWidgets::getInstance().global_counter);
	EditorWidgets::getInstance().global_counter++;

	while (current_widget && !WidgetTypes::getInstance().find(current_widget->getTypeName())->parent) current_widget = current_widget->getParent();

	MyGUI::IntSize parent_size;
	if (current_widget && WidgetTypes::getInstance().find(new_widget_type)->child)
	{
		parent_size = current_widget->getSize();
		current_widget = current_widget->createWidgetT(new_widget_type, new_widget_skin, MyGUI::IntCoord(), MyGUI::Align::Default, tmpname);
	}
	else
	{
		parent_size = MyGUI::RenderManager::getInstance().getViewSize();
		current_widget = MyGUI::Gui::getInstance().createWidgetT(new_widget_type, new_widget_skin, MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);
	}
	// place in parent center
	const MyGUI::IntCoord size((parent_size.width - width)/2, (parent_size.height - height)/2, width, height);
	current_widget->setCoord(size);
	if (current_widget->isType<MyGUI::StaticText>())
		current_widget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", new_widget_skin));

	WidgetContainer * widgetContainer = new WidgetContainer(new_widget_type, new_widget_skin, current_widget);
	EditorWidgets::getInstance().add(widgetContainer);
	current_widget = nullptr;

	tools::WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);

	MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setStateSelected(false);
	new_widget_type = "";
	new_widget_skin = "";

	UndoManager::getInstance().addValue();
}

void WidgetsWindow::clearAllSheets()
{
	mTabSkins->removeAllItems();
	updateSize();
}

void WidgetsWindow::notifyChangeSelectedWidget(MyGUI::Widget* _current_widget)
{
	current_widget = _current_widget;
}
