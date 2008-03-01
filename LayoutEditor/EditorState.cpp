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

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

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
	ASSIGN_FUNCTION("LayoutEditor_buttonTest", &EditorState::notifyTest);
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
			button->setCaption(*iterSkin);
			button->setTextAlign(MyGUI::ALIGN_CENTER);
			button->setUserString("widget", (*iter)->name);
			button->setUserString("skin", *iterSkin);
			button->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetType);
			button->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetTypeDoubleclick);
			i++;
		}
	}
	i++;
	allWidgetsCombo = mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_allWidgetsCombo");//windowWidgets->createWidget<MyGUI::ComboBox>("EditorComboBox", 0, (i/2)*h, w*2, h, MyGUI::ALIGN_TOP|MyGUI::ALIGN_HSTRETCH);
	allWidgetsCombo->setComboModeDrop(true);
	allWidgetsCombo->eventKeySetFocus = MyGUI::newDelegate(this, &EditorState::notifyWidgetsTabPressed);
	allWidgetsCombo->eventComboChangePosition = MyGUI::newDelegate(this, &EditorState::notifyWidgetsTabSelect);

	int height = windowWidgets->getHeight() - windowWidgets->getClientRect().height;
	windowWidgets->setSize(windowWidgets->getSize().width, height + (i/2+2)*h);

	// properties panel
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowProperties");
	window->setPosition(mGUI->getViewWidth() - window->getSize().width, 0);

	// settings panel
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("LayoutEditor_gridEdit");
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
	ASSIGN_FUNCTION("LayoutEditor_buttonOkSettings", &EditorState::notifyOkSettings);

	MyGUI::ComboBoxPtr combo= mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_comboboxResolution");
	Ogre::ConfigOptionMap map = BasisManager::getInstance().mRoot->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator iter = map.find("Video Mode");
	int selectedIdx = 0;
	int wid, hei;
	for (unsigned int j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		std::string tmp;
		std::istringstream str(videoMode);
		str >> wid >> tmp >> hei;
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		combo->addItem(MyGUI::utility::toString(wid, " x ", hei));
	}
	combo->setItemSelect(selectedIdx);
	MyGUI::ComboBoxPtr comboFullScreen = mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_comboboxFullscreen");
	iter = map.find("Full Screen");
	selectedIdx = 0;
	for (unsigned int j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		comboFullScreen->addItem(videoMode);
	}
	comboFullScreen->setItemSelect(selectedIdx);

	// strings panel
	MyGUI::WindowPtr windowStrings = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowStrings");
	windowStrings->setPosition(mGUI->getViewWidth() - windowStrings->getSize().width, mGUI->getViewHeight() - windowStrings->getSize().height);
	ASSIGN_FUNCTION("LayoutEditor_buttonAddString", &EditorState::notifyAddString);
	ASSIGN_FUNCTION("LayoutEditor_buttonDeleteString", &EditorState::notifyDeleteString);
	ASSIGN_FUNCTION("LayoutEditor_buttonSelectString", &EditorState::notifySelectString);
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editString");
	edit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyUpdateString);
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	list->eventListMouseItemActivate = MyGUI::newDelegate(this, &EditorState::notifySelectStringItem);

	// create widget rectangle
	current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", MyGUI::IntCoord(), MyGUI::ALIGN_DEFAULT, "LayoutEditor_Rectangle");
	current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
	current_widget_rectangle->eventMouseButtonDoubleClick = newDelegate(this, &EditorState::notifyRectangleDoubleClick);
	current_widget_rectangle->eventKeyButtonPressed = newDelegate(this, &EditorState::notifyRectangleKeyPressed);

	loadSettings();
	clear();
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

	if (creating_status == 1)
	{
		MyGUI::IntCoord coord(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));

		creating_status = 2;

		// внимание current_widget родитель и потом сразу же сын
		std::string tmpname = MyGUI::utility::toString("LayoutEditor_", current_widget_type, ew->global_counter);
		ew->global_counter++;
		// пока не найдем ближайшего над нами способного быть родителем
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

	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool EditorState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if ((null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad")) || (want_quit))
	{
		// if we have modal widgets we can't select any widget
		MyGUI::InputManager::getInstance().injectMousePress(arg, id);
		return true;
	}

	x1 = TO_GRID(arg.state.X.abs);
	y1 = TO_GRID(arg.state.Y.abs);

	if (id == OIS::MB_Left && !creating_status && current_widget_type != "") creating_status = 1;

	// это чтобы можно было двигать прямоугольник у невидимых виджето (или виджетов за границами)
	//MyGUI::LayerItemInfoPtr rootItem = null;
	//MyGUI::WidgetPtr itemWithRect = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	// не стал это доделывать, т.к. неоднозначность выбора виджета получается, если кто скажет как выбирать - сделаю

	// чтобы прямоугольник не мешался
	current_widget_rectangle->hide();

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
				MyGUI::InputManager::getInstance().injectMouseMove(arg);// это чтобы сразу можно было тащить
			}
		}
		MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	}else{
		MyGUI::InputManager::getInstance().injectMousePress(arg, id);
		notifySelectWidget(null);
	}

	// вернем прямоугольник
	if (current_widget && !creating_status)
		current_widget_rectangle->show();
	else if (creating_status)
		current_widget_rectangle->hide();

	return true;
}
//===================================================================================
bool EditorState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))
	{
	}
	else
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
				current_widget = null;
				notifySelectWidget(widgetContainer->widget);
				mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
				current_widget_type = "";
				current_widget_skin = "";
				creating_status = 0;
				um->addValue();
			}
			else
			{
				// не удалось создать, т.к. размер нулевой
				if ((creating_status > 1) && current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
				mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
				current_widget_type = "";
				current_widget_skin = "";
				if (creating_status == 2) ew->global_counter--;
				creating_status = 0;
			}
		}
	}

	um->dropLastProperty();
	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);

	if (recreate)
	{
		recreate = false;
		notifySelectWidget(null); // виджет пересоздался, теперь никто незнает его адреса :)
	}

	return true;
}
//===================================================================================
bool EditorState::keyPressed( const OIS::KeyEvent &arg )
{
	if (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))
	{
		if (arg.key == OIS::KC_ESCAPE) notifyLoadSaveCancel();
		else if (arg.key == OIS::KC_RETURN) notifyLoadSaveEditAccept();
	}
	else
	{
		if ( arg.key == OIS::KC_ESCAPE )
		{
			if (!want_quit){ notifyQuit(); return true;}
		}
		else if ( arg.key == OIS::KC_DELETE )
		{
			if (MyGUI::InputManager::getInstance().getKeyFocusWidget() == current_widget_rectangle){
				if (current_widget){ ew->remove(current_widget); notifySelectWidget(null); }
				um->addValue();
			}
		}
		if (ctrlPressed)
		{
			if (arg.key == OIS::KC_O || arg.key == OIS::KC_L) notifyLoadSaveAs(mGUI->findWidgetT("LayoutEditor_buttonLoad"));
			else if (arg.key == OIS::KC_S) notifySave(mGUI->findWidgetT("LayoutEditor_buttonSave"));
			else if (arg.key == OIS::KC_Z){
				um->undo();
				notifySelectWidget(null);
			}
			else if ((arg.key == OIS::KC_Y) || ((shiftPressed) && (arg.key == OIS::KC_Z))){
				um->redo();
				notifySelectWidget(null);
			}
			else if (arg.key == OIS::KC_T)
			{
				notifyTest();
				return true;
			}
		}
	}

	if ( arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT ) shiftPressed = true;
	if ( arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL ) ctrlPressed = true;
	if ( arg.key == OIS::KC_SYSRQ ) {BasisManager::getInstance().mWindow->writeContentsToFile("screenshot.png");}

	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool EditorState::keyReleased( const OIS::KeyEvent &arg )
{
	if ( arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT ) shiftPressed = false;
	if ( arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL ) ctrlPressed = false;
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
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowSettings");
	window->show();
	MyGUI::LayerManager::getInstance().upItem(window);
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("LayoutEditor_gridEdit");
	gridEdit->setCaption(MyGUI::utility::toString(grid_step));
}

void EditorState::notifyTest(MyGUI::WidgetPtr _sender)
{
	//BasisManager::getInstance().pushState(&BasisManager::getInstance().mTest);
}

void EditorState::notifyClear(MyGUI::WidgetPtr _sender)
{
	MyGUI::Message::createMessage("Warning", "Are you sure you want to delete all widgets?", true, newDelegate(this, &EditorState::notifyClearMessage), MyGUI::Message::IconWarning | MyGUI::Message::Yes | MyGUI::Message::No);
}

void EditorState::notifyClearMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)
{
	if (_button == MyGUI::Message::Yes || _button == MyGUI::Message::Button1) clear();
}

void EditorState::clear()
{
	current_widget_type = "";
	current_widget_skin = "";
	creating_status = 0;
	recreate = false;
	arrow_move = false;
	shiftPressed = false;
	ctrlPressed = false;
	want_quit = false;
	fileName = "";
	ew->clear();
	notifySelectWidget(null);
	um->addValue();
}

void EditorState::notifyQuit(MyGUI::WidgetPtr _sender)
{
	MyGUI::Message::createMessage("Warning", "Are you sure you want to exit?", true, newDelegate(this, &EditorState::notifyQuitMessage), MyGUI::Message::IconWarning | MyGUI::Message::Yes | MyGUI::Message::No);
	want_quit = true;
}

void EditorState::notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)
{
	if (_button == MyGUI::Message::Yes || _button == MyGUI::Message::Button1) BasisManager::getInstance().m_exit = true;
	want_quit = false;
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
		um->addValue();
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

	std::string tmpname = MyGUI::utility::toString("LayoutEditor_", current_widget_type, ew->global_counter);
	ew->global_counter++;

	if (current_widget && wt->find(current_widget_type)->child)
		current_widget = current_widget->createWidgetT(current_widget_type, current_widget_skin, MyGUI::IntCoord(0, 0, 100, 100), MyGUI::ALIGN_DEFAULT, tmpname);
	else
	{
		MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
		current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, MyGUI::IntCoord(), MyGUI::ALIGN_DEFAULT, "Back", tmpname);
		MyGUI::IntSize size(current_widget->getSize());
		current_widget->setPosition((view.width-size.width)/2, (view.height-size.height)/2, 100/*DEFAULT*/, /*DEFAULT*/100); // FIXME
	}
	current_widget->setCaption(current_widget_skin);

	WidgetContainer * widgetContainer = new WidgetContainer(current_widget_type, current_widget_skin, current_widget);
	widgetContainer->position = current_widget->getCoord().print();
	widgetContainer->layer = current_widget->getLayerName();
	ew->add(widgetContainer);
	current_widget = null;
	notifySelectWidget(widgetContainer->widget);
	mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
	current_widget_type = "";
	current_widget_skin = "";

	um->addValue();
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
			item = MyGUI::utility::toString("[#333333", tmp, "#000000]");
		}
		else item = (*iter)->name;
		allWidgetsCombo->addItem(item + " #0000FF" + (*iter)->widget->getWidgetType());
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
	bool fullscreen;
	int width, height;
	MyGUI::ComboBoxPtr combo= mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_comboboxResolution");
	MyGUI::ComboBoxPtr comboFullScreen = mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_comboboxFullscreen");
	std::string tmp;
	std::istringstream str(combo->getCaption());
	str >> width >> tmp >> height;
	fullscreen = (comboFullScreen->getCaption() == "Yes");
	BasisManager::getInstance().mWindow->setFullscreen(fullscreen, width, height);
	mGUI->findWidgetT("LayoutEditor_windowSettings")->hide();
}

void EditorState::notifySelectWidget(MyGUI::WidgetPtr _sender)
{
	if (_sender == current_widget)
	{
		if (current_widget)
		{
			current_widget_rectangle->show();
			MyGUI::InputManager::getInstance().setKeyFocusWidget(current_widget_rectangle);
		}
		return;
	}

	current_widget = _sender;

	if (null == _sender)
		current_widget_rectangle->hide();
	else
	{
		MyGUI::IntCoord coord = current_widget->getCoord();
		MyGUI::WidgetPtr parent = current_widget->getParent();
		if (null != parent)
		{
			// если выбрали виджет на табе, то поднять лист таба
			if (parent->getWidgetType() == "Sheet")
			{
				MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(parent->getParent());
				tab->selectSheet(MyGUI::castWidget<MyGUI::Sheet>(parent));
			}
			coord = convertParentCoordToCoord(coord, current_widget);
		}
		current_widget_rectangle->show();
		current_widget_rectangle->setPosition(coord);
		MyGUI::InputManager::getInstance().setKeyFocusWidget(current_widget_rectangle);
	}

	updatePropertiesPanel(_sender);
}

void EditorState::updatePropertiesPanel(MyGUI::WidgetPtr _widget)
{
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowProperties");
	window->hide();
	MyGUI::WindowPtr windowStrings = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowStrings");
	windowStrings->hide();

	{
		for (MyGUI::VectorWidgetPtr::iterator iter = propertiesText.begin(); iter != propertiesText.end(); ++iter) mGUI->destroyWidget(*iter);
		propertiesText.clear();
		for (MyGUI::VectorWidgetPtr::iterator iter = propertiesElement.begin(); iter != propertiesElement.end(); ++iter) mGUI->destroyWidget(*iter);
		propertiesElement.clear();
	}

	if (null == _widget)
		allWidgetsCombo->setCaption("");
	else
	{
		window->show();
		int x1 = 0, x2 = 125;
		int w1 = 120;
		int w2 = window->getClientRect().width - w1 - 5;
		int y = 0;
		const int h = 20;
		WidgetType * widgetType = wt->find(current_widget->getWidgetType());
		WidgetContainer * widgetContainer = ew->find(current_widget);

		// show/update strings panel if needed
		if (widgetType->many_strings)
		{
			windowStrings->show();
			if (widgetType->name == "Tab") windowStrings->setCaption("Sheets");
			else windowStrings->setCaption("Items");
			syncStrings(false);
			MyGUI::WidgetPtr but = MyGUI::WidgetManager::getInstance().findWidgetT("LayoutEditor_buttonSelectString");
			if (widgetType->name == "Tab") but->show();
			else but->hide();
			MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editString");
			edit->setCaption("");
		}

		if (widgetContainer->name.empty()){
			// trim "LayoutEditor_"
			std::string tmp = current_widget->getName();
			if (0 == strncmp("LayoutEditor_", tmp.c_str(), 13))
			{
					std::string::iterator iter = std::find(tmp.begin(), tmp.end(), '_');
					if (iter != tmp.end()) tmp.erase(tmp.begin(), ++iter);
			}
			allWidgetsCombo->setCaption(MyGUI::utility::toString("[#333333", tmp, "#000000] #0000FF", current_widget->getWidgetType()));
		}
		else allWidgetsCombo->setCaption(widgetContainer->name + " #0000FF" + current_widget->getWidgetType());

		createPropertiesWidgetsPair(window, "Name", widgetContainer->name, "Name", x1, x2, w1, w2, y, h);
		y += h;
		if (widgetType->resizeable)
		{
			createPropertiesWidgetsPair(window, "Position", current_widget->getCoord().print(), "Position", x1, x2, w1, w2, y, h);
			y += h;
		}

		if (widgetType->skin.size() > 1)
		{
			createPropertiesWidgetsPair(window, "Skin", widgetContainer->skin, "Skin", x1, x2, w1, w2, y, h);
			y += h;
		}

		createPropertiesWidgetsPair(window, "Align", widgetContainer->align, "Align", x1, x2, w1, w2, y, h);
		y += h;

		if (null == current_widget->getParent())
		{
			createPropertiesWidgetsPair(window, "Layer", current_widget->getLayerName(), "Layer", x1, x2, w1, w2, y, h);
			y += h;
		}

		if (widgetType->name == "Widget")
		{
			if (current_widget->getWidgetType() != "Widget")
			{
				createSeparator(window, current_widget->getWidgetType() + " properties not aviable", x1, y, w1+w2+5, h);
				y += h;
			}
		}
		else
		{
			createSeparator(window, current_widget->getWidgetType() + " specific properties", x1, y, w1+w2+5, h);
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
		}

		createSeparator(window, "Other properties", x1, y, w1+w2+5, h);
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
		window->setMinMax(window->getSize().width, height + y, mGUI->getViewWidth(), height + y);
		window->setSize(window->getSize().width, height + y);
	}
}

void EditorState::createPropertiesWidgetsPair(MyGUI::WindowPtr _window, std::string _property, std::string _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h)
{
	MyGUI::StaticTextPtr text;
	MyGUI::WidgetPtr editOrCombo;
	//int string_int_float; // 0 - string, 1 - int, 2 - float

	int widget_for_type;// 0 - Edit, 1 - Combo, 2 - ...
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
	else if ("MessageButton" == _type) widget_for_type = 0;//"MessageButton" - тож хз
	// неправильно FIXME
	else if ("WidgetState" == _type) widget_for_type = 1;//по идее комба, но тогда надо еще и все состояния доступные в xml вписать
	else widget_for_type = 1;

	text = _window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w1, h, MyGUI::ALIGN_DEFAULT);
	std::string prop = _property;
	// trim widget name
	std::string::iterator iter = std::find(prop.begin(), prop.end(), '_');
	if (iter != prop.end()) prop.erase(prop.begin(), ++iter);
	text->setCaption(prop);
	text->setFontHeight(h-2);
	text->setTextAlign(MyGUI::ALIGN_RIGHT);

	if (widget_for_type == 0)
	{
		editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		if (_property != "RenderBox_Mesh" && _property != "Image_Material") MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditTextChange = newDelegate (this, &EditorState::notifyApplyProperties);
		MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditSelectAccept = newDelegate (this, &EditorState::notifyApplyProperties);
	}
	else if (widget_for_type == 1)
	{
		editOrCombo = _window->createWidget<MyGUI::ComboBox>("EditorComboBox", x2, y, w2, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
		MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->eventComboAccept = newDelegate (this, &EditorState::notifyApplyPropertiesCombo);

		std::vector<std::string> values;
		if (_type == "Skin") values = wt->find(current_widget->getWidgetType())->skin;
		else values = wt->findPossibleValues(_type);

		for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
			MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->addItem(*iter);

		MyGUI::castWidget<MyGUI::ComboBox>(editOrCombo)->setComboModeDrop(true);
	}

	editOrCombo->setUserString("action", _property);
	editOrCombo->setUserString("type", _type);
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

void EditorState::createSeparator(MyGUI::WindowPtr _window, std::string _caption, int x1, int y, int w, int h)
{
	propertiesElement.push_back(_window->createWidget<MyGUI::Widget>("Separator1", x1, y, w, 2, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH));
	MyGUI::StaticTextPtr text = _window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w, h, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	text->setCaption(_caption);
	text->setTextAlign(MyGUI::ALIGN_CENTER);
	propertiesText.push_back(text);
}

void EditorState::notifyApplyProperties(MyGUI::WidgetPtr _sender)
{
	ON_EXIT(um->addValue());
	WidgetContainer * widgetContainer = ew->find(current_widget);
	std::string action = _sender->getUserString("action");
	std::string value = MyGUI::castWidget<MyGUI::Edit>(_sender)->getOnlyText();
	std::string type = _sender->getUserString("type");

	if ((action == "Align") || (action == "Widget_AlignText"))
	{
		std::string tmp = "";
		const std::vector<std::string> & vec = MyGUI::utility::split(value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			if (!tmp.empty()) tmp += " ";
			tmp += "ALIGN_" + vec[pos];
		}
		value = tmp;
	}

	if (value == "DEFAULT") value = "";

	if (action == "Name")
	{
		if ((!value.empty()) && (null != ew->find(value)) && (widgetContainer != ew->find(value)))
		{
			MyGUI::Message::createMessage("Warning", "Widget with name '" + value + "' already exist.", true, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
			MyGUI::castWidget<MyGUI::Edit>(_sender)->setCaption(widgetContainer->name);
			return;
		}
		widgetContainer->name = value;
		return;
	}
	else if (action == "Skin")
	{
		widgetContainer->skin = value;
		MyGUI::xml::xmlDocument * save = ew->savexmlDocument();
		ew->clear();
		ew->loadxmlDocument(save);
		delete save;
		recreate = true;
		return;
	}
	else if (action == "Position")
	{
		widgetContainer->position = value;
		MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, "Widget_Move", value);
		return;
	}
	else if (action == "Align")
	{
		widgetContainer->align = value;
		widgetContainer->widget->setAlign(MyGUI::SkinManager::getInstance().parseAlign(value));
		return;
	}
	else if (action == "Layer")
	{
		widgetContainer->layer = value;
		MyGUI::LayerManager::getInstance().detachItem(current_widget);
		MyGUI::LayerManager::getInstance().attachItem(current_widget, widgetContainer->layer, true);
		return;
	}

	try{
		if (("Message_Modal" != action) && ("Window_AutoAlpha" != action))
		{
			if ((type == "1 int") || (type == "2 int") || (type == "4 int") || (type == "1 float") || (type == "2 float"))
			{
				if ((value != "") && (value.find_first_of("0123456789") != std::string::npos))
					MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);
			}
			else if (value != "" || "Widget_FontName" != action)
				MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);
		}
	}catch(...)
	{
		MyGUI::Message::createMessage("Warning", "No such " + action + ": '" + value + "'", true, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
	}// for incorrect meshes or materials

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
	if (!_sender->isShow()) return;
	// найдем соответствующий контейнер виджета и переместим/растянем
	WidgetContainer * widgetContainer = ew->find(current_widget);
	if (wt->find(current_widget->getWidgetType())->resizeable)
	{
		MyGUI::IntCoord coord = _sender->getCoord();
		MyGUI::IntCoord old_coord = convertParentCoordToCoord(current_widget->getCoord(), current_widget);
		// align to grid
		if (!shiftPressed  && !arrow_move){
			if ((old_coord.width == coord.width) && (old_coord.height == coord.height)) // если только перемещаем
			{
				coord.left = TO_GRID(coord.left + grid_step-1 - old_coord.left) + old_coord.left;
				coord.top = TO_GRID(coord.top + grid_step-1 - old_coord.top) + old_coord.top;
			}
			else // если растягиваем
			{
				if (old_coord.left != coord.left){
					coord.left = TO_GRID(coord.left + grid_step-1);
					coord.width = old_coord.right() - coord.left;
				}else if (old_coord.width != coord.width){
					coord.width = TO_GRID(coord.width + old_coord.left) - old_coord.left;
				}

				if (old_coord.top != coord.top){
					coord.top = TO_GRID(coord.top + grid_step-1);
					coord.height = old_coord.bottom() - coord.top;
				}else if (old_coord.height != coord.height){
					coord.height = TO_GRID(coord.height + old_coord.top) - old_coord.top;
				}
			}
		}
		arrow_move = false;

		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
		// update coord because of current_widget can have MinMax size
		coord = current_widget->getCoord();
		widgetContainer->position = coord.print();
		propertiesElement[1]->setCaption(coord.print());
		coord = convertParentCoordToCoord(current_widget->getCoord(), current_widget);
		current_widget_rectangle->setPosition(coord);
		um->addValue(PR_POSITION);
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

void EditorState::notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender)
{
	if (current_widget->getWidgetType() == "Tab")
	{
		addSheetToTab(current_widget);
		// update strings panel
		syncStrings(false);
		um->addValue();
	}
}

void EditorState::notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char)
{
	MyGUI::IntPoint delta;
	if (OIS::KC_TAB == _key)
	{
		if ((null != current_widget->getParent()) && (current_widget->getParent()->getWidgetType() == "Tab")) notifySelectWidget(current_widget->getParent());
		if (current_widget->getWidgetType() == "Tab")
		{
			MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(current_widget);
			size_t sheet = tab->getSelectSheetIndex();
			sheet++;
			if (sheet >= tab->getSheetCount()) sheet = 0;
			if (tab->getSheetCount()) tab->selectSheetIndex(sheet);
		}
	}
	else if (OIS::KC_LEFT == _key)
	{
		if (!shiftPressed) delta = MyGUI::IntPoint(-grid_step, 0);
		else delta = MyGUI::IntPoint(-1, 0);
	}
	else if (OIS::KC_RIGHT == _key)
	{
		if (!shiftPressed) delta = MyGUI::IntPoint(grid_step, 0);
		else delta = MyGUI::IntPoint(1, 0);
	}
	else if (OIS::KC_UP == _key)
	{
		if (!shiftPressed) delta = MyGUI::IntPoint(0, -grid_step);
		else delta = MyGUI::IntPoint(0, -1);
	}
	else if (OIS::KC_DOWN == _key)
	{
		if (!shiftPressed) delta = MyGUI::IntPoint(0, grid_step);
		else delta = MyGUI::IntPoint(0, 1);
	}

	if (delta != MyGUI::IntPoint())
	{
		arrow_move = true;
		current_widget_rectangle->setPosition(current_widget_rectangle->getPosition() + delta);
		notifyRectangleResize(current_widget_rectangle);
		um->addValue(PR_KEY_POSITION);
	}
}

void EditorState::addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption)
{
	MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(_tab);
	MyGUI::SheetPtr sheet = tab->addSheet(_caption);
	WidgetContainer * wc = new WidgetContainer("Sheet", "Sheet", sheet, _caption);
	if (!_caption.empty()) wc->mProperty.push_back(std::make_pair("Widget_Caption", _caption));
	ew->add(wc);
}

void EditorState::syncStrings(bool _apply, bool _add, Ogre::String _value)
{
	Ogre::String action;
	// FIXME/2 как-то громоздко и не настраиваемо...
	if (current_widget->getWidgetType() == "ComboBox") action = "Combo_AddString";
	else if (current_widget->getWidgetType() == "List") action = "List_AddString";
	//else if (current_widget->getWidgetType() == "Message") action = "Message_AddButton";
	else if (current_widget->getWidgetType() == "Tab") action = "Tab_AddSheet";

	WidgetContainer * widgetContainer = ew->find(current_widget);
	if (_apply)
	{
		if (_add)
		{
			if (action == "Tab_AddSheet")
			{
				addSheetToTab(current_widget, _value);
				um->addValue();
			}
			else
			{
				MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, _value);
				widgetContainer->mProperty.push_back(std::make_pair(action, _value));
			}
		}
		else
		{
			if (action == "Tab_AddSheet")
			{
				ew->remove(MyGUI::castWidget<MyGUI::Tab>(current_widget)->findSheet(_value));
			}
			else
			{
				int index = 0;
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
				{
					if (iterProperty->first == action){
						if (iterProperty->second == _value)
						{
							widgetContainer->mProperty.erase(iterProperty);
							if (current_widget->getWidgetType() == "ComboBox") MyGUI::castWidget<MyGUI::ComboBox>(current_widget)->deleteItem(index);
							else if (current_widget->getWidgetType() == "List") MyGUI::castWidget<MyGUI::List>(current_widget)->deleteItem(index);
							//else if (current_widget->getWidgetType() == "Message") MyGUI::castWidget<MyGUI::Message>(current_widget)->deleteItem(index);
							//else if (current_widget->getWidgetType() == "Tab") MyGUI::castWidget<MyGUI::Tab>(current_widget)->removeSheet(_value);
							return;
						}
						++index;
					}
				}
			}
		}
	}
	else // if !apply (if load)
	{
		MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
		list->deleteAllItems();
		if (action == "Tab_AddSheet")
		{
			for (size_t i = 0; i<MyGUI::castWidget<MyGUI::Tab>(current_widget)->getSheetCount(); ++i)
				list->addItem(MyGUI::castWidget<MyGUI::Tab>(current_widget)->getSheetNameIndex(i));
		}
		else
		{
			for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
			{
				if (iterProperty->first == action){
					list->addItem(iterProperty->second);
				}
			}
		}
	}
}

void EditorState::notifyAddString(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editString");
	syncStrings(true, true, edit->getOnlyText());
	list->addItem(edit->getOnlyText());
	um->addValue();
}

void EditorState::notifyDeleteString(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	size_t item = list->getItemSelect();
	if (ITEM_NONE == item) return;
	syncStrings(true, false, list->getItem(item));
	list->deleteItem(item);
	um->addValue();
}

void EditorState::notifySelectString(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	size_t item = list->getItemSelect();
	if (ITEM_NONE == item) return;
	ON_EXIT(um->addValue());
	MyGUI::castWidget<MyGUI::Tab>(current_widget)->selectSheetIndex(item);

	WidgetContainer * widgetContainer = ew->find(current_widget);

	Ogre::String action = "Tab_SelectSheet";
	Ogre::String value = MyGUI::utility::toString(item);
	MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);
	// если такое св-во было, то заменим (или удалим если стерли) значение
	for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
	{
		if (iterProperty->first == action){
			iterProperty->second = value;
			return;
		}
	}

	// если такого свойства не было раньше, то сохраняем
	widgetContainer->mProperty.push_back(std::make_pair(action, value));
}

void EditorState::notifyUpdateString(MyGUI::WidgetPtr _widget)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editString");
	size_t item = list->getItemSelect();
	if (ITEM_NONE == item) return;
	ON_EXIT(um->addValue());
	Ogre::String action;
	Ogre::String value = edit->getOnlyText();
	Ogre::String lastitem = list->getItem(item);
	list->setItem(item, value);

	if (current_widget->getWidgetType() == "ComboBox") action = "Combo_AddString";
	else if (current_widget->getWidgetType() == "List") action = "List_AddString";
	else if (current_widget->getWidgetType() == "Tab")
	{
		addSheetToTab(current_widget, value);
		return;
	}

	WidgetContainer * widgetContainer = ew->find(current_widget);
	int index = 0;
	for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
	{
		if (iterProperty->first == action){
			if (iterProperty->second == lastitem){
				iterProperty->second = value;
				if (current_widget->getWidgetType() == "ComboBox") MyGUI::castWidget<MyGUI::ComboBox>(current_widget)->setItem(index, value);
				else if (current_widget->getWidgetType() == "List") MyGUI::castWidget<MyGUI::List>(current_widget)->setItem(index, value);
				return;
			}
			++index;
		}
	}
}

void EditorState::notifySelectStringItem(MyGUI::WidgetPtr _widget, size_t _position)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listStrings");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editString");
	size_t item = list->getItemSelect();
	if (ITEM_NONE == item) return;
	Ogre::String value = list->getItem(item);
	edit->setOnlyText(value);
}
