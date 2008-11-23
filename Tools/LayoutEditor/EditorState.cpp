#include "precompiled.h"
#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

const std::string LogSection = "LayoutEditor";

const std::string settingsFile = "settings.xml";
const std::string userSettingsFile = "le_user_settings.xml";

const float POSITION_CONTROLLER_TIME = 0.5f;
const int HIDE_REMAIN_PIXELS = 3;

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

enum POPUP_MENU_MAIN
{
	ITEM_LOAD,
	ITEM_SAVE,
	ITEM_SAVE_AS,
	ITEM_CLEAR,
	ITEM_SEPARATOR1,
	ITEM_SETTINGS,
	ITEM_TEST,
	ITEM_SEPARATOR2,
	ITEM_QUIT
};

void eventInfo(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _event)
{
	MyGUI::MYGUI_OUT("eventInfo: ", _event);
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

	MyGUI::ResourceManager::getInstance().load("initialise.xml");

	// set locale language if it was taken from OS
	if (! BasisManager::getInstance().getLanguage().empty() )
		MyGUI::LanguageManager::getInstance().setCurrentLanguage(BasisManager::getInstance().getLanguage());
	// if you want to test LanguageManager uncomment next line
	//MyGUI::LanguageManager::getInstance().setCurrentLanguage("Russian");

	mToolTip.initialise();
	MyGUI::DelegateManager::getInstance().addDelegate("eventInfo", MyGUI::newDelegate(eventInfo));
	//MyGUI::DelegateManager::getInstance().addDelegate("eventEditorToolTip", MyGUI::newDelegate(this, &EditorState::notifyToolTip));

	interfaceWidgets = MyGUI::LayoutManager::getInstance().loadLayout("interface.layout", "LayoutEditor_");

	// settings window
	mSettingsWindow.initialise();
	mSettingsWindow.eventWidgetsUpdate = MyGUI::newDelegate(this, &EditorState::notifyWidgetsUpdate);
	interfaceWidgets.push_back(mSettingsWindow.mainWidget());

	loadSettings(settingsFile);
	loadSettings(userSettingsFile);

	// создание меню
	createMainMenu();

	// properties panelView
	mPropertiesPanelView.initialise();
	mPropertiesPanelView.eventRecreate = MyGUI::newDelegate(this, &EditorState::notifyRecreate);
	mPropertiesPanelView->setCoord(mGUI->getViewWidth() - mPropertiesPanelView->getSize().width, bar->getHeight(),
					mPropertiesPanelView->getSize().width, mGUI->getViewHeight() - bar->getHeight());
	interfaceWidgets.push_back(mPropertiesPanelView.mainWidget());

	mWidgetsWindow.initialise();
	mWidgetsWindow.eventToolTip = MyGUI::newDelegate(this, &EditorState::notifyToolTip);
	mWidgetsWindow.eventSelectWidget = MyGUI::newDelegate(this, &EditorState::notifySelectWidget);
	interfaceWidgets.push_back(mWidgetsWindow.mainWidget());

	if (mSettingsWindow.getEdgeHide())
	{
		for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
		{
			MyGUI::ControllerEdgeHide * controller = new MyGUI::ControllerEdgeHide(POSITION_CONTROLLER_TIME, HIDE_REMAIN_PIXELS, 3);
			MyGUI::ControllerManager::getInstance().addItem(*iter, controller);
		}
	}

	clear();

	/*MyGUI::WidgetPtr mFpsInfo = mGUI->createWidget<MyGUI::Widget>("ButtonSmall", 20, (int)mGUI->getViewHeight() - 80, 120, 70, MyGUI::Align::Left | MyGUI::Align::Bottom, "Main", "fpsInfo");
	mFpsInfo->setColour(Ogre::ColourValue::White);*/

	typedef std::vector<std::string> Params;
	Params params = BasisManager::getInstance().getCommandParams();
	for (Params::iterator iter=params.begin(); iter!=params.end(); ++iter) {
		load(iter->c_str());
	}
}

void EditorState::createMainMenu()
{
	bar = mGUI->createWidget<MyGUI::MenuBar>("MenuBar", MyGUI::IntCoord(0, 0, mGUI->getViewWidth(), 28), MyGUI::Align::Top | MyGUI::Align::HStretch, "Overlapped", "LayoutEditor_MenuBar");
	bar->addItem(localise("File"));
	bar->addItem(localise("Widgets"));
	// FIXME менюбар сунуть в лейаут
	interfaceWidgets.push_back(bar);

	mPopupMenuWidgets = bar->getItemChildAt(1);

	mPopupMenuFile = bar->getItemChildAt(0);
	mPopupMenuFile->addItem(localise("Load"), MyGUI::MenuItemType::Normal, "File/Load");
	mPopupMenuFile->addItem(localise("Save"), MyGUI::MenuItemType::Normal, "File/Save");
	mPopupMenuFile->addItem(localise("Save_as"), MyGUI::MenuItemType::Normal, "File/SaveAs");
	mPopupMenuFile->addItem(localise("Clear"), MyGUI::MenuItemType::Normal, "File/Clear");
	mPopupMenuFile->addItem("", MyGUI::MenuItemType::Separator);
	mPopupMenuFile->addItem(localise("Settings"), MyGUI::MenuItemType::Normal, "File/Settings");
	mPopupMenuFile->addItem(localise("Test"), MyGUI::MenuItemType::Normal, "File/Test");
	mPopupMenuFile->addItem("", MyGUI::MenuItemType::Separator);

	// список последних открытых файлов
	if (recentFiles.size()) {
		for (std::vector<Ogre::String>::reverse_iterator iter = recentFiles.rbegin(); iter != recentFiles.rend(); ++iter) {
			//MyGUI::PopupMenu::ItemType type = MyGUI::PopupMenu::ItemTypeNormal;
			//if (recentFiles.rend() - iter == 1 ) type = MyGUI::PopupMenu::ItemTypeSeparator;
			// id одинаковый, им€ файла в юзер дату
			mPopupMenuFile->addItem(*iter, MyGUI::MenuItemType::Normal, "File/RecentFiles",  *iter);
		}

		// если есть файлы, то еще один сепаратор
		mPopupMenuFile->addItem("", MyGUI::MenuItemType::Separator);
	}
	
	mPopupMenuFile->addItem(localise("Quit"), MyGUI::MenuItemType::Normal, "File/Quit");

	bar->eventPopupMenuAccept = newDelegate(this, &EditorState::notifyPopupMenuAccept);
}

void EditorState::notifyPopupMenuAccept(MyGUI::WidgetPtr _sender, MyGUI::PopupMenuPtr _menu, size_t _index)
{
	if (mPopupMenuFile == _menu) {

		MyGUI::MenuItemPtr item = _menu->getItemAt(_index);
		std::string id = item->getItemId();

		if (id == "File/Load") {
			notifyLoadSaveAs(false);
		}
		else if (id == "File/Save") {
			notifySave();
		}
		else if (id == "File/SaveAs") {
			notifyLoadSaveAs(true);
		}
		else if (id == "File/Clear") {
			notifyClear();
		}
		else if (id == "File/Settings") {
			notifySettings();
		}
		else if (id == "File/Test") {
			notifyTest();
		}
		else if (id == "File/RecentFiles") {
			load(*item->getItemData<std::string>());
		}
		else if (id == "File/Quit") {
			notifyQuit();
		}

		/*if (_index < ITEM_TEST+1)
		{
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
				case ITEM_CLEAR:
					notifyClear();
					break;
				case ITEM_SETTINGS:
					notifySettings();
					break;
				case ITEM_TEST:
					notifyTest();
					break;
			}
		}
		else if (_index == _menu->getItemCount()-1)
		{
			notifyQuit();
		}
		else
		{
			load(_menu->getItemNameAt(_index));
		}*/
	}
}

void EditorState::exit()
{
	saveSettings(userSettingsFile);
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

	// drop select depth if we moved mouse
	const int DIST = 2;
	if ((abs(x - arg.state.X.abs) > DIST) || (abs(y - arg.state.Y.abs) > DIST))
	{
		selectDepth = 0;
		x = arg.state.X.abs;
		y = arg.state.Y.abs;
	}

	// align to grid if shift not pressed
	int x2, y2;
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x2 = toGrid(arg.state.X.abs);
		y2 = toGrid(arg.state.Y.abs);
	}
	else
	{
		x2 = arg.state.X.abs;
		y2 = arg.state.Y.abs;
	}

	mWidgetsWindow.createNewWidget(x2, y2);

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
	int x1, y1;
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x1 = toGrid(arg.state.X.abs);
		y1 = toGrid(arg.state.Y.abs);
	}
	else
	{
		x1 = arg.state.X.abs;
		y1 = arg.state.Y.abs;
	}

	// юбилейный комит  =)
	mWidgetsWindow.startNewWidget(x1, y1, id);

	// это чтобы можно было двигать пр€моугольник у невидимых виджето (или виджетов за границами)
	//MyGUI::LayerItemInfoPtr rootItem = null;
	//MyGUI::WidgetPtr itemWithRect = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	// не стал это доделывать, т.к. неоднозначность выбора виджета получаетс€, если кто скажет как выбирать - сделаю

	MyGUI::WidgetPtr item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(arg.state.X.abs, arg.state.Y.abs);

	// не убираем пр€моугольник если нажали на его раст€гивалку
	if (item && (item->getParent() != mPropertiesPanelView.getWidgetRectangle()))
	{
		// чтобы пр€моугольник не мешалс€
		mPropertiesPanelView.getWidgetRectangle()->hide();
		item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(arg.state.X.abs, arg.state.Y.abs);
	}

	if (null != item)
	{
		// find widget registered as container
		while ((null == ew->find(item)) && (null != item)) item = item->getParent();
		MyGUI::WidgetPtr oldItem = item;

		// try to selectin depth
		int depth = selectDepth;
		while (depth && (null != item))
		{
			item = item->getParent();
			while ((null == ew->find(item)) && (null != item)) item = item->getParent();
			depth--;
		}
		if (null == item)
		{
			item = oldItem;
			selectDepth = 0;
		}

		// found widget
		if (null != item)
		{
			MyGUI::IntSize size = item->getTextSize();
			notifySelectWidget(item);
			if (mWidgetsWindow.getCreatingStatus() != 1){
				mGUI->injectMouseMove(arg);// это чтобы сразу можно было тащить
			}
		}
		mGUI->injectMousePress(arg, id);
	}
	else {
		mGUI->injectMousePress(arg, id);
		notifySelectWidget(null);
	}

	// вернем пр€моугольник
	if (current_widget && mWidgetsWindow.getCreatingStatus() == 0)
		mPropertiesPanelView.getWidgetRectangle()->show();
	else if (mWidgetsWindow.getCreatingStatus())
		mPropertiesPanelView.getWidgetRectangle()->hide();

	return true;
}
//===================================================================================
bool EditorState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	selectDepth++;
	if (testMode){ mGUI->injectMouseRelease(arg, id); return true;}

	if (MyGUI::InputManager::getInstance().isModalAny())
	{
	}
	else
	{
		// align to grid if shift not pressed
		int x2, y2;
		if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
		{
			x2 = toGrid(arg.state.X.abs);
			y2 = toGrid(arg.state.Y.abs);
		}
		else
		{
			x2 = arg.state.X.abs;
			y2 = arg.state.Y.abs;
		}

		mWidgetsWindow.finishNewWidget(x2, y2);
	}

	um->dropLastProperty();
	mGUI->injectMouseRelease(arg, id);

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
				if (mSettingsWindow.getEdgeHide())
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
				clear(false);
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
				mPropertiesPanelView.toggleRelativeMode();
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

	if (recreate)
	{
		recreate = false;
		notifySelectWidget(null); // виджет пересоздалс€, теперь никто незнает его адреса :)
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
void EditorState::loadSettings(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
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
			// берем детей и крутимс€
			MyGUI::xml::xmlNodeIterator field = root->getNodeIterator();
			while (field.nextNode()) {
				if (field->getName() == "PropertiesPanelView") mPropertiesPanelView.load(field);
				else if (field->getName() == "SettingsWindow") mSettingsWindow.load(field);
				else if (field->getName() == "WidgetsWindow") mWidgetsWindow.load(field);
				else if (field->getName() == "RecentFile") 
				{
					Ogre::String name;
					if (false == field->findAttribute("name", name)) continue;
					recentFiles.push_back(name);
				}
			}
		}
	}
}

void EditorState::saveSettings(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
	}

	doc.createInfo();
	MyGUI::xml::xmlNodePtr root = doc.createRoot("MyGUI");
	root->addAttributes("type", "Settings");

	mPropertiesPanelView.save(root);
	mSettingsWindow.save(root);
	mWidgetsWindow.save(root);

	// cleanup for duplicates
	
	std::reverse(recentFiles.begin(), recentFiles.end());
	for (size_t i = 0; i < recentFiles.size(); ++i)
		recentFiles.erase(std::remove(recentFiles.begin() + i + 1, recentFiles.end(), recentFiles[i]), recentFiles.end());
	
	// remove old files
	while (recentFiles.size() > MAX_RECENT_FILES)
		recentFiles.pop_back();
	std::reverse(recentFiles.begin(), recentFiles.end());

	for (std::vector<Ogre::String>::iterator iter = recentFiles.begin(); iter != recentFiles.end(); ++iter)
	{
		MyGUI::xml::xmlNodePtr nodeProp = root->createChild("RecentFile");
		nodeProp->addAttributes("name", *iter);
	}

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
			MyGUI::Message::_createMessage(localise("Warning"), "Failed to save file '" + fileName + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
		}
		else
		{
			recentFiles.push_back(fileName);
		}
	}
	else notifyLoadSaveAs(true);
}

void EditorState::notifyLoadSaveAs(bool _save)
{
	// create message box with file name and two buttons
	MyGUI::WidgetPtr messageWindow = MyGUI::LayoutManager::getInstance().load("SaveLoadMessage.layout")[0];
	MyGUI::IntSize view(mGUI->getViewSize());
	MyGUI::IntSize size(messageWindow->getSize());
	messageWindow->setCoord((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);
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
	MyGUI::ComboBoxPtr combo = combo2->castType<MyGUI::ComboBox>();
	if (fileName != "") {
		combo->setCaption(fileName);
	}
	combo->eventEditSelectAccept = newDelegate(this, &EditorState::notifyLoadSaveEditAccept);
	std::vector<Ogre::String> strs = MyGUI::helper::getVectorResourcePath("*.layout");
	for (std::vector<Ogre::String>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		combo->addItem(*iter);
	}

}

void EditorState::notifySettings()
{
	mSettingsWindow->show();
	MyGUI::LayerManager::getInstance().upLayerItem(mSettingsWindow.mainWidget());
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

void EditorState::clear(bool _clearName)
{
	mWidgetsWindow.clearNewWidget();
	recreate = false;
	if (_clearName) fileName = "";
	testMode = false;
	ew->clear();
	notifySelectWidget(null);
	um->addValue();
	selectDepth = 0;

	if (_clearName) BasisManager::getInstance().setWindowCaption("MyGUI Layout Editor");
}

void EditorState::notifyQuit()
{
	MyGUI::Message::_createMessage(localise("Warning"), localise("Warn_exit"), "", "Overlapped", true, newDelegate(this, &EditorState::notifyQuitMessage), MyGUI::Message::IconWarning | MyGUI::Message::Yes | MyGUI::Message::No);
}

void EditorState::notifyQuitMessage(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)
{
	if (_button == MyGUI::Message::Yes || _button == MyGUI::Message::Button1) {
		BasisManager::getInstance().eventExit();
	}
}

void EditorState::notifyLoadSaveAccept(MyGUI::WidgetPtr _sender)
{
	bool success;
	Ogre::UTFString file_name = mGUI->findWidget<MyGUI::Edit>("LayoutEditor_editFileName")->getCaption();

	if (_sender->getCaption() == "Load") success = ew->load(file_name);
	else/*(_sender->getCaption() == "Save")*/ success = ew->save(file_name);

	if (false == success)
	{
		MyGUI::Message::_createMessage(localise("Warning"), "Failed to " + _sender->getCaption() + " file '" + file_name + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
	}
	else
	{
		// запоминает последнее удачное им€ файла
		fileName = file_name;
		BasisManager::getInstance().setWindowCaption(fileName + " - MyGUI Layout Editor");
		notifyLoadSaveCancel(_sender);
		um->addValue();
		recentFiles.push_back(fileName);
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
		MyGUI::Message::_createMessage(localise("Warning"), "Failed to load file '" + fileName + "'", "", "Overlapped", true, null, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
		return;
	}

	BasisManager::getInstance().setWindowCaption(_file + " - MyGUI Layout Editor");

	fileName = _file;
	um->addValue();
	recentFiles.push_back(fileName);
}

void EditorState::notifyWidgetsUpdate()
{
	bool print_name = mSettingsWindow.getShowName();
	bool print_type = mSettingsWindow.getShowType();
	bool print_skin = mSettingsWindow.getShowSkin();

	mPopupMenuWidgets->removeAllItems();
	mPopupMenuWidgets->eventPopupMenuAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);

	for (std::vector<WidgetContainer*>::iterator iter = ew->widgets.begin(); iter != ew->widgets.end(); ++iter )
	{
		createWidgetPopup(*iter, mPopupMenuWidgets, print_name, print_type, print_skin);
	}
}

void EditorState::createWidgetPopup(WidgetContainer* _container, MyGUI::PopupMenuPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
{
	bool submenu = !_container->childContainers.empty();

	_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
	_parentPopup->setItemDataAt(_parentPopup->getItemCount()-1, _container->widget);

	if (submenu) {
		MyGUI::PopupMenuPtr child = _parentPopup->getItemChildAt(_parentPopup->getItemCount()-1);
		child->eventPopupMenuAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);

		for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
		{
			createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
		}
	}
}

void EditorState::notifyWidgetsSelect(MyGUI::WidgetPtr _widget, size_t _index)
{
	MyGUI::WidgetPtr widget = *_widget->castType<MyGUI::PopupMenu>()->getItemDataAt<MyGUI::WidgetPtr>(_index);
	//MyGUI::WidgetPtr widget = *_widget->castType<MyGUI::PopupMenu>()->getItemInfoAt(_index).data.castType<MyGUI::WidgetPtr>();
	notifySelectWidget(widget);
}

void EditorState::notifySelectWidget(MyGUI::WidgetPtr _sender)
{
	if (_sender == current_widget)
	{
		if (current_widget)
		{
			mPropertiesPanelView.getWidgetRectangle()->show();
			MyGUI::InputManager::getInstance().setKeyFocusWidget(mPropertiesPanelView.getWidgetRectangle());
		}
		return;
	}

	current_widget = _sender;

	mPropertiesPanelView.update(_sender);
	mWidgetsWindow.update(_sender);
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
		type = "#0000FF" + _widget->getTypeName() + " ";
	}

	if (_print_skin)
	{
		skin = "#FFFFFF" + widgetContainer->skin;
	}
	return name + type + skin;
}

void EditorState::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	if (_info.type == MyGUI::ToolTipInfo::Show) {
		mToolTip.show(_sender, _info.point);
	}
	else if (_info.type == MyGUI::ToolTipInfo::Hide) {
		mToolTip.hide();
	}
}

