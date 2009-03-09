/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "WidgetsWindow.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

const int MARGIN = 2;

WidgetsWindow::WidgetsWindow() : BaseLayout("WidgetsWindow.layout")
{
	current_widget = nullptr;
	assignWidget(mTabSkins, "tabSkins");
}

void WidgetsWindow::initialise()
{
	int w = widgetsButtonWidth, h = widgetsButtonHeight;

	MyGUI::SheetPtr sheet;
	int maxLines = 0;
	for (SkinGroups::iterator iter = WidgetTypes::getInstance().skin_groups.begin(); iter != WidgetTypes::getInstance().skin_groups.end(); ++iter)
	{
		sheet = mTabSkins->addItem(iter->first);
		int i = 0;
		for (StringPairs::iterator iterSkin = iter->second.begin(); iterSkin != iter->second.end(); ++iterSkin)
		{
			MyGUI::ButtonPtr button = sheet->createWidget<MyGUI::Button>("ButtonSmall",
				i%widgetsButtonsInOneLine * w + MARGIN, i/widgetsButtonsInOneLine * h + MARGIN, w, h,
				MyGUI::Align::Top|MyGUI::Align::Left, MyGUI::utility::toString(iterSkin->second, iterSkin->first));
			button->setCaption(iterSkin->first);
			button->setTextAlign(MyGUI::Align::Center);
			button->setUserString("widget", iterSkin->second);
			button->setUserString("skin", iterSkin->first);
			MyGUI::IntSize skinDefaultSize = MyGUI::SkinManager::getInstance().getSkin(iterSkin->first)->getSize();
			button->setUserString("width", MyGUI::utility::toString(skinDefaultSize.width));
			button->setUserString("height", MyGUI::utility::toString(skinDefaultSize.height));

			button->eventMouseButtonClick = MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetType);
			button->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &WidgetsWindow::notifySelectWidgetTypeDoubleclick);
			button->setNeedToolTip(true);
			button->eventToolTip = MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);
			i++;
		}
		maxLines = std::max((i+widgetsButtonsInOneLine-1)/widgetsButtonsInOneLine, maxLines);
	}

	int width = mTabSkins->getWidth() - sheet->getWidth();
	int height = mTabSkins->getHeight() - sheet->getHeight();
	mTabSkins->setSize(width + widgetsButtonsInOneLine * w + 2*MARGIN, height + maxLines*h + 2*MARGIN);

	// выбрать вкладку с прошлого раза
	size_t index = mTabSkins->findItemIndexWith(skinSheetName);
	if (index != MyGUI::ITEM_NONE) mTabSkins->setIndexSelected(index);

	width = mMainWidget->getWidth() - mMainWidget->getClientCoord().width;
	height = mMainWidget->getHeight() - mMainWidget->getClientCoord().height;

	const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();
	mMainWidget->setCoord(0, size.height - (height + mTabSkins->getHeight()), width + mTabSkins->getWidth(), height + mTabSkins->getHeight());
}

void WidgetsWindow::load(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
	while (field.next()) {
		std::string key, value;

		if (field->getName() == "Property")
		{
			if (false == field->findAttribute("key", key)) continue;
			if (false == field->findAttribute("value", value)) continue;

			if (key == "widgetsButtonWidth") widgetsButtonWidth = MyGUI::utility::parseInt(value);
			else if (key == "widgetsButtonHeight") widgetsButtonHeight = MyGUI::utility::parseInt(value);
			else if (key == "widgetsButtonsInOneLine") widgetsButtonsInOneLine = MyGUI::utility::parseInt(value);
			else if (key == "lastSkinGroup") skinSheetName = value;
		}
	}
}

void WidgetsWindow::save(MyGUI::xml::ElementPtr root)
{
	root = root->createChild("WidgetsWindow");
	MyGUI::xml::ElementPtr nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "widgetsButtonWidth");
	nodeProp->addAttribute("value", widgetsButtonWidth);

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "widgetsButtonHeight");
	nodeProp->addAttribute("value", widgetsButtonHeight);

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "widgetsButtonsInOneLine");
	nodeProp->addAttribute("value", widgetsButtonsInOneLine);

	nodeProp = root->createChild("Property");
	nodeProp->addAttribute("key", "lastSkinGroup");
	nodeProp->addAttribute("value", mTabSkins->getItemNameAt(mTabSkins->getIndexSelected()));
}

void WidgetsWindow::clearNewWidget()
{
	new_widget_type = "";
	new_widget_skin = "";
	creating_status = 0;
}

void WidgetsWindow::startNewWidget(int _x1, int _y1, int _id)
{
	x1 = _x1;
	y1 = _y1;
	if (_id == 0 && !creating_status && new_widget_type != "") creating_status = 1;
}

void WidgetsWindow::createNewWidget(int _x2, int _y2)
{
	x2 = _x2;
	y2 = _y2;
	MyGUI::IntCoord coord(std::min(x1, x2), std::min(y1, y2), abs(x1 - x2), abs(y1 - y2));
	if ((creating_status == 1) && ((x1-x2)*(y1-y2) != 0))
	{
		creating_status = 2;

		// внимание current_widget родитель и потом сразу же сын
		std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", new_widget_type, EditorWidgets::getInstance().global_counter);
		EditorWidgets::getInstance().global_counter++;
		// пока не найдем ближайшего над нами способного быть родителем
		while (current_widget && false == WidgetTypes::getInstance().find(current_widget->getTypeName())->parent) current_widget = current_widget->getParent();
		if (current_widget && WidgetTypes::getInstance().find(new_widget_type)->child)
		{
			coord = coord - current_widget->getPosition();
			current_widget = current_widget->createWidgetT(new_widget_type, new_widget_skin, coord, MyGUI::Align::Default, tmpname);
		}
		else current_widget = MyGUI::Gui::getInstance().createWidgetT(new_widget_type, new_widget_skin, coord, MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);

		current_widget->setCaption(MyGUI::utility::toString("#888888",new_widget_skin));
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
			eventSelectWidget(widgetContainer->widget);
			MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setButtonPressed(false);
			new_widget_type = "";
			new_widget_skin = "";
			UndoManager::getInstance().addValue();
		}
		else
		{
			// не удалось создать, т.к. размер нулевой
			if ((creating_status > 1) && current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
			MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setButtonPressed(false);
			new_widget_type = "";
			new_widget_skin = "";
			if (creating_status == 2) EditorWidgets::getInstance().global_counter--;
		}
		creating_status = 0;
	}
}

void WidgetsWindow::notifySelectWidgetType(MyGUI::WidgetPtr _sender)
{
	new_widget_type = _sender->getUserString("widget");
	new_widget_skin = _sender->getUserString("skin");
	_sender->castType<MyGUI::Button>()->setButtonPressed(true);
}

void WidgetsWindow::notifySelectWidgetTypeDoubleclick(MyGUI::WidgetPtr _sender)
{
	new_widget_type = _sender->getUserString("widget");
	new_widget_skin = _sender->getUserString("skin");
	int width = MyGUI::utility::parseInt(_sender->getUserString("width"));
	int height = MyGUI::utility::parseInt(_sender->getUserString("height"));

	std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", new_widget_type, EditorWidgets::getInstance().global_counter);
	EditorWidgets::getInstance().global_counter++;

	while (current_widget && false == WidgetTypes::getInstance().find(current_widget->getTypeName())->parent) current_widget = current_widget->getParent();
	if (current_widget && WidgetTypes::getInstance().find(new_widget_type)->child)
		current_widget = current_widget->createWidgetT(new_widget_type, new_widget_skin, 0, 0, width, height, MyGUI::Align::Default, tmpname);
	else
	{
		MyGUI::IntSize view(MyGUI::Gui::getInstance().getViewSize());
		current_widget = MyGUI::Gui::getInstance().createWidgetT(new_widget_type, new_widget_skin, MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);
		MyGUI::IntSize size(current_widget->getSize());
		current_widget->setCoord((view.width-size.width)/2, (view.height-size.height)/2, width, height);
	}
	current_widget->setCaption(MyGUI::utility::toString("#888888",new_widget_skin));

	WidgetContainer * widgetContainer = new WidgetContainer(new_widget_type, new_widget_skin, current_widget);
	EditorWidgets::getInstance().add(widgetContainer);
	current_widget = nullptr;
	eventSelectWidget(widgetContainer->widget);
	MyGUI::Gui::getInstance().findWidget<MyGUI::Button>(MyGUI::utility::toString(new_widget_type, new_widget_skin))->setButtonPressed(false);
	new_widget_type = "";
	new_widget_skin = "";

	UndoManager::getInstance().addValue();
}
