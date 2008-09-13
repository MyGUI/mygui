#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#if defined OIS_WIN32_PLATFORM
#include "windows.h"
#endif

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define TO_GRID(x) ((x)/grid_step*grid_step)
#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

const std::string LogSection = "LayoutEditor";
const std::string DEFAULT_VALUE = "#444444[DEFAULT]";
const int PANELS_MARGIN = 3;
const int PANELS_MIN_HEIGHT = 25;
const MyGUI::ControllerPosition::MoveMode move_mode = MyGUI::ControllerPosition::Inertional;

const float POSITION_CONTROLLER_TIME = 0.5f;
const int HIDE_REMAIN_PIXELS = 3;

enum POPUP_MENU_MAIN
{
	ITEM_LOAD,
	ITEM_SAVE,
	ITEM_SAVE_AS,
	ITEM_CLEAR,
	ITEM_SETTINGS,
	ITEM_TEST,
	ITEM_QUIT
};

void eventInfo(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _event)
{
	MyGUI::MYGUI_OUT("eventInfo: ", _event);
}

void MapSet(StringPairs & _map, const std::string &_key, const std::string &_value)
{
	bool find = false;
	for (StringPairs::iterator iter=_map.begin(); iter!=_map.end(); ++iter) {
		if (iter->first == _key) {
			iter->second = _value;
			find = true;
		}
	}
	if (!find) {
		_map.push_back(std::make_pair(_key, _value));
	}
}

StringPairs::iterator MapFind(StringPairs & _map, const std::string &_key)
{
	StringPairs::iterator iter = _map.begin();
	for (; iter!=_map.end(); ++iter) {
		if (iter->first == _key) break;
	}
	return iter;
}

void MapErase(StringPairs & _map, const std::string &_key)
{
	for (StringPairs::iterator iter = _map.begin(); iter!=_map.end(); ++iter) {
		if (iter->first == _key) {
			_map.erase(iter);
			return;
		}
	}
}

//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	mGUI = MyGUI::Gui::getInstancePtr();

	MyGUI::LogManager::registerSection(LogSection, "MyGUI.log");
	wt = new WidgetTypes();
	wt->initialise();
	ew = new EditorWidgets();
	ew->initialise();
	um = new UndoManager();
	um->initialise(ew);

	langManager = MyGUI::LanguageManager::getInstancePtr();
	// set locale language if it was taken from OS
	if (! BasisManager::getInstance().getLanguage().empty() )
		langManager->setCurrentLanguage(BasisManager::getInstance().getLanguage());
	// if you want to test LanguageManager uncomment next line
	//langManager->setCurrentLanguage("Russian");

	mToolTip.initialise();
	mToolTip->hide();
	MyGUI::DelegateManager::getInstance().addDelegate("eventInfo", MyGUI::newDelegate(eventInfo));
	//MyGUI::DelegateManager::getInstance().addDelegate("eventEditorToolTip", MyGUI::newDelegate(this, &EditorState::notifyToolTip));

	interfaceWidgets = MyGUI::LayoutManager::getInstance().loadLayout("interface.layout", "LayoutEditor_");

	loadSettings();

	// создание меню
	bar = mGUI->createWidget<MyGUI::MenuBar>("MenuBar", MyGUI::IntCoord(0, 0, mGUI->getViewWidth(), 28), MyGUI::Align::Top | MyGUI::Align::HStretch, "Overlapped", "LayoutEditor_MenuBar");
	bar->addItem(localise("File"));
	bar->addItem(localise("Widgets"));
	// FIXME менюбар сунуть в лейаут
	interfaceWidgets.push_back(bar);

	mPopupMenuWidgets = bar->getItemMenu(1);

	mPopupMenuFile = bar->getItemMenu(0);
	mPopupMenuFile->addItem(localise("Load"));
	mPopupMenuFile->addItem(localise("Save"));
	mPopupMenuFile->addItem(localise("Save_as"));
	mPopupMenuFile->addItem(localise("Clear"), false, true);
	mPopupMenuFile->addItem(localise("Settings"));
	mPopupMenuFile->addItem(localise("Test"), false, true);
	mPopupMenuFile->addItem(localise("Quit"));

	bar->eventPopupMenuAccept = newDelegate(this, &EditorState::notifyPopupMenuAccept);

	if (MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkEdgeHide")->getButtonPressed())
	{
		for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
		{
			MyGUI::ControllerEdgeHide * controller = new MyGUI::ControllerEdgeHide(POSITION_CONTROLLER_TIME, HIDE_REMAIN_PIXELS, 3);
			MyGUI::ControllerManager::getInstance().addItem(*iter, controller);
		}
	}

	// widgets panel
	int w = widgetsButtonWidth, h = widgetsButtonHeight;

	MyGUI::TabPtr tabSkinGroups = mGUI->findWidget<MyGUI::Tab>("LayoutEditor_tabSkinGroups");
	MyGUI::SheetPtr sheet;
	int maxLines = 0;
	for (SkinGroups::iterator iter = wt->skin_groups.begin(); iter != wt->skin_groups.end(); ++iter)
	{
		sheet = tabSkinGroups->addSheet(iter->first);
		int i = 0;
		for (StringPairs::iterator iterSkin = iter->second.begin(); iterSkin != iter->second.end(); ++iterSkin)
		{
			MyGUI::ButtonPtr button = sheet->createWidget<MyGUI::Button>("ButtonSmall", 
				i%widgetsButtonsInOneLine * w, i/widgetsButtonsInOneLine * h, w, h,
				MyGUI::Align::Top|MyGUI::Align::Left, MyGUI::utility::toString(iterSkin->second, iterSkin->first));
			button->setCaption(iterSkin->first);
			button->setTextAlign(MyGUI::Align::Center);
			button->setUserString("widget", iterSkin->second);
			button->setUserString("skin", iterSkin->first);
			button->setUserString("TooTipText", "Widget: " + iterSkin->second + "\nSkin: " + iterSkin->first);
			button->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetType);
			button->eventMouseButtonDoubleClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetTypeDoubleclick);
			button->setNeedToolTip(true);
			button->eventToolTip = MyGUI::newDelegate(this, &EditorState::notifyToolTip);
			i++;
		}
		maxLines = std::max((i+widgetsButtonsInOneLine-1)/widgetsButtonsInOneLine, maxLines);
	}

	int width = tabSkinGroups->getWidth() - sheet->getWidth();
	int height = tabSkinGroups->getHeight() - sheet->getHeight();
	tabSkinGroups->setSize(width + widgetsButtonsInOneLine * w, height + maxLines*h);

	allWidgetsCombo = mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_allWidgetsCombo");
	allWidgetsCombo->setComboModeDrop(true);
	allWidgetsCombo->eventComboChangePosition = MyGUI::newDelegate(this, &EditorState::notifyAllWidgetsSelect);
	allWidgetsCombo->setMaxListHeight(200);

	MyGUI::WindowPtr windowWidgets = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowWidgets");
	width = windowWidgets->getWidth() - windowWidgets->getClientCoord().width;
	height = windowWidgets->getHeight() - windowWidgets->getClientCoord().height;
	windowWidgets->setSize(width + tabSkinGroups->getWidth(), height + tabSkinGroups->getHeight() + 2*h);

	// properties panel
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowProperties");
	window->setPosition(mGUI->getViewWidth() - window->getSize().width, 0);
	ASSIGN_FUNCTION("LayoutEditor_buttonRelativePosition", &EditorState::notifyToggleRelativeMode);

	// settings panel
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("LayoutEditor_gridEdit");
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
	ASSIGN_FUNCTION("LayoutEditor_buttonOkSettings", &EditorState::notifyOkSettings);

	MyGUI::ComboBoxPtr combo= mGUI->findWidget<MyGUI::ComboBox>("LayoutEditor_comboboxResolution");
	Ogre::ConfigOptionMap map = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();
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
	for (size_t j = 0; j<iter->second.possibleValues.size();j++){
		Ogre::String videoMode = iter->second.possibleValues[j];
		if(iter->second.possibleValues[j] == iter->second.currentValue)
			selectedIdx = j;
		comboFullScreen->addItem(videoMode);
	}
	comboFullScreen->setItemSelect(selectedIdx);

	ASSIGN_FUNCTION("LayoutEditor_checkShowName", &EditorState::notifyToggleCheck);
	ASSIGN_FUNCTION("LayoutEditor_checkShowType", &EditorState::notifyToggleCheck);
	ASSIGN_FUNCTION("LayoutEditor_checkShowSkin", &EditorState::notifyToggleCheck);
	ASSIGN_FUNCTION("LayoutEditor_checkEdgeHide", &EditorState::notifyToggleCheck);

	// minimize panel buttons
	panelMinimizeButtons.push_back(mGUI->findWidgetT("LayoutEditor_minimizeSpecificProperties"));
	panelMinimizeButtons.push_back(mGUI->findWidgetT("LayoutEditor_minimizeWidgetProperties"));
	panelMinimizeButtons.push_back(mGUI->findWidgetT("LayoutEditor_minimizeItem"));
	panelMinimizeButtons.push_back(mGUI->findWidgetT("LayoutEditor_minimizeUserData"));
	for (MyGUI::VectorWidgetPtr::iterator iter = panelMinimizeButtons.begin(); iter != panelMinimizeButtons.end(); ++iter)
	{
		panels.push_back((*iter)->getParent());
		(*iter)->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifyMinimizePanel);
	}

	// strings panel
	ASSIGN_FUNCTION("LayoutEditor_buttonAddItem", &EditorState::notifyAddItem);
	ASSIGN_FUNCTION("LayoutEditor_buttonDeleteItem", &EditorState::notifyDeleteItem);
	ASSIGN_FUNCTION("LayoutEditor_buttonSelectSheet", &EditorState::notifySelectSheet);
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editItem");
	edit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyUpdateItem);
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	list->eventListChangePosition = MyGUI::newDelegate(this, &EditorState::notifySelectItem);

	// UserData panel
	ASSIGN_FUNCTION("LayoutEditor_buttonAddUserData", &EditorState::notifyAddUserData);
	ASSIGN_FUNCTION("LayoutEditor_buttonDeleteUserData", &EditorState::notifyDeleteUserData);
	MyGUI::EditPtr editKey = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editKeyUserData");
	editKey->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyUpdateUserData);
	MyGUI::EditPtr editValue = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editValueUserData");
	editValue->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyUpdateUserData);
	MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
	multilist->eventListChangePosition = MyGUI::newDelegate(this, &EditorState::notifySelectUserDataItem);
	multilist->addColumn(editKey->getWidth() - PANELS_MARGIN, localise("Key"));
	multilist->addColumn(multilist->getWidth() - editKey->getWidth() - PANELS_MARGIN, localise("Value"));

	// create widget rectangle
	current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", MyGUI::IntCoord(), MyGUI::Align::Default, "LayoutEditor_Rectangle");
	current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
	current_widget_rectangle->eventMouseButtonDoubleClick = newDelegate(this, &EditorState::notifyRectangleDoubleClick);
	current_widget_rectangle->eventKeyButtonPressed = newDelegate(this, &EditorState::notifyRectangleKeyPressed);

	clear();

	/*MyGUI::WidgetPtr mFpsInfo = mGUI->createWidget<MyGUI::Widget>("ButtonSmall", 20, (int)mGUI->getViewHeight() - 80, 120, 70, MyGUI::Align::Left | MyGUI::Align::Bottom, "Main", "fpsInfo");
	mFpsInfo->setColour(Ogre::ColourValue::White);*/

	typedef std::vector<std::string> Params;
	Params params = BasisManager::getInstance().getCommandParams();
	for (Params::iterator iter=params.begin(); iter!=params.end(); ++iter) {
		load(iter->c_str());
	}
}

void EditorState::notifyPopupMenuAccept(MyGUI::WidgetPtr _sender, MyGUI::PopupMenuPtr _menu, size_t _index)
{
	if (mPopupMenuFile == _menu) {
		switch(_index) {
			case ITEM_LOAD:
				notifyLoadSaveAs(false);
				break;
			case ITEM_SAVE:
				notifySave();
				break;
			case ITEM_SAVE_AS:
				notifyLoadSaveAs(true);
				break;
			case ITEM_SETTINGS:
				notifySettings();
				break;
			case ITEM_TEST:
				notifyTest();
				break;
			case ITEM_CLEAR:
				notifyClear();
				break;
			case ITEM_QUIT:
				notifyQuit();
				break;
		}
	}
}

void EditorState::exit()
{
	saveSettings();
	um->shutdown();
	delete um;
	ew->shutdown();
	delete ew;
	wt->shutdown();
	delete wt;
}
//===================================================================================
bool EditorState::mouseMoved( const OIS::MouseEvent &arg )
{
	if (testMode){ mGUI->injectMouseMove(arg); return true;}

	// align to grid if shift not pressed
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x2 = TO_GRID(arg.state.X.abs);
		y2 = TO_GRID(arg.state.Y.abs);
	}
	else
	{
		x2 = arg.state.X.abs;
		y2 = arg.state.Y.abs;
	}

	if ((creating_status == 1) && ((x1-x2)*(y1-y2) != 0))
	{
		MyGUI::IntCoord coord(std::min(x1, x2), std::min(y1, y2), abs(x1 - x2), abs(y1 - y2));

		creating_status = 2;

		// внимание current_widget родитель и потом сразу же сын
		std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", current_widget_type, ew->global_counter);
		ew->global_counter++;
		// пока не найдем ближайшего над нами способного быть родителем
		while (current_widget && false == wt->find(current_widget->getWidgetType())->parent) current_widget = current_widget->getParent();
		if (current_widget && wt->find(current_widget_type)->child)
		{
			coord = coord - current_widget->getPosition();
			current_widget = current_widget->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::Align::Default, tmpname);
		}
		else current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::Align::Default, "Back", tmpname);

		current_widget->setCaption(MyGUI::utility::toString("#888888",current_widget_skin));
	}
	else if (creating_status == 2)
	{
		MyGUI::IntCoord coord(std::min(x1, x2), std::min(y1, y2), abs(x1 - x2), abs(y1 - y2));
		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
	}

	mGUI->injectMouseMove(arg);
	return true;
}
//===================================================================================
bool EditorState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (testMode){ mGUI->injectMousePress(arg, id); return true;}

	if (MyGUI::InputManager::getInstance().isModalAny())
	{
		// if we have modal widgets we can't select any widget
		mGUI->injectMousePress(arg, id);
		return true;
	}

	// align to grid if shift not pressed
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x1 = TO_GRID(arg.state.X.abs);
		y1 = TO_GRID(arg.state.Y.abs);
	}
	else
	{
		x1 = arg.state.X.abs;
		y1 = arg.state.Y.abs;
	}

	if (id == OIS::MB_Left && !creating_status && current_widget_type != "") creating_status = 1;

	// это чтобы можно было двигать прямоугольник у невидимых виджето (или виджетов за границами)
	//MyGUI::LayerItemInfoPtr rootItem = null;
	//MyGUI::WidgetPtr itemWithRect = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	// не стал это доделывать, т.к. неоднозначность выбора виджета получается, если кто скажет как выбирать - сделаю

	MyGUI::LayerItem * rootItem = null;
	MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance()._findLayerItem(arg.state.X.abs, arg.state.Y.abs, rootItem));

	// не убираем прямоугольник если нажали на его растягивалку
	if (item && (item->getParent() != current_widget_rectangle))
	{
		// чтобы прямоугольник не мешался
		current_widget_rectangle->hide();
		item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance()._findLayerItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	}

	if (null != item)
	{
		while ((null == ew->find(item)) && (null != item)) item = item->getParent();

		if (null != item) // нашли
		{
			MyGUI::IntSize size = item->getTextSize();
			notifySelectWidget(item);
			if (creating_status != 1){
				mGUI->injectMouseMove(arg);// это чтобы сразу можно было тащить
			}
		}
		mGUI->injectMousePress(arg, id);
	}
	else {
		mGUI->injectMousePress(arg, id);
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
	if (testMode){ mGUI->injectMouseRelease(arg, id); return true;}

	if (MyGUI::InputManager::getInstance().isModalAny())
	{
	}
	else
	{
		// align to grid if shift not pressed
		if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
		{
			x2 = TO_GRID(arg.state.X.abs);
			y2 = TO_GRID(arg.state.Y.abs);
		}
		else
		{
			x2 = arg.state.X.abs;
			y2 = arg.state.Y.abs;
		}

		if (creating_status > 0)
		{
			if ((x1-x2)*(y1-y2) != 0)
			{
				// создали виджет, все счастливы
				WidgetContainer * widgetContainer = new WidgetContainer(current_widget_type, current_widget_skin, current_widget);
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
	mGUI->injectMouseRelease(arg, id);

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
	MyGUI::InputManager & input = MyGUI::InputManager::getInstance();

	if (testMode)
	{
		if (input.isModalAny() == false)
		{
			if ( arg.key == OIS::KC_ESCAPE )
			{
				if (MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkEdgeHide")->getButtonPressed())
				{
					for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
					{
						MyGUI::ControllerEdgeHide * controller = new MyGUI::ControllerEdgeHide(POSITION_CONTROLLER_TIME, HIDE_REMAIN_PIXELS, 3);
						MyGUI::ControllerManager::getInstance().addItem(*iter, controller);
					}
				}
				for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
				{
					(*iter)->setPosition((*iter)->getPosition() + MyGUI::IntPoint(2048, 2048));
				}
				bar->show();
				testMode = false;
				clear();
				ew->loadxmlDocument(testLayout);
			}
		}
		mGUI->injectKeyPress(arg);
		return true;
	}

	if (input.isModalAny())
	{
		if (null != mGUI->findWidgetT("LayoutEditor_windowSaveLoad"))
		{
			if (arg.key == OIS::KC_ESCAPE) notifyLoadSaveCancel();
			else if (arg.key == OIS::KC_RETURN) notifyLoadSaveEditAccept();
		}
	}
	else
	{
		if ( arg.key == OIS::KC_ESCAPE )
		{
			notifyQuit(); return true;
		}
		else if ( arg.key == OIS::KC_DELETE )
		{
			if (input.getKeyFocusWidget() == current_widget_rectangle){
				if (current_widget){ ew->remove(current_widget); notifySelectWidget(null); }
				um->addValue();
			}
		}
		if (input.isControlPressed())
		{
			if (arg.key == OIS::KC_O || arg.key == OIS::KC_L) notifyLoadSaveAs(false);
			else if (arg.key == OIS::KC_S) notifySave();
			else if (arg.key == OIS::KC_Z){
				um->undo();
				notifySelectWidget(null);
			}
			else if ((arg.key == OIS::KC_Y) || ((input.isShiftPressed()) && (arg.key == OIS::KC_Z))){
				um->redo();
				notifySelectWidget(null);
			}
			else if (arg.key == OIS::KC_T)
			{
				notifyTest();
				return true;
			}
			else if (arg.key == OIS::KC_R)
			{
				notifyToggleRelativeMode();
				return true;
			}
		}
	}

	mGUI->injectKeyPress(arg);
	return true;
}
//===================================================================================
bool EditorState::keyReleased( const OIS::KeyEvent &arg )
{
	if (testMode){ mGUI->injectKeyRelease(arg); return true;}

	mGUI->injectKeyRelease(arg);
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
			mGUI->findWidgetT("fpsInfo")->setCaption(MyGUI::utility::toString("FPS : ", stats.lastFPS, "\ntriangle : ", stats.triangleCount, "\nbatch : ", stats.batchCount));
		} catch (...) { }
	}*/
	if (ew->widgets_changed)
	{
		notifyWidgetsUpdate();
		ew->widgets_changed = false;
	}

	mGUI->injectFrameEntered(evt.timeSinceLastFrame);
	return true;
}
//===================================================================================
void EditorState::windowResize()
{
	if (testMode) return;

	// force update
	MyGUI::WidgetPtr current_widget1 = current_widget;
	current_widget = null;
	notifySelectWidget(current_widget1);
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
					else if (key == "ShowName")
						MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowName")->setButtonPressed(MyGUI::utility::parseBool(value));
					else if (key == "ShowType")
						MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowType")->setButtonPressed(MyGUI::utility::parseBool(value));
					else if (key == "ShowSkin")
						MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowSkin")->setButtonPressed(MyGUI::utility::parseBool(value));
					else if (key == "EdgeHide")
						MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkEdgeHide")->setButtonPressed(MyGUI::utility::parseBool(value));
					else if (key == "widgetsButtonWidth") widgetsButtonWidth = MyGUI::utility::parseInt(value);
					else if (key == "widgetsButtonHeight") widgetsButtonHeight = MyGUI::utility::parseInt(value);
					else if (key == "widgetsButtonsInOneLine") widgetsButtonsInOneLine = MyGUI::utility::parseInt(value);
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

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowName");
	nodeProp->addAttributes("value", MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowName")->getButtonPressed());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowType");
	nodeProp->addAttributes("value", MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowType")->getButtonPressed());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "ShowSkin");
	nodeProp->addAttributes("value", MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowSkin")->getButtonPressed());

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "EdgeHide");
	nodeProp->addAttributes("value", MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkEdgeHide")->getButtonPressed());

	// actually properties below can't be changed in Editor - only by editing settings.xml manually
	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "widgetsButtonWidth");
	nodeProp->addAttributes("value", widgetsButtonWidth);

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "widgetsButtonHeight");
	nodeProp->addAttributes("value", widgetsButtonHeight);

	nodeProp = root->createChild("Property");
	nodeProp->addAttributes("key", "widgetsButtonsInOneLine");
	nodeProp->addAttributes("value", widgetsButtonsInOneLine);

	if (false == doc.save(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}
}

void EditorState::notifySave()
{
	if (fileName != "")
	{
		if ( !ew->save(fileName)) {
			Ogre::DisplayString file_name = anci_to_utf16(fileName);
			MyGUI::Message::_createMessage(localise("Warning"), "Failed to save file '" + file_name + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
		}
	}
	else notifyLoadSaveAs(true);
}

void EditorState::notifyLoadSaveAs(bool _save)
{
	// create message box with file name and two buttons
	MyGUI::WidgetPtr messageWindow = MyGUI::LayoutManager::getInstance().load("SaveLoadMessage.layout")[0];
	MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
	MyGUI::IntSize size(messageWindow->getSize());
	messageWindow->setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);
	MyGUI::InputManager::getInstance().addWidgetModal(messageWindow);
	
	//MyGUI::VectorWidgetPtr childs = messageWindow->getChilds();
	//if (_save) childs[1]->setCaption("Save");
	//else childs[1]->setCaption("Load");
	//childs[1]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveAccept);
	//childs[2]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveCancel);

	// жесткий код, но оригинал не лучше =/
	MyGUI::WidgetPtr combo2 = null, button1 = null, button2 = null;
	size_t pos = 0;

	MyGUI::EnumeratorWidgetPtr childs = messageWindow->getEnumerator();
	while(childs.next()) {
		if (pos == 0) combo2 = childs.current();
		else if (pos == 1) button1 = childs.current();
		else if (pos == 2) button2 = childs.current();
		pos++;
	};

	if (_save) button1->setCaption("Save");
	else button1->setCaption("Load");
	button1->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveAccept);
	button2->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveCancel);

	// set fileName in edit
	MyGUI::ComboBoxPtr combo = MyGUI::castWidget<MyGUI::ComboBox>(combo2);
	if (fileName != "") {
		const Ogre::DisplayString & item = anci_to_utf16(fileName);
		combo->setCaption(item);
	}
	combo->eventEditSelectAccept = newDelegate(this, &EditorState::notifyLoadSaveEditAccept);
	std::vector<Ogre::String> strs = MyGUI::helper::getVectorResourcePath("*.layout");
	for (std::vector<Ogre::String>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		const Ogre::DisplayString & item = anci_to_utf16(*iter);
		combo->addItem(item);
	}

}

void EditorState::notifySettings()
{
	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowSettings");
	window->show();
	MyGUI::LayerManager::getInstance().upLayerItem(window);
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("LayoutEditor_gridEdit");
	gridEdit->setCaption(MyGUI::utility::toString(grid_step));
}

void EditorState::notifyTest()
{
	for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
	{
		MyGUI::ControllerManager::getInstance().removeItem(*iter);
		(*iter)->setPosition((*iter)->getPosition() + MyGUI::IntPoint(-2048, -2048));
	}
	bar->hide();
	testLayout = ew->savexmlDocument();
	ew->clear();
	notifySelectWidget(null);
	ew->loadxmlDocument(testLayout, true);
	testMode = true;
}

void EditorState::notifyClear()
{
	MyGUI::Message::_createMessage(localise("Warning"), localise("Warn_delete_all_widgets"), "", "Overlapped", true, newDelegate(this, &EditorState::notifyClearMessage), MyGUI::Message::IconWarning | MyGUI::Message::Yes | MyGUI::Message::No);
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
	want_quit = false;
	fileName = "";
	testMode = false;
	ew->clear();
	notifySelectWidget(null);
	um->addValue();

	BasisManager::getInstance().setWindowCaption("MyGUI Layout Editor");
}

void EditorState::notifyQuit()
{
	MyGUI::Message::_createMessage(localise("Warning"), localise("Warn_exit"), "", "Overlapped", true, newDelegate(this, &EditorState::notifyQuitMessage), MyGUI::Message::IconWarning | MyGUI::Message::Yes | MyGUI::Message::No);
	want_quit = true;
}

void EditorState::notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)
{
	if (_button == MyGUI::Message::Yes || _button == MyGUI::Message::Button1) {
		BasisManager::getInstance().eventExit();
	}
	want_quit = false;
}

void EditorState::notifyLoadSaveAccept(MyGUI::WidgetPtr _sender)
{
	bool success;
	Ogre::UTFString file_name = mGUI->findWidget<MyGUI::Edit>("LayoutEditor_editFileName")->getCaption();
	// конвертируем
	std::string fName = utf16_to_anci(file_name);

	if (_sender->getCaption() == "Load") success = ew->load(fName);
	else/*(_sender->getCaption() == "Save")*/ success = ew->save(fName);

	if (false == success) 
	{
		MyGUI::Message::_createMessage(localise("Warning"), "Failed to " + _sender->getCaption() + " file '" + file_name + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
	}
	else
	{
		// запоминает последнее удачное имя файла
		fileName = fName;
		BasisManager::getInstance().setWindowCaption(fileName + " - MyGUI Layout Editor");
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

void EditorState::load(const std::string & _file)
{
	if (!ew->load(_file))
	{
		Ogre::DisplayString file_name = anci_to_utf16(fileName);
		MyGUI::Message::_createMessage(localise("Warning"), "Failed to load file '" + file_name + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
		return;
	}

	BasisManager::getInstance().setWindowCaption(_file + " - MyGUI Layout Editor");

	fileName = _file;
	um->addValue();
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

	std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", current_widget_type, ew->global_counter);
	ew->global_counter++;

	while (current_widget && false == wt->find(current_widget->getWidgetType())->parent) current_widget = current_widget->getParent();
	if (current_widget && wt->find(current_widget_type)->child)
		current_widget = current_widget->createWidgetT(current_widget_type, current_widget_skin, MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default, tmpname);
	else
	{
		MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
		current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, MyGUI::IntCoord(), MyGUI::Align::Default, "Back", tmpname);
		MyGUI::IntSize size(current_widget->getSize());
		current_widget->setPosition((view.width-size.width)/2, (view.height-size.height)/2, 100/*DEFAULT*/, /*DEFAULT*/100); // FIXME
	}
	current_widget->setCaption(MyGUI::utility::toString("#888888",current_widget_skin));

	WidgetContainer * widgetContainer = new WidgetContainer(current_widget_type, current_widget_skin, current_widget);
	ew->add(widgetContainer);
	current_widget = null;
	notifySelectWidget(widgetContainer->widget);
	mGUI->findWidget<MyGUI::Button>(MyGUI::utility::toString(current_widget_type, current_widget_skin))->setButtonPressed(false);
	current_widget_type = "";
	current_widget_skin = "";

	um->addValue();
}

void EditorState::notifyWidgetsUpdate()
{
	allWidgetsCombo->deleteAllItems();

	bool print_name = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowName")->getButtonPressed();
	bool print_type = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowType")->getButtonPressed();
	bool print_skin = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowSkin")->getButtonPressed();

	for (std::vector<WidgetContainer*>::iterator iter = ew->widgets.begin(); iter != ew->widgets.end(); ++iter )
	{
		allWidgetsCombo->addItem(getDescriptionString((*iter)->widget, print_name, print_type, print_skin));
	}

	mPopupMenuWidgets->deleteAllItems();

	for (std::vector<WidgetContainer*>::iterator iter = ew->widgets.begin(); iter != ew->widgets.end(); ++iter )
	{
		createWidgetPopup(*iter, mPopupMenuWidgets, print_name, print_type, print_skin);
	}
}

void EditorState::createWidgetPopup(WidgetContainer* _container, MyGUI::PopupMenuPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
{
	_parentPopup->eventPopupMenuAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);
	bool submenu = !_container->childContainers.empty();
	MyGUI::PopupMenu::ItemInfo & item = _parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu);
	item.data = _container->widget;
	for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
	{
		createWidgetPopup(*iter, item.submenu, _print_name, _print_type, _print_skin);
	}
}

void EditorState::notifyWidgetsSelect(MyGUI::WidgetPtr _widget, size_t _index)
{
	void * data = MyGUI::castWidget<MyGUI::PopupMenu>(_widget)->getItemInfo(_index).data;
	MyGUI::WidgetPtr widget = static_cast<MyGUI::WidgetPtr>(data);
	notifySelectWidget(widget);
}

void EditorState::notifyAllWidgetsSelect(MyGUI::WidgetPtr _widget, size_t _index)
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
	BasisManager::getInstance().setFullscreen(fullscreen);//setFullscreen, width, height);
	mGUI->findWidgetT("LayoutEditor_windowSettings")->hide();
}

void EditorState::notifyToggleCheck(MyGUI::WidgetPtr _sender)
{
	MyGUI::ButtonPtr checkbox = MyGUI::castWidget<MyGUI::Button>(_sender);
	checkbox->setButtonPressed(!checkbox->getButtonPressed());
	notifyWidgetsUpdate();
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
		MyGUI::LayerManager::getInstance().upLayerItem(current_widget);
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
			// если выбрали лист таба, то поднять лист таба
			if (current_widget->getWidgetType() == "Sheet")
			{
				MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(parent);
				tab->selectSheet(MyGUI::castWidget<MyGUI::Sheet>(current_widget));
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
	// delete all previous properties
	for (MyGUI::VectorWidgetPtr::iterator iter = propertiesText.begin(); iter != propertiesText.end(); ++iter) mGUI->destroyWidget(*iter);
	propertiesText.clear();
	for (MyGUI::VectorWidgetPtr::iterator iter = propertiesElement.begin(); iter != propertiesElement.end(); ++iter) mGUI->destroyWidget(*iter);
	propertiesElement.clear();

	MyGUI::WindowPtr window = mGUI->findWidget<MyGUI::Window>("LayoutEditor_windowProperties");

	if (null == _widget)
	{
		window->hide();
		allWidgetsCombo->setCaption("");
		allWidgetsCombo->resetItemSelect();
	}
	else
	{
		window->show();
		int x1 = 0, x2 = 125;
		int w1 = 120;
		int w2 = panels[0]->getWidth() - w1 - 4*PANELS_MARGIN;
		int y = 0;
		const int h = 22;
		WidgetType * widgetType = wt->find(current_widget->getWidgetType());
		WidgetContainer * widgetContainer = ew->find(current_widget);

		bool print_name = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowName")->getButtonPressed();
		bool print_type = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowType")->getButtonPressed();
		bool print_skin = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowSkin")->getButtonPressed();
		allWidgetsCombo->setCaption(getDescriptionString(current_widget, print_name, print_type, print_skin));

		createPropertiesWidgetsPair(window, "Name", widgetContainer->name, "Name", x1, x2, w1, w2, y, h);
		y += h;

		MyGUI::WidgetPtr buttonRelativePosition = mGUI->findWidget<MyGUI::Widget>("LayoutEditor_buttonRelativePosition");
		if (widgetType->resizeable)
		{
			// update caption of LayoutEditor_buttonRelativePosition
			buttonRelativePosition->show();
			if (widgetContainer->relative_mode) buttonRelativePosition->setCaption(localise("to_pixels"));
			else buttonRelativePosition->setCaption(localise("to_percents"));
	
			int brpWidth = buttonRelativePosition->getWidth();
			createPropertiesWidgetsPair(window, "Position", widgetContainer->position(), "Position", x1 + brpWidth, x2, w1 - brpWidth, w2, y, h);
			y += h;
		}
		else
		{
			buttonRelativePosition->hide();
		}

		createPropertiesWidgetsPair(window, "Align", widgetContainer->align, "Align", x1, x2, w1, w2, y, h);
		y += h;

		if (null == current_widget->getParent())
		{
			createPropertiesWidgetsPair(window, "Layer", widgetContainer->layer, "Layer", x1, x2, w1, w2, y, h);
			y += h;
		}

		if (widgetType->skin.size() > 1)
		{
			createPropertiesWidgetsPair(window, "Skin", widgetContainer->skin, "Skin", x1, x2, w1, w2, y, h);
			y += h;
		}

		panels[0]->setPosition(panels[0]->getLeft(), y + PANELS_MARGIN);
		if (widgetType->name == "Widget")
		{
			if (current_widget->getWidgetType() != "Widget")
			{
				panels[0]->setCaption(current_widget->getWidgetType() + " properties not aviable");
				y += h + 3*PANELS_MARGIN;
			}
		}
		else
		{
			langManager->addTag("widget_type", current_widget->getWidgetType());
			panels[0]->setCaption(langManager->replaceTags(localise("Widget_type_propertes")));
			y += h + 3*PANELS_MARGIN;

			//all other
			for (StringPairs::iterator iter = widgetType->parameter.begin(); iter != widgetType->parameter.end(); ++iter)
			{
				std::string value = "";
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
				createPropertiesWidgetsPair(panels[0], iter->first, value, iter->second, x1, x2, w1, w2, y - panels[0]->getTop(), h);
				y += h;
			}
			if (widgetType->parameter.empty()){ panels[0]->hide();}
			else panels[0]->show();
		}
		y += PANELS_MARGIN;
		panels[0]->setUserString("Size", MyGUI::utility::toString(y - panels[0]->getTop()));
		std::string resizing = panels[0]->getUserString("resizing");
		if (resizing == "up")
			panels[0]->setSize(panels[0]->getWidth(), PANELS_MIN_HEIGHT);
		else
			panels[0]->setSize(panels[0]->getWidth(), y - panels[0]->getTop());
		y = panels[0]->getTop() + (panels[0]->getHeight() + PANELS_MARGIN)*panels[0]->isShow();

		panels[1]->setPosition(panels[1]->getLeft(), y);
		panels[1]->setCaption(localise("Other_properties"));
		y += h + 3*PANELS_MARGIN;

		if (current_widget->getWidgetType() != "Sheet")
		{
			panels[1]->show();
			//base properties (from Widget)
			WidgetType * baseType = wt->find("Widget");
			for (StringPairs::iterator iter = baseType->parameter.begin(); iter != baseType->parameter.end(); ++iter)
			{
				std::string value = "";
				for (StringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
					if (iterProperty->first == iter->first){ value = iterProperty->second; break;}
				createPropertiesWidgetsPair(panels[1], iter->first, value, iter->second, x1, x2, w1, w2, y - panels[1]->getTop(), h);
				y += h;
			}
		}
		else
		{
			panels[1]->hide();
		}
		y += PANELS_MARGIN;
		panels[1]->setUserString("Size", MyGUI::utility::toString(y - panels[1]->getTop()));
		resizing = panels[1]->getUserString("resizing");
		if (resizing == "up")
			panels[1]->setSize(panels[1]->getWidth(), PANELS_MIN_HEIGHT);
		else
			panels[1]->setSize(panels[1]->getWidth(), y - panels[1]->getTop());
		y = panels[1]->getTop() + (panels[1]->getHeight() + PANELS_MARGIN)*panels[1]->isShow();

		// show/update strings panel if needed
		MyGUI::WidgetPtr panelItems = panels[2];
		panelItems->setPosition(PANELS_MARGIN, y);
		if (widgetType->many_items)
		{
			panelItems->show();
			if (widgetType->name == "Tab") panelItems->setCaption("Sheets");
			else panelItems->setCaption("Items");
			syncItems(false);
			MyGUI::WidgetPtr but = MyGUI::WidgetManager::getInstance().findWidgetT("LayoutEditor_buttonSelectSheet");
			if (widgetType->name == "Tab") but->show();
			else but->hide();
			MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editItem");
			edit->setCaption("");
		}
		else
		{
			panelItems->hide();
		}

		y = panelItems->getTop() + (panelItems->getHeight() + PANELS_MARGIN)*panelItems->isShow();

		MyGUI::EditPtr editKey = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editKeyUserData");
		MyGUI::EditPtr editValue = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editValueUserData");
		MyGUI::WidgetPtr panelUserData = panels[3];
		panelUserData->setPosition(PANELS_MARGIN, y);
		MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
		multilist->deleteAllItems();
		for (StringPairs::iterator iterProperty = widgetContainer->mUserString.begin(); iterProperty != widgetContainer->mUserString.end(); ++iterProperty)
		{
			multilist->addItem(iterProperty->first);
			multilist->setSubItem(1, multilist->getItemCount() - 1, iterProperty->second);
		}

		y = panelUserData->getTop() + (panelUserData->getHeight() + PANELS_MARGIN)*panelUserData->isShow();

		int height = window->getHeight() - window->getClientCoord().height;
		window->setMinMax(window->getMinMax().left, height + y, mGUI->getViewWidth(), height + y);
		window->setSize(window->getSize().width, height + y);
	}
}

void EditorState::createPropertiesWidgetsPair(MyGUI::WidgetPtr _window, std::string _property, std::string _value, std::string _type, int x1, int x2, int w1, int w2 ,int y, int h)
{
	MyGUI::StaticTextPtr text;
	MyGUI::WidgetPtr editOrCombo;
	//int string_int_float; // 0 - string, 1 - int, 2 - float

	int widget_for_type;// 0 - Edit, 1 - Combo mode drop, 2 - ...
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

	text = _window->createWidget<MyGUI::StaticText>("Editor_StaticText", x1, y, w1, h, MyGUI::Align::Default);
	std::string prop = _property;
	// trim widget name
	std::string::iterator iter = std::find(prop.begin(), prop.end(), '_');
	if (iter != prop.end()) prop.erase(prop.begin(), ++iter);
	text->setCaption(prop);
	//text->setFontHeight(h-3);
	text->setTextAlign(MyGUI::Align::Right);

	if (widget_for_type == 0)
	{
		editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
		if (_property != "RenderBox_Mesh" && _property != "Image_Texture") MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditTextChange = newDelegate (this, &EditorState::notifyApplyProperties);
		MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->eventEditSelectAccept = newDelegate (this, &EditorState::notifyApplyProperties);
	}
	else if (widget_for_type == 1)
	{
		editOrCombo = _window->createWidget<MyGUI::ComboBox>("ComboBox", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
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
	//editOrCombo->setFontHeight(h-2);

	// trim "ALIGN_"
	/*if (0 == strncmp("ALIGN_", _value.c_str(), 6))
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
	}*/

	if (_value.empty()) editOrCombo->setCaption(DEFAULT_VALUE);
	else MyGUI::castWidget<MyGUI::Edit>(editOrCombo)->setOnlyText(_value);
	propertiesText.push_back(text);
	propertiesElement.push_back(editOrCombo);
}

void EditorState::notifyApplyProperties(MyGUI::WidgetPtr _sender)
{
	ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES));
	WidgetContainer * widgetContainer = ew->find(current_widget);
	std::string action = _sender->getUserString("action");
	std::string value = MyGUI::castWidget<MyGUI::Edit>(_sender)->getOnlyText();
	std::string type = _sender->getUserString("type");

	if (value == "[DEFAULT]") value = "";
	/*else if ((action == "Align") || (action == "Widget_AlignText") || (action == "Progress_StartPoint"))
	{
		std::string tmp = "";
		const std::vector<std::string> & vec = MyGUI::utility::split(value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			if (!tmp.empty()) tmp += " ";
			tmp += "ALIGN_" + vec[pos];
		}
		value = tmp;
	}*/

	if (action == "Name")
	{
		if ((!value.empty()) && (null != ew->find(value)) && (widgetContainer != ew->find(value)))
		{
			MyGUI::Message::_createMessage(localise("Warning"), "Widget with name '" + value + "' already exist.", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
			MyGUI::castWidget<MyGUI::Edit>(_sender)->setCaption(widgetContainer->name);
			return;
		}
		widgetContainer->name = value;

		bool print_name = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowName")->getButtonPressed();
		bool print_type = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowType")->getButtonPressed();
		bool print_skin = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Button>("LayoutEditor_checkShowSkin")->getButtonPressed();
		allWidgetsCombo->setCaption(getDescriptionString(current_widget, print_name, print_type, print_skin));

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
			recreate = true;
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
			current_widget_rectangle->setPosition(convertParentCoordToCoord(coord, current_widget));
			return;
		}
		MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, "Widget_Move", value);
		current_widget_rectangle->setPosition(convertParentCoordToCoord(current_widget->getCoord(), current_widget));
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
		current_widget_rectangle->setPosition(convertParentCoordToCoord(current_widget->getCoord(), current_widget));
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

void EditorState::notifyApplyPropertiesCombo(MyGUI::WidgetPtr _sender)
{
	notifyApplyProperties(_sender);
}

void EditorState::notifyToggleRelativeMode(MyGUI::WidgetPtr _sender)
{
	if (current_widget){
		WidgetContainer * widgetContainer = ew->find(current_widget);
		MyGUI::WidgetPtr buttonRelativePosition = mGUI->findWidget<MyGUI::Widget>("LayoutEditor_buttonRelativePosition");
		if (widgetContainer->relative_mode) buttonRelativePosition->setCaption(localise("to_percents"));
		else buttonRelativePosition->setCaption(localise("to_pixels"));
		widgetContainer->relative_mode = !widgetContainer->relative_mode;
		propertiesElement[1]->setCaption(widgetContainer->position());
	}
}

std::string EditorState::getDescriptionString(MyGUI::WidgetPtr _widget, bool _print_name, bool _print_type, bool _print_skin)
{
	std::string name = "";
	std::string type = "";
	std::string skin = "";

	WidgetContainer * widgetContainer = ew->find(_widget);
	if (_print_name)
	{
		if (widgetContainer->name.empty()){
			// trim "LayoutEditorWidget_"
			name = _widget->getName();
			if (0 == strncmp("LayoutEditorWidget_", name.c_str(), 19))
			{
					std::string::iterator iter = std::find(name.begin(), name.end(), '_');
					if (iter != name.end()) name.erase(name.begin(), ++iter);
			}
			name = "[#333333" + name + "#000000] ";
		}
		else name = widgetContainer->name + " ";
	}

	if (_print_type)
	{
		type = "#0000FF" + _widget->getWidgetType() + " ";
	}

	if (_print_skin)
	{
		skin = "#FFFFFF" + widgetContainer->skin;
	}
	return name + type + skin;
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
		if (!MyGUI::InputManager::getInstance().isShiftPressed() && !arrow_move){
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
		propertiesElement[1]->setCaption(widgetContainer->position());
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
	while (null != parent){
		coord = coord - parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		parent = parent->getParent();
	}
	return coord;
}

MyGUI::IntCoord EditorState::convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	while (null != parent){
		coord = coord + parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		parent = parent->getParent();
	}
	return coord;
}

void EditorState::notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender)
{
	if (current_widget->getWidgetType() == "Tab")
	{
		addSheetToTab(current_widget);
		// update strings panel
		syncItems(false);
		um->addValue();
	}
}

void EditorState::notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
	MyGUI::IntPoint delta;
	int k = MyGUI::InputManager::getInstance().isShiftPressed() ? 1 : grid_step;
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
		um->addValue(PR_KEY_POSITION);
	}
}

void EditorState::addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption)
{
	MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(_tab);
	MyGUI::SheetPtr sheet = tab->addSheet(_caption);
	WidgetContainer * wc = new WidgetContainer("Sheet", "Default", sheet, "");
	if (!_caption.empty()) wc->mProperty.push_back(std::make_pair("Widget_Caption", _caption));
	ew->add(wc);
}

void EditorState::notifyMinimizePanel(MyGUI::WidgetPtr _sender)
{
	MyGUI::WidgetPtr panel = _sender->getParent();
	int dy;
	std::string resizing = panel->getUserString("resizing");
	if ((resizing == "") || (resizing == "down"))
	{// close
		dy = PANELS_MIN_HEIGHT - panel->getHeight();
		panel->setUserString("resizing", "up");
	}
	else
	{// open
		dy = atoi(panel->getUserString("Size").c_str()) - panel->getHeight();
		panel->setUserString("resizing", "down");
	}
	MyGUI::castWidget<MyGUI::Button>(_sender)->setButtonPressed(!MyGUI::castWidget<MyGUI::Button>(_sender)->getButtonPressed());

	MyGUI::VectorWidgetPtr::iterator panelIter = std::find(panels.begin(), panels.end(), panel);

	MyGUI::IntCoord newcord = (*panelIter)->getCoord() + MyGUI::IntSize(0, dy);
	MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(newcord, POSITION_CONTROLLER_TIME, move_mode);
	MyGUI::ControllerManager::getInstance().addItem((*panelIter), controller);

	for (++panelIter; panelIter != panels.end(); ++panelIter)
	{
		MyGUI::IntCoord newcord = (*panelIter)->getCoord() + MyGUI::IntPoint(0, dy);
		MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(newcord, POSITION_CONTROLLER_TIME, move_mode);
		MyGUI::ControllerManager::getInstance().addItem((*panelIter), controller);
	}

	MyGUI::WindowPtr windowProperties = MyGUI::castWidget<MyGUI::Window>(panel->getParent()->getParent());
	MyGUI::IntRect newMinMax = windowProperties->getMinMax() + MyGUI::IntRect(0, (dy<0)?dy:0, 0, (dy>0)?dy:0);
	windowProperties->setUserString("resizing", (dy>0)?"down":"up");
	windowProperties->setMinMax(newMinMax);

	newcord = windowProperties->getCoord() + MyGUI::IntSize(0, dy);
	controller = new MyGUI::ControllerPosition(newcord, POSITION_CONTROLLER_TIME, move_mode);
	controller->eventPostAction = MyGUI::newDelegate(this, &EditorState::notifyEndResize);
	MyGUI::ControllerManager::getInstance().addItem(windowProperties, controller);

	// сделать все кнопки временно недоступными
	for (MyGUI::VectorWidgetPtr::iterator iter = panelMinimizeButtons.begin(); iter != panelMinimizeButtons.end(); ++iter)
		(*iter)->setEnabledSilent(false);
}

void EditorState::notifyEndResize(MyGUI::WidgetPtr _sender)
{
	MyGUI::WindowPtr windowProperties = MyGUI::castWidget<MyGUI::Window>(_sender);
	std::string resizing = windowProperties->getUserString("resizing");
	MyGUI::IntRect newMinMax;
	if ((resizing == "") || (resizing == "down"))
	{// close
		newMinMax = windowProperties->getMinMax();
		newMinMax.top = newMinMax.bottom;
		windowProperties->setUserString("resizing", "up");
	}
	else
	{// open
		newMinMax = windowProperties->getMinMax();
		newMinMax.bottom = newMinMax.top;
		windowProperties->setUserString("resizing", "down");
	}
	windowProperties->setMinMax(newMinMax);

	for (MyGUI::VectorWidgetPtr::iterator iter = panelMinimizeButtons.begin(); iter != panelMinimizeButtons.end(); ++iter)
		(*iter)->setEnabled(true);
}

void EditorState::syncItems(bool _apply, bool _add, Ogre::String _value)
{
	Ogre::String action;
	// FIXME/2 как-то громоздко и не настраиваемо...
	if (current_widget->getWidgetType() == "Tab") action = "Tab_AddSheet";
	else
	{
		// for example "ComboBox_AddItem", "List_AddItem", etc...
		action = current_widget->getWidgetType() + "_AddItem";
	}

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
							else if (current_widget->getWidgetType() == "MenuBar") MyGUI::castWidget<MyGUI::MenuBar>(current_widget)->deleteItem(index);
							//else if (current_widget->getWidgetType() == "Message") MyGUI::castWidget<MyGUI::Message>(current_widget)->deleteItem(index);
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
		MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
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

void EditorState::notifyAddItem(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editItem");
	syncItems(true, true, edit->getOnlyText());
	list->addItem(edit->getOnlyText());
	um->addValue();
}

void EditorState::notifyDeleteItem(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	size_t item = list->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;
	syncItems(true, false, list->getItem(item));
	list->deleteItem(item);
	um->addValue();
}

void EditorState::notifySelectSheet(MyGUI::WidgetPtr _sender)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	size_t item = list->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;
	ON_EXIT(UndoManager::getInstance().addValue());
	MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(current_widget);
	WidgetContainer * widgetContainer = ew->find(current_widget);

	Ogre::String action = "Tab_SelectSheet";
	Ogre::String value = MyGUI::utility::toString(item);
	MyGUI::WidgetManager::getInstance().parse(widgetContainer->widget, action, value);

	action = "Sheet_Select";
	for (size_t i = 0; i < tab->getSheetCount(); ++i)
	{
		WidgetContainer * sheetContainer = ew->find(tab->getSheet(i));

		if (i == item) MapSet(sheetContainer->mProperty, action, "true");
		else MapErase(sheetContainer->mProperty, action);
	}
}

void EditorState::notifyUpdateItem(MyGUI::WidgetPtr _widget)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editItem");
	size_t item = list->getItemSelect();
	if (MyGUI::ITEM_NONE == item){ notifyAddItem(); return;}
	ON_EXIT(UndoManager::getInstance().addValue());
	Ogre::String action;
	Ogre::String value = edit->getOnlyText();
	Ogre::String lastitem = list->getItem(item);
	list->setItem(item, value);

	if (current_widget->getWidgetType() == "Tab")
	{
		action = "Widget_Caption";
		MyGUI::TabPtr tab = MyGUI::castWidget<MyGUI::Tab>(current_widget);
		MyGUI::SheetPtr sheet = tab->getSheet(item);
		WidgetContainer * widgetContainer = ew->find(sheet);
		MyGUI::WidgetManager::getInstance().parse(sheet, "Widget_Caption", value);
		MapSet(widgetContainer->mProperty, action, value);
		return;
	}
	else
	{
		action = current_widget->getWidgetType() + "_AddItem";
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
				else if (current_widget->getWidgetType() == "MenuBar") MyGUI::castWidget<MyGUI::MenuBar>(current_widget)->setItem(index, value);
				return;
			}
			++index;
		}
	}
}

void EditorState::notifySelectItem(MyGUI::WidgetPtr _widget, size_t _position)
{
	MyGUI::ListPtr list = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::List>("LayoutEditor_listItems");
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editItem");
	size_t item = list->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;
	Ogre::String value = list->getItem(item);
	edit->setOnlyText(value);
}

void EditorState::notifyAddUserData(MyGUI::WidgetPtr _sender)
{
	MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
	MyGUI::EditPtr editKey = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editKeyUserData");
	MyGUI::EditPtr editValue = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editValueUserData");
	Ogre::String key = editKey->getOnlyText();
	Ogre::String value = editValue->getOnlyText();
	WidgetContainer * widgetContainer = ew->find(current_widget);
	if (MapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
	{
		multilist->addItem(key);
	}
	multilist->setSubItem(1, multilist->findItem(0, key), value);
	MapSet(widgetContainer->mUserString, key, value);
	um->addValue();
}

void EditorState::notifyDeleteUserData(MyGUI::WidgetPtr _sender)
{
	MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
	size_t item = multilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;

	WidgetContainer * widgetContainer = ew->find(current_widget);
	MapErase(widgetContainer->mUserString, multilist->getItem(item));
	multilist->deleteItem(item);
	um->addValue();
}

void EditorState::notifyUpdateUserData(MyGUI::WidgetPtr _widget)
{
	MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
	MyGUI::EditPtr editKey = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editKeyUserData");
	MyGUI::EditPtr editValue = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editValueUserData");
	size_t item = multilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item){ notifyAddUserData(); return;}
	Ogre::String key = editKey->getOnlyText();
	Ogre::String value = editValue->getOnlyText();
	Ogre::String lastkey = multilist->getItem(item);

	WidgetContainer * widgetContainer = ew->find(current_widget);
	multilist->deleteItem(multilist->findItem(0, lastkey));
	MapErase(widgetContainer->mUserString, lastkey);
	if (MapFind(widgetContainer->mUserString, key) == widgetContainer->mUserString.end())
	{
		multilist->addItem(key);
	}
	multilist->setSubItem(1, multilist->findItem(0, key), value);
	multilist->setItemSelect(multilist->findItem(0, key));
	MapSet(widgetContainer->mUserString, key, value);
	um->addValue();
}

void EditorState::notifySelectUserDataItem(MyGUI::WidgetPtr _widget, size_t _index)
{
	MyGUI::MultiListPtr multilist = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::MultiList>("LayoutEditor_multilistUserData");
	MyGUI::EditPtr editKey = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editKeyUserData");
	MyGUI::EditPtr editValue = MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("LayoutEditor_editValueUserData");
	size_t item = multilist->getItemSelect();
	if (MyGUI::ITEM_NONE == item) return;
	Ogre::String key = multilist->getSubItem(0, item);
	Ogre::String value = multilist->getSubItem(1, item);
	editKey->setOnlyText(key);
	editValue->setOnlyText(value);
}

void EditorState::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	if (_info.type == MyGUI::TOOLTIP_SHOW) {
		mToolTip.show(_sender->getUserString("TooTipText"), _info.point);
	}
	else if (_info.type == MyGUI::TOOLTIP_HIDE) {
		mToolTip.hide();
	}
}

Ogre::DisplayString EditorState::anci_to_utf16(const std::string & _source)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	const char* srcPtr = _source.c_str();
	int tmpSize = MultiByteToWideChar( CP_ACP, 0, srcPtr, -1, 0, 0 );
	WCHAR* tmpBuff = new WCHAR [ tmpSize + 1 ];
	MultiByteToWideChar( CP_ACP, 0, srcPtr, -1, tmpBuff, tmpSize );
	std::wstring ret = tmpBuff;
	delete[] tmpBuff;
	return ret;
#else
	return Ogre::DisplayString(_source).asWStr();
#endif
}

std::string EditorState::utf16_to_anci(const Ogre::DisplayString & _source)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	const wchar_t* srcPtr = _source.asWStr_c_str(); 
	int dstSize = WideCharToMultiByte( CP_ACP, 0, srcPtr, (int)_source.size(), 0, 0, 0, 0 ); 
	char * dest = new char [ dstSize + 1 ];
	WideCharToMultiByte( CP_ACP, 0, srcPtr, (int)_source.size(), dest, dstSize, 0, 0 ); 
	dest[dstSize] = 0;
	std::string ret = dest;
	delete [] dest;
	return ret;
#else
	return _source.asUTF8();
#endif
}
