#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#include "MyGUI.h"
#include <string>
#include <stdlib.h>

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define TO_GRID(x) ((x)/grid_step*grid_step)

const std::string LogSection = "LayoutEditor";

EditorWidgets * ew;
WidgetTypes * wt;
MyGUI::Gui * mGUI;
UndoManager * um;

//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	MyGUI::LogManager::registerSection(LogSection, "LayoutEditor.log");
	wt = new WidgetTypes();
	wt->initialise();
	ew = new EditorWidgets();
	ew->initialise();
	um = new UndoManager(ew);

	mGUI = new MyGUI::Gui();
	mGUI->initialise(BasisManager::getInstance().mWindow, "editor.xml");

	MyGUI::LayoutManager::getInstance().load("interface.layout");

	// menu panel (should be dropdown menu)
	ASSIGN_FUNCTION("LayoutEditor_buttonLoad", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("LayoutEditor_buttonSave", &EditorState::notifySave);
	ASSIGN_FUNCTION("LayoutEditor_buttonSaveAs", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("LayoutEditor_buttonSettings", &EditorState::notifySettings);
	ASSIGN_FUNCTION("LayoutEditor_buttonClear", &EditorState::notifyClear);
	ASSIGN_FUNCTION("LayoutEditor_buttonQuit", &EditorState::notifyQuit);

	// widgets panel
	int i = 0;
	int w, h = 24;
	MyGUI::WindowPtr windowWidgets = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowWidgets");
	w = windowWidgets->getClientRect().width/2;
	for (std::vector<WidgetType*>::iterator iter = wt->widget_types.begin(); iter != wt->widget_types.end(); ++iter)
	{
		for (std::vector<std::string>::iterator iterSkin = (*iter)->skin.begin(); iterSkin != (*iter)->skin.end(); ++iterSkin)
		{
			MyGUI::ButtonPtr button = windowWidgets->createWidget<MyGUI::Button>("ButtonSmall", i%2*w, i/2*h, w, h, MyGUI::ALIGN_TOP|MyGUI::ALIGN_HSTRETCH, MyGUI::utility::toString((*iter)->name, *iterSkin));
			button->setCaption(/*(*iter)->name + " " +*/ *iterSkin);
			button->setTextAlign(MyGUI::ALIGN_LEFT);
			button->setUserString("widget", (*iter)->name);
			button->setUserString("skin", *iterSkin);
			button->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetType);
			button->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetTypeDoubleclick);
			i++;
		}
	}
	i++;
	allWidgetsCombo = windowWidgets->createWidget<MyGUI::ComboBox>("EditorComboBox", 0, (i/2)*h, w*2, h, MyGUI::ALIGN_TOP|MyGUI::ALIGN_HSTRETCH);
	allWidgetsCombo->setComboModeDrop(true);
	allWidgetsCombo->eventKeySetFocus = MyGUI::newDelegate(this, &EditorState::notifyWidgetsTabPressed);
	allWidgetsCombo->eventComboChangePosition = MyGUI::newDelegate(this, &EditorState::notifyWidgetsTabSelect);

	int height = windowWidgets->getHeight() - windowWidgets->getClientRect().height;
	windowWidgets->setSize(windowWidgets->getSize().width, height + (i/2+1)*h);

	loadSettings();
	notifyClear();
}
//===================================================================================
void EditorState::exit()
{
	saveSettings();
	mGUI->shutdown();
	delete mGUI;
	delete um;
	ew->shutdown();
	delete ew;
	wt->shutdown();
	delete wt;
}
//===================================================================================
bool EditorState::mouseMoved( const OIS::MouseEvent &arg )
{
	x2 = TO_GRID(arg.state.X.abs);
	y2 = TO_GRID(arg.state.Y.abs);

	if ((creating_status == 1) && ((x1-x2)*(y1-y2) != 0))
	{
		MyGUI::IntCoord coord(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));

		creating_status = 2;

		// внимание current_widget родитель и потом сразу же сын
		// пока не найдем ближайшего над нами способного быть родителем
		std::string tmpname = MyGUI::utility::toString("LayoutEditor_", current_widget_type, global_counter);
		global_counter++;
		while (current_widget && false == wt->find(current_widget->getWidgetType())->parent) current_widget = current_widget->getParent();
		if (current_widget && wt->find(current_widget_type)->child)
		{
			coord = coord - current_widget->getPosition();
			current_widget = current_widget->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::ALIGN_DEFAULT, tmpname);
		}
		else current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::ALIGN_DEFAULT, "Back", tmpname);

		current_widget->setCaption(current_widget_skin);
	}
	else if (creating_status == 2)
	{
		MyGUI::IntCoord coord(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));
		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
	}

	/*MyGUI::LayerItemInfoPtr rootItem = null;
	MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	while ((null == ew->find(item)) && (null != item)) item = item->getParent();
	if (null != item)
	{
		updatePropertiesPanel(item);
	}*/

	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool EditorState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))
		MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	else
	{
		x1 = TO_GRID(arg.state.X.abs);
		y1 = TO_GRID(arg.state.Y.abs);

		if (id == OIS::MB_Left && !creating_status && current_widget_type != "") creating_status = 1;

		// чтобы прямоугольник не мешался
		if (current_widget_rectangle)
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(current_widget_rectangle);
			current_widget_rectangle = null;
		}

		MyGUI::LayerItemInfoPtr rootItem = null;
		MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
		if (null != item)
		{
			while ((null == ew->find(item)) && (null != item)) item = item->getParent();

			if (null != item) // нашли
			{
				MyGUI::IntSize size = item->getTextSize();
				notifySelectWidget(item);
				if (creating_status != 1){
					MyGUI::InputManager::getInstance().injectMouseMove(arg);
				}// это чтобы сразу можно было тащить
			}
			MyGUI::InputManager::getInstance().injectMousePress(arg, id);
		}else{
			MyGUI::InputManager::getInstance().injectMousePress(arg, id);
			notifySelectWidget(item);
		}
	}

	return true;
}
//===================================================================================
bool EditorState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	x2 = TO_GRID(arg.state.X.abs);
	y2 = TO_GRID(arg.state.Y.abs);

	if (creating_status > 0)
	{
		if ((x1-x2)*(y1-y2) != 0)
		{
			// создали виджет, все счастливы
			WidgetContainer * widgetContainer = new WidgetContainer(current_widget_type, current_widget_skin, current_widget);
			widgetContainer->position = current_widget->getCoord().print();
			widgetContainer->layer = current_widget->getLayerName();
			ew->add(widgetContainer);
			notifySelectWidget(current_widget);
			mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
			current_widget_type = "";
			current_widget_skin = "";
			creating_status = 0;
		}
		else
		{
			// не удалось создать, т.к. размер нулевой
			if ((creating_status > 1) && current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
			mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
			current_widget_type = "";
			current_widget_skin = "";
			creating_status = 0;
			global_counter--;
		}
	}

	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool EditorState::keyPressed( const OIS::KeyEvent &arg )
{
	if ((arg.key == OIS::KC_ESCAPE) && (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))) notifyLoadSaveCancel();
	else if ((arg.key == OIS::KC_RETURN) && (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))) notifyLoadSaveEditAccept();
	//if ( arg.key == OIS::KC_ESCAPE ) notifyQuit();
	else if ( arg.key == OIS::KC_DELETE ) {
		if (MyGUI::InputManager::getInstance().getKeyFocusWidget() == current_widget_rectangle)
			if (current_widget){ ew->remove(current_widget); notifySelectWidget(null); }
	}
	else if ( arg.key == OIS::KC_LSHIFT ) shiftPressed = true;

	if ( arg.key == OIS::KC_SYSRQ ) {BasisManager::getInstance().mWindow->writeContentsToFile("screenshot.png");}

	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool EditorState::keyReleased( const OIS::KeyEvent &arg )
{
	if ( arg.key == OIS::KC_LSHIFT ) shiftPressed = false;
	MyGUI::InputManager::getInstance().injectKeyRelease(arg);
	return true;
}
//===================================================================================
bool EditorState::frameStarted(const Ogre::FrameEvent& evt)
{
	/*static float time = 0;
	time += evt.timeSinceLastFrame;
	if (time > 1) {
		time -= 1;
		try {
			const Ogre::RenderTarget::FrameStats& stats = BasisManager::getInstance().mWindow->getStatistics();
			MyGUI::MYGUI_OUT(MyGUI::utility::toString("FPS : ", stats.lastFPS, "\ntriangle : ", stats.triangleCount, "\nbatch : ", stats.batchCount));
		} catch (...) { }
	}*/

	mGUI->injectFrameEntered(evt.timeSinceLastFrame);
	return true;
}
//===================================================================================
void EditorState::loadSettings()
{
	std::string _fileName = "settings.xml";
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "' not found");
		return;
	}
	if (false == doc.open(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}

	MyGUI::xml::xmlNodePtr root = doc.getRoot();
	if ( (null == root) || (root->getName() != "MyGUI") ) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Settings")
		{
			// берем детей и крутимся
			MyGUI::xml::xmlNodeIterator field = root->getNodeIterator();
			while (field.nextNode()) {

				std::string key, value;

				if (field->getName() == "Property")
				{
					if (false == field->findAttribute("key", key)) continue;
					if (false == field->findAttribute("value", value)) continue;

					if (key == "Grid") grid_step = MyGUI::utility::parseInt(value);
				}
			}
		}
	}

	if (grid_step <= 0) grid_step = 1;
}

void EditorState::saveSettings()
{
	std::string _fileName = "settings.xml";
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
	}

	doc.createInfo();
	MyGUI::xml::xmlNodePtr root = doc.createRoot("MyGUI");
	root->addAttributes("type", "Settings");

	MyGUI::xml::xmlNodePtr nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "Grid");
	nodeProp->addAttributes("value", grid_step);

	if (false == doc.save(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}
}

void EditorState::notifySave(MyGUI::WidgetPtr _sender)
{
	if (fileName != "") ew->save(fileName);
	else notifyLoadSaveAs(_sender);
}

void EditorState::notifyLoadSaveAs(MyGUI::WidgetPtr _sender)
{
	// create message box with file name and two buttons
	MyGUI::WidgetPtr messageWindow = MyGUI::LayoutManager::getInstance().load("SaveLoadMessage.layout")[0];
	MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
	MyGUI::IntSize size(messageWindow->getSize());
	messageWindow->setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);
	MyGUI::InputManager::getInstance().addWidgetModal(messageWindow);
	
	MyGUI::VectorWidgetPtr childs = messageWindow->getChilds();

	// set fileName in edit
	MyGUI::ComboBoxPtr combo = MyGUI::castWidget<MyGUI::ComboBox>(childs[0]);
	if (fileName != "") combo->setCaption(fileName);
	combo->eventEditSelectAccept = newDelegate(this, &EditorState::notifyLoadSaveEditAccept);
	std::vector<Ogre::String> strs = MyGUI::helper::getVectorResourcePath("*.layout");
	for (std::vector<Ogre::String>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		combo->addItem(*iter);
	}
	if (_sender->getCaption() == "SaveAs...") childs[1]->setCaption("Save");
	else childs[1]->setCaption(_sender->getCaption());
	childs[1]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveAccept);
	childs[2]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveCancel);
}

void EditorState::notifySettings(MyGUI::WidgetPtr _sender)
{
	MyGUI::LayoutManager::getInstance().load("Settings.layout");
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("LayoutEditor_gridEdit");
	gridEdit->setCaption(MyGUI::utility::toString(grid_step));
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
	mGUI->findWidget<MyGUI::Button>("LayoutEditor_buttonOkSettings")
		->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifyOkSettings);
}

void EditorState::notifyClear(MyGUI::WidgetPtr _sender)
{
	current_widget_type = "";
	current_widget_skin = "";
	creating_status = 0;
	shiftPressed = false;
	fileName = "";
	ew->clear();
	notifySelectWidget(null);
}

void EditorState::notifyQuit(MyGUI::WidgetPtr _sender)
{
  BasisManager::getInstance().m_exit = true;
}

void EditorState::notifyLoadSaveAccept(MyGUI::WidgetPtr _sender)
{
	bool success;
	if (_sender->getCaption() == "Load") success = ew->load(mGUI->findWidget<MyGUI::Edit>("LayoutEditor_editFileName")->getCaption());
	else/*(_sender->getCaption() == "Save")*/ success = ew->save(mGUI->findWidget<MyGUI::Edit>("LayoutEditor_editFileName")->getCaption());

	if (false == success) 
	{
		MyGUI::Message::createMessage("Warning", "Failed to " + _sender->getCaption() + " file", true, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
	}
	else
	{
		// запоминает последнее удачное имя файла
		fileName = mGUI->findWidget<MyGUI::Edit>("LayoutEditor_editFileName")->getCaption();
		notifyLoadSaveCancel(_sender);
	}
}

void EditorState::notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget)
{
	notifyLoadSaveAccept(mGUI->findWidgetT("LayoutEditor_buttonSaveLoad"));
}

void EditorState::notifyLoadSaveCancel(MyGUI::WidgetPtr _sender)
{
	MyGUI::InputManager::getInstance().removeWidgetModal(mGUI->findWidgetT("LayoutEditor_windowSaveLoad"));
	MyGUI::WidgetManager::getInstance().destroyWidget(mGUI->findWidgetT("LayoutEditor_windowSaveLoad"));
}

void EditorState::notifySelectWidgetType(MyGUI::WidgetPtr _sender)
{
	current_widget_type = _sender->getUserString("widget");
	current_widget_skin = _sender->getUserString("skin");
	MyGUI::castWidget<MyGUI::Button>(_sender)->setButtonPressed(true);
}

void EditorState::notifySelectWidgetTypeDoubleclick(MyGUI::WidgetPtr _sender)
{
	current_widget_type = _sender->getUserString("widget");
	current_widget_skin = _sender->getUserString("skin");

	current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, MyGUI::IntCoord(), MyGUI::ALIGN_DEFAULT, "Back");

	MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
	MyGUI::IntSize size(current_widget->getSize());
	current_widget->setPosition((view.width-size.width)/2, (view.height-size.height)/2, 100/*size.width*/, /*size.height*/100); // FIXME

	current_widget->setCaption(current_widget_skin);
	WidgetContainer * widgetContainer = new WidgetContainer(current_widget_type, current_widget_skin, current_widget);
	widgetContainer->position = current_widget->getCoord().print();
	widgetContainer->layer = current_widget->getLayerName();
	ew->add(widgetContainer);
	notifySelectWidget(current_widget);
	mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
	current_widget_type = "";
	current_widget_skin = "";
}

void EditorState::notifyWidgetsTabPressed(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
{
	allWidgetsCombo->deleteAllItems();
	for (std::vector<WidgetContainer*>::iterator iter = ew->widgets.begin(); iter != ew->widgets.end(); ++iter )
	{
		std::string item;
		if ((*iter)->name.empty())
		{				
			// trim "LayoutEditor_"
			std::string tmp = (*iter)->widget->getName();
			if (0 == strncmp("LayoutEditor_", tmp.c_str(), 13))
			{
					std::string::iterator iter = std::find(tmp.begin(), tmp.end(), '_');
					if (iter != tmp.end()) tmp.erase(tmp.begin(), ++iter);
			}
			item = MyGUI::utility::toString("[", tmp, "]");
		}
		else item = (*iter)->name + " ";
		allWidgetsCombo->addItem(item + (*iter)->widget->getWidgetType());
	}
}

void EditorState::notifyWidgetsTabSelect(MyGUI::WidgetPtr _widget, size_t _index)
{
	notifySelectWidget(ew->widgets[_index]->widget);
}

void EditorState::notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0) grid_step = 1;
	_sender->setCaption(Ogre::StringConverter::toString(grid_step));
}

void EditorState::notifyNewGridStepAccept(MyGUI::WidgetPtr _sender)
{
	notifyNewGridStep(_sender);
}

void EditorState::notifyOkSettings(MyGUI::WidgetPtr _sender)
{
	mGUI->destroyWidget(mGUI->findWidgetT("LayoutEditor_windowSettings"));
}

void EditorState::notifySelectWidget(MyGUI::WidgetPtr _sender)
{
	current_widget = _sender;
	if (current_widget_rectangle)
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(current_widget_rectangle);
		current_widget_rectangle = null;
	}

	if (null != current_widget)
	{
		MyGUI::IntCoord coord = _sender->getCoord();

		MyGUI::WidgetPtr parent = _sender->getParent();
		if (null != parent) coord = convertParentCoordToCoord(coord, _sender);
		current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", coord, MyGUI::ALIGN_DEFAULT, "LayoutEditor_Rectangle");
		current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
		current_widget_rectangle->eventMouseButtonDoubleClick = newDelegate(this, &EditorState::notifyRectangleDoubleClick);
		current_widget_rectangle->eventKeyButtonPressed = newDelegate(this, &EditorState::notifyRectangleKeyPressed);
	}

	updatePropertiesPanel(current_widget);
}

void EditorState::updatePropertiesPanel(MyGUI::WidgetPtr _widget)
{
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowProperties");
	window->hide();

	{
		for (MyGUI::VectorWidgetPtr::iterator iter = propertiesText.begin(); iter != propertiesText.end(); ++iter) mGUI->destroyWidget(*iter);
		propertiesText.clear();
		for (MyGUI::VectorWidgetPtr::iterator iter = propertiesElement.begin(); iter != propertiesElement.end(); ++iter) mGUI->destroyWidget(*iter);
		propertiesElement.clear();
	}

	if (null != _widget){
		window->show();
		int x1 = 0, x2 = 120;
		int w1 = 120;
		int w2 = window->getClientRect().width - w1;
		int y = 0;
		const int h = 20;
		WidgetType * widgetType = wt->find(current_widget->getWidgetType());
		WidgetContainer * widgetContainer = ew->find(current_widget);

		if (widgetContainer->name.empty()){
			// trim "LayoutEditor_"
			std::string tmp = current_widget->getName();
			if (0 == strncmp("LayoutEditor_", tmp.c_str(), 13))
			{
					std::string::iterator iter = std::find(tmp.begin(), tmp.end(), '_');
					if (iter != tmp.end()) tmp.erase(tmp.begin(), ++iter);
			}
			allWidgetsCombo->setCaption(MyGUI::utility::toString("[", tmp, "]", current_widget->getWidgetType()));
		}
		else allWidgetsCombo->setCaption(widgetContainer->name + " " + current_widget->getWidgetType());

		createPropertiesWidgetsPair(window, "Name", widgetContainer->name, "Name", x1, x2, w1, w2, y, h);
		y += h;
		if (widgetType->resizeable)
		{
			createPropertiesWidgetsPair(window, "Position", _widget->getCoord().print(), "Position", x1, x2, w1, w2, y, h);
			y += h;
		}

		createPropertiesWidgetsPair(window, "Align", widgetContainer->align, "Align", x1, x2, w1, w2, y, h);
		y += h;

		if (null == _widget->getParent())
		{
			createPropertiesWidgetsPair(window, "Layer", _widget->getLayerName(), "Layer", x1, x2, w1, w2, y, h);
			y += h;
		}

		// separator
		propertiesElement.push_back(window->createWidget<MyGUI::Widget>("Separator1", x1, y, w1 + w2, 2, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH));
		MyGUI::StaticTextPtr text;
		text = window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w1 + w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		text->setCaption(current_widget->getWidgetType() + " specific properties");
		text->setTextAlign(MyGUI::ALIGN_CENTER);
		propertiesText.push_back(text);
		y += h;

		//all other
		for (MapString::iterator iter = widgetType->parameter.begin(); iter != widgetType->parameter.end(); ++iter)
		{
			std::string value = "";
			for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
				if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
			createPropertiesWidgetsPair(window, iter->first, value, iter->second, x1, x2, w1, w2, y, h);
			y += h;
		}

		// separator
		propertiesElement.push_back(window->createWidget<MyGUI::Widget>("Separator1", x1, y, w1 + w2, 2, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH));
		text = window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w1 + w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		text->setCaption("Other properties");
		text->setTextAlign(MyGUI::ALIGN_CENTER);
		propertiesText.push_back(text);
		y += h;

		if (widgetType->resizeable)
		{
			//base properties (from Widget)
			WidgetType * baseType = wt->find("Widget");
			for (MapString::iterator iter = baseType->parameter.begin(); iter != baseType->parameter.end(); ++iter)
			{
				std::string value = "";
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
				createPropertiesWidgetsPair(window, iter->first, value, iter->second, x1, x2, w1, w2, y, h);
				y += h;
			}
		}
		int height = window->getHeight() - window->getClientRect().height;
		window->setSize(window->getSize().width, height + y);
	}
}

void EditorState::createPropertiesWidgetsPair(MyGUI::WindowPtr _window, std::string _property, std::string _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h)
{
	MyGUI::StaticTextPtr text;
	MyGUI::WidgetPtr editOrCombo;

	int widget_for_type;// 0 - Edit, 1 - Combo, 2 - ...
	if ("Name" == _type) widget_for_type = 0;
	else if ("Position" == _type) widget_for_type = 0;
	else if ("Layer" == _type) widget_for_type = 1;
	else if ("String" == _type) widget_for_type = 0;
	// не совсем правильно FIXME
	else if ("1 int" == _type) widget_for_type = 0;
	else if ("2 int" == _type) widget_for_type = 0;
	else if ("4 int" == _type) widget_for_type = 0;
	else if ("1 float" == _type) widget_for_type = 0;
	else if ("2 float" == _type) widget_for_type = 0;
	// надо сделать проще FIXME
	else if ("Colour" == _type) widget_for_type = 0;//"Colour" хорошо бы колорпикером
	else if ("Align" == _type) widget_for_type = 1;//"Align" - одной комбой не катит
	else if ("MessageButton" == _type) widget_for_type = 0;//"MessageButton" - тож хз
	// неправильно FIXME
	else if ("WidgetState" == _type) widget_for_type = 1;//по идее комба, но тогда надо еще и все состояния доступные в xml вписать
	else  widget_for_type = 1;

	text = _window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w1, h, MyGUI::ALIGN_DEFAULT);
	std::string prop = _property;
	// trim widget name
	std::string::iterator iter = std::find(prop.begin(), prop.end(), '_');
	if (iter != prop.end()) prop.erase(prop.begin(), ++iter);
	text->setCaption(prop);
	text->setFontHeight(h-2);

	if (widget_for_type == 0)
	{
		editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditTextChange = newDelegate (this, &EditorState::notifyApplyProperties);
		MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditSelectAccept = newDelegate (this, &EditorState::notifyApplyProperties);
	}
	else if (widget_for_type == 1)
	{
		editOrCombo = _window->createWidget<MyGUI::ComboBox>("ComboBox", x2, y, w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		//MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->eventComboChangePosition = newDelegate (this, &EditorState::notifyApplyPropertiesCombo);
		MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->eventComboAccept = newDelegate (this, &EditorState::notifyApplyPropertiesCombo);

		std::vector<std::string> values = wt->findPossibleValues(_type);
		for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
			MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->addItem(*iter);

		MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->setComboModeDrop(true);
	}

	editOrCombo->setUserString("action", _property);
	text->setFontHeight(h-4);

	// trim "ALIGN_"
	if (0 == strncmp("ALIGN_", _value.c_str(), 6))
	{
		std::string tmp = "";
		const std::vector<std::string> & vec = MyGUI::utility::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {

			prop = vec[pos];
			iter = std::find(prop.begin(), prop.end(), '_');
			if (iter != prop.end()) prop.erase(prop.begin(), ++iter);
			if (!tmp.empty()) tmp += " ";
			tmp += prop;
		}
		_value = tmp;
	}

	if (_value.empty()) editOrCombo->setCaption("DEFAULT");
	else MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->setOnlyText(_value);
	propertiesText.push_back(text);
	propertiesElement.push_back(editOrCombo);
}

void EditorState::notifyApplyProperties(MyGUI::WidgetPtr _sender)
{
	WidgetContainer * widgetContainer = ew->find(current_widget);
	std::string action = _sender->getUserString("action");
	std::string value = MyGUI::castWidget<MyGUI::Edit>(_sender)->getOnlyText();
	if (value == "DEFAULT") value = "";

	if (std::string::npos != action.find("Align"))
	{
		std::string tmp = "";
		const std::vector<std::string> & vec = MyGUI::utility::split(value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			if (!tmp.empty()) tmp += " ";
			tmp += "ALIGN_" + vec[pos];
		}
		value = tmp;
	}

	if (action == "Name")
	{
		widgetContainer->name = value;
		return;
	}
	else if (action == "Position")
	{
		widgetContainer->position = value;
		MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);
		return;
	}
	else if (action == "Align")
	{
		widgetContainer->align = value;
		widgetContainer->widget->setAlign(MyGUI::SkinManager::getInstance().parseAlign(value));
		// FIXME применяется только после сохранения и повторной загрузки
		return;
	}
	else if (action == "Layer")
	{
		widgetContainer->layer = value;
		MyGUI::LayerManager::getInstance().detachItem(current_widget);
		MyGUI::LayerManager::getInstance().attachItem(current_widget, widgetContainer->layer, true);
		return;
	}

	if ("Message_Modal" != action) MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);

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

void EditorState::notifyApplyPropertiesCombo(MyGUI::WidgetPtr _sender/*, size_t _index*/)
{
	notifyApplyProperties(_sender);
}

void EditorState::notifyRectangleResize(MyGUI::WidgetPtr _sender)
{
	// найдем соответствующий контейнер виджета и переместим/растянем
	WidgetContainer * widgetContainer = ew->find(current_widget);
	// align to grid
	MyGUI::IntCoord coord = _sender->getCoord();
	if (wt->find(current_widget->getWidgetType())->resizeable)
	{
		if (!shiftPressed) coord = snapToGrid(coord);
		current_widget_rectangle->setPosition(coord);

		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
		widgetContainer->position = coord.print();
		//updatePropertiesPanel(current_widget);
		propertiesElement[1]->setCaption(coord.print());
		current_widget_rectangle->setPosition(convertParentCoordToCoord(current_widget->getCoord(), current_widget));
	}
	else
	{
		current_widget_rectangle->setPosition(convertParentCoordToCoord(current_widget->getCoord(), current_widget));
	}
}

MyGUI::IntCoord EditorState::convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	if (null != parent){
		coord = coord - parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		coord = convertCoordToParentCoord(coord, parent);
	}
	return coord;
}

MyGUI::IntCoord EditorState::convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	if (null != parent){
		coord = coord + parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		coord = convertParentCoordToCoord(coord, parent);
	}
	return coord;
}

MyGUI::IntCoord EditorState::snapToGrid(MyGUI::IntCoord coord)
{
	coord.left = TO_GRID(coord.left + grid_step-1);
	coord.top = TO_GRID(coord.top + grid_step-1);
	coord.width = TO_GRID(coord.width);
	coord.height = TO_GRID(coord.height);
	return coord;
}

void EditorState::notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender)
{
	if (current_widget->getWidgetType() == "Tab")
	{
		MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(current_widget);
		std::string name = MyGUI::utility::toString("Sheet", tab->getSheetCount()+1);
		MyGUI::SheetPtr sheet = tab->addSheet(name);
		sheet->setCaption(name);
		ew->add(new WidgetContainer("Sheet", "Sheet", sheet, name));
	}
}

void EditorState::notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char)
{
	if (OIS::KC_TAB == _key)
	{
		if ((null != current_widget->getParent()) && (current_widget->getParent()->getWidgetType() == "Tab")) notifySelectWidget(current_widget->getParent());
		if (current_widget->getWidgetType() == "Tab")
		{
			MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(current_widget);
			size_t sheet = tab->getSelectSheetIndex();
			sheet++;
			if (sheet >= tab->getSheetCount()) sheet = 0;
			tab->selectSheetIndex(sheet);
		}
	}
}
