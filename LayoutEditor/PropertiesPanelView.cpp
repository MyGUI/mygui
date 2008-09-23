/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#include "PropertiesPanelView.h"
#include "WidgetContainer.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

int grid_step;//FIXME_HOOK
inline int toGrid(int _x){ return _x / grid_step * grid_step; }

const std::string DEFAULT_VALUE = "#444444[DEFAULT]";

inline const Ogre::UTFString localise(const Ogre::UTFString & _str) {
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PropertiesPanelView::PropertiesPanelView()
{
}

void PropertiesPanelView::initialise()
{
	mLayoutName = "PropertiesPanelView.layout";
	
	PanelViewWindow::initialise();

	addItem(&mPanelMainProperties);
	mPanelMainProperties.eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
	mPanelMainProperties.eventSetPositionText = MyGUI::newDelegate(this, &PropertiesPanelView::setPositionText);
	addItem(&mPanelTypeProperties);
	mPanelTypeProperties.eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
	addItem(&mPanelGeneralProperties);
	mPanelGeneralProperties.eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
	addItem(&mPanelEvents);
	mPanelEvents.eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
	addItem(&mPanelItems);
	addItem(&mPanelUserData);

	// create widget rectangle
	current_widget_rectangle = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("StretchRectangle", MyGUI::IntCoord(), MyGUI::Align::Default, "LayoutEditor_Rectangle");
	current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &PropertiesPanelView::notifyRectangleResize);
	current_widget_rectangle->eventMouseButtonDoubleClick = newDelegate(&mPanelItems, &PanelItems::notifyRectangleDoubleClick);
	current_widget_rectangle->eventKeyButtonPressed = newDelegate(this, &PropertiesPanelView::notifyRectangleKeyPressed);

	arrow_move = false;
}

void PropertiesPanelView::notifyRectangleResize(MyGUI::WidgetPtr _sender)
{
	if (!_sender->isShow()) return;
	// найдем соответствующий контейнер виджета и переместим/растянем
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	if (WidgetTypes::getInstance().find(current_widget->getTypeName())->resizeable)
	{
		MyGUI::IntCoord coord = _sender->getCoord();
		MyGUI::IntCoord old_coord = current_widget->getAbsoluteCoord();
		// align to grid
		if (!MyGUI::InputManager::getInstance().isShiftPressed() && !arrow_move){
			if ((old_coord.width == coord.width) && (old_coord.height == coord.height)) // если только перемещаем
			{
				coord.left = toGrid(coord.left + grid_step-1 - old_coord.left) + old_coord.left;
				coord.top = toGrid(coord.top + grid_step-1 - old_coord.top) + old_coord.top;
			}
			else // если растягиваем
			{
				if (old_coord.left != coord.left){
					coord.left = toGrid(coord.left + grid_step-1);
					coord.width = old_coord.right() - coord.left;
				}else if (old_coord.width != coord.width){
					coord.width = toGrid(coord.width + old_coord.left) - old_coord.left;
				}

				if (old_coord.top != coord.top){
					coord.top = toGrid(coord.top + grid_step-1);
					coord.height = old_coord.bottom() - coord.top;
				}else if (old_coord.height != coord.height){
					coord.height = toGrid(coord.height + old_coord.top) - old_coord.top;
				}
			}
		}
		arrow_move = false;

		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
		// update coord because of current_widget can have MinMax size
		coord = current_widget->getCoord();
		setPositionText(widgetContainer->position());

		UndoManager::getInstance().addValue(PR_POSITION);
	}
	current_widget_rectangle->setPosition(current_widget->getAbsoluteCoord());
}

void PropertiesPanelView::notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
	MyGUI::IntPoint delta;
	int k = MyGUI::InputManager::getInstance().isShiftPressed() ? 1 : grid_step;
	if (OIS::KC_TAB == _key)
	{
		if ((null != current_widget->getParent()) && (current_widget->getParent()->getTypeName() == "Tab")) update(current_widget->getParent());
		if (current_widget->getTypeName() == "Tab")
		{
			MyGUI::TabPtr tab = current_widget->castType<MyGUI::Tab>();
			size_t sheet = tab->getSelectSheetIndex();
			sheet++;
			if (sheet >= tab->getSheetCount()) sheet = 0;
			if (tab->getSheetCount()) tab->selectSheetIndex(sheet);
		}
	}
	else if (OIS::KC_DELETE == _key)
	{
		if (current_widget){ EditorWidgets::getInstance().remove(current_widget); update(null); }
		UndoManager::getInstance().addValue();
	}
	else if (OIS::KC_LEFT == _key)
	{
		delta = MyGUI::IntPoint(-k, 0);
	}
	else if (OIS::KC_RIGHT == _key)
	{
		delta = MyGUI::IntPoint(k, 0);
	}
	else if (OIS::KC_UP == _key)
	{
		delta = MyGUI::IntPoint(0, -k);
	}
	else if (OIS::KC_DOWN == _key)
	{
		delta = MyGUI::IntPoint(0, k);
	}

	if (delta != MyGUI::IntPoint())
	{
		arrow_move = true;
		current_widget_rectangle->setPosition(current_widget_rectangle->getPosition() + delta);
		notifyRectangleResize(current_widget_rectangle);
		UndoManager::getInstance().addValue(PR_KEY_POSITION);
	}
}

void PropertiesPanelView::update(MyGUI::WidgetPtr _current_widget)
{
	current_widget = _current_widget;

	if (null == current_widget)
		current_widget_rectangle->hide();
	else
	{
		MyGUI::LayerManager::getInstance().upLayerItem(current_widget);
		MyGUI::IntCoord coord = current_widget->getCoord();
		MyGUI::WidgetPtr parent = current_widget->getParent();
		if (null != parent)
		{
			// если выбрали виджет на табе, то поднять лист таба
			if (parent->getTypeName() == "Sheet")
			{
				MyGUI::TabPtr tab = parent->getParent()->castType<MyGUI::Tab>();
				tab->selectSheet(parent->castType<MyGUI::Sheet>());
			}
			// если выбрали лист таба, то поднять лист таба
			if (current_widget->getTypeName() == "Sheet")
			{
				MyGUI::TabPtr tab = parent->castType<MyGUI::Tab>();
				tab->selectSheet(current_widget->castType<MyGUI::Sheet>());
			}
			coord = current_widget->getAbsoluteCoord();
		}
		current_widget_rectangle->show();
		current_widget_rectangle->setPosition(coord);
		MyGUI::InputManager::getInstance().setKeyFocusWidget(current_widget_rectangle);
	}

	// delete all previous properties
	for (std::vector<MyGUI::StaticTextPtr>::iterator iter = propertiesText.begin(); iter != propertiesText.end(); ++iter)
		(*iter)->hide();
	for (MyGUI::VectorWidgetPtr::iterator iter = propertiesElement.begin(); iter != propertiesElement.end(); ++iter)
		(*iter)->hide();

	if (null == _current_widget)
	{
		mainWidget()->hide();
	}
	else
	{
		mainWidget()->show();

		pairs_counter = 0;
		mPanelMainProperties.update(_current_widget);
		mPanelTypeProperties.update(_current_widget, PanelProperties::TYPE_PROPERTIES);
		mPanelGeneralProperties.update(_current_widget, PanelProperties::WIDGET_PROPERTIES);
		mPanelEvents.update(_current_widget, PanelProperties::EVENTS);
		mPanelItems.update(_current_widget);
		mPanelUserData.update(_current_widget);
	}
}

void PropertiesPanelView::createPropertiesWidgetsPair(MyGUI::WidgetPtr _window, std::string _property, std::string _value, std::string _type,int y)
{
	pairs_counter++;
	int x1 = 0, x2 = 125;
	int w1 = 120;
	int w2 = _window->getWidth() - x2;
	const int h = PropertyItemHeight;

	if (_property == "Position")
	{
		x1 = 66;
		w1 = w1 - x1;
	}

	MyGUI::StaticTextPtr text;
	MyGUI::WidgetPtr editOrCombo;
	//int string_int_float; // 0 - string, 1 - int, 2 - float

	int widget_for_type;// 0 - Edit, 1 - Combo mode drop, 2 - ...
	std::string type_names[2] = {"Edit", "ComboBox"};
	if ("Name" == _type) widget_for_type = 0;
	else if ("Skin" == _type) widget_for_type = 1;
	else if ("Position" == _type) widget_for_type = 0;
	else if ("Layer" == _type) widget_for_type = 1;
	else if ("String" == _type) widget_for_type = 0;
	else if ("Align" == _type) widget_for_type = 1;
	// не совсем правильно FIXME
	else if ("1 int" == _type) widget_for_type = 0;
	else if ("2 int" == _type) widget_for_type = 0;
	else if ("4 int" == _type) widget_for_type = 0;
	else if ("1 float" == _type) widget_for_type = 0;
	else if ("2 float" == _type) widget_for_type = 0;
	// надо сделать проще FIXME
	else if ("Colour" == _type) widget_for_type = 0;//"Colour" хорошо бы колорпикером
	else if ("MessageButton" == _type) widget_for_type = 1;//"MessageButton" - тож хз
	// неправильно FIXME
	else if ("WidgetState" == _type) widget_for_type = 1;//по идее комба, но тогда надо еще и все состояния доступные в xml вписать
	else widget_for_type = 1;

	if ((propertiesText.size() < pairs_counter) || (propertiesText[pairs_counter-1]->getParent() != _window))
	{
		text = _window->createWidget<MyGUI::StaticText>("Editor_StaticText", x1, y, w1, h, MyGUI::Align::Default);
		text->setTextAlign(MyGUI::Align::Right);
		if (propertiesText.size() < pairs_counter)
		{
			propertiesText.push_back(text);
		}
		else
		{
			MyGUI::Gui::getInstance().destroyWidget(propertiesText[pairs_counter-1]);
			propertiesText[pairs_counter-1] = text;
		}
	}
	else
	{
		text = propertiesText[pairs_counter-1];
		text->show();
		text->setPosition(x1, y, w1, h);
	}
	std::string prop = _property;
	// trim widget name
	std::string::iterator iter = std::find(prop.begin(), prop.end(), '_');
	if (iter != prop.end()) prop.erase(prop.begin(), ++iter);
	text->setCaption(prop);

	if ((propertiesElement.size() < pairs_counter) || (propertiesElement[pairs_counter-1]->getParent() != _window) ||
		(type_names[widget_for_type] != propertiesElement[pairs_counter-1]->getTypeName()))
	{
		if (widget_for_type == 0)
		{
			editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
			if (_property != "RenderBox_Mesh" && _property != "Image_Texture") editOrCombo->castType<MyGUI::Edit>()->eventEditTextChange = newDelegate (this, &PropertiesPanelView::notifyApplyProperties);
			editOrCombo->castType<MyGUI::Edit>()->eventEditSelectAccept = newDelegate (this, &PropertiesPanelView::notifyApplyProperties);
		}
		else if (widget_for_type == 1)
		{
			editOrCombo = _window->createWidget<MyGUI::ComboBox>("ComboBox", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
			editOrCombo->castType<MyGUI::ComboBox>()->eventComboAccept = newDelegate (this, &PropertiesPanelView::notifyApplyPropertiesCombo);

			editOrCombo->castType<MyGUI::ComboBox>()->setComboModeDrop(true);
		}

		if (propertiesElement.size() < pairs_counter)
		{
			propertiesElement.push_back(editOrCombo);
		}
		else
		{
			MyGUI::Gui::getInstance().destroyWidget(propertiesElement[pairs_counter-1]);
			propertiesElement[pairs_counter-1] = editOrCombo;
		}
	}
	else
	{
		editOrCombo = propertiesElement[pairs_counter-1];
		if (widget_for_type == 1) editOrCombo->castType<MyGUI::ComboBox>()->deleteAllItems();
		editOrCombo->show();
		editOrCombo->setPosition(x2, y, w2, h);
	}

	// fill possoble values
	if (widget_for_type == 1)
	{
		std::vector<std::string> values;
		if (_type == "Skin") values = WidgetTypes::getInstance().find(current_widget->getTypeName())->skin;
		else values = WidgetTypes::getInstance().findPossibleValues(_type);

		for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
			editOrCombo->castType<MyGUI::ComboBox>()->addItem(*iter);
	}

	editOrCombo->setUserString("action", _property);
	editOrCombo->setUserString("type", _type);

	if (_value.empty()) editOrCombo->setCaption(DEFAULT_VALUE);
	else editOrCombo->castType<MyGUI::Edit>()->setOnlyText(_value);
}

void PropertiesPanelView::notifyApplyProperties(MyGUI::WidgetPtr _sender)
{
	EditorWidgets * ew = &EditorWidgets::getInstance();
	ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES));
	WidgetContainer * widgetContainer = ew->find(current_widget);
	std::string action = _sender->getUserString("action");
	std::string value = _sender->castType<MyGUI::Edit>()->getOnlyText();
	std::string type = _sender->getUserString("type");

	if (value == "[DEFAULT]") value = "";

	if (action == "Name")
	{
		if ((!value.empty()) && (null != ew->find(value)) && (widgetContainer != ew->find(value)))
		{
			MyGUI::Message::_createMessage(localise("Warning"), "Widget with name '" + value + "' already exist.", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
			_sender->castType<MyGUI::Edit>()->setCaption(widgetContainer->name);
			return;
		}

		widgetContainer->name = value;
		ew->widgets_changed = true;
		return;
	}
	else if (action == "Skin")
	{
		widgetContainer->skin = value;
		if ( MyGUI::SkinManager::getInstance().isExist(widgetContainer->skin) )
		{
			MyGUI::xml::xmlDocument * save = ew->savexmlDocument();
			ew->clear();
			ew->loadxmlDocument(save);
			delete save;
			eventRecreate(true);
		}
		else
		{
			std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
			MyGUI::Message::_createMessage("Error", mess , "", "Overlapped", true, null, MyGUI::Message::IconError | MyGUI::Message::Ok);
		}
		return;
	}
	else if (action == "Position")
	{
		if (widgetContainer->relative_mode){
			std::istringstream str(value);
			MyGUI::FloatCoord float_coord;
			str >> float_coord;
			float_coord.left = float_coord.left/100;
			float_coord.top = float_coord.top/100;
			float_coord.width = float_coord.width/100;
			float_coord.height = float_coord.height/100;
			MyGUI::IntCoord coord = MyGUI::WidgetManager::getInstance().convertRelativeToInt(float_coord, current_widget->getParent());
			current_widget->setPosition(coord);
			current_widget_rectangle->setPosition(current_widget->getAbsoluteCoord());
			return;
		}
		MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, "Widget_Move", value);
		current_widget_rectangle->setPosition(current_widget->getAbsoluteCoord());
		return;
	}
	else if (action == "Align")
	{
		widgetContainer->align = value;
		widgetContainer->widget->setAlign(MyGUI::Align::parse(value));
		return;
	}
	else if (action == "Layer")
	{
		widgetContainer->layer = value;
		return;
	}

	bool success = false;
	if ((type == "1 int") || (type == "2 int") || (type == "4 int") || (type == "1 float") || (type == "2 float"))
	{
		if ((value != "") && (value.find_first_of("0123456789") != std::string::npos))
			success = ew->tryToApplyProperty(widgetContainer->widget, action, value);
	}
	else if (value != "" || "Widget_FontName" != action)
		success = ew->tryToApplyProperty(widgetContainer->widget, action, value);

	if (success)
	{
		current_widget_rectangle->setPosition(current_widget->getAbsoluteCoord());
	}
	else
	{
		_sender->setCaption(DEFAULT_VALUE);
		return;
	}

	// если такое св-во было, то заменим (или удалим если стерли) значение
	for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
	{
		if (iterProperty->first == action){
			if (value.empty()) widgetContainer->mProperty.erase(iterProperty);
			else iterProperty->second = value;
			return;
		}
	}

	// если такого свойства не было раньше, то сохраняем
	widgetContainer->mProperty.push_back(std::make_pair(action, value));
}

void PropertiesPanelView::notifyApplyPropertiesCombo(MyGUI::WidgetPtr _sender)
{
	notifyApplyProperties(_sender);
}
