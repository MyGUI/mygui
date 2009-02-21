#include "precompiled.h"
#include "BasisManager.h"
#include "EditorState.h"
#include "EditorWidgets.h"
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

void eventInfo(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _event)
{
	MyGUI::MYGUI_OUT("eventInfo: ", _event);
}

//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	mGUI = MyGUI::Gui::getInstancePtr();

	MyGUI::LogManager::registerSection(LogSection, "MyGUI.log");

	// set locale language if it was taken from OS
	if (! BasisManager::getInstance().getLanguage().empty() )
		MyGUI::LanguageManager::getInstance().setCurrentLanguage(BasisManager::getInstance().getLanguage());
	// if you want to test LanguageManager uncomment next line
	//MyGUI::LanguageManager::getInstance().setCurrentLanguage("Russian");

	wt = new WidgetTypes();
	wt->initialise();
	ew = new EditorWidgets();
	ew->initialise();
	um = new UndoManager();
	um->initialise(ew);

	MyGUI::ResourceManager::getInstance().load("initialise.xml");

	mToolTip = new EditorToolTip();
	MyGUI::DelegateManager::getInstance().addDelegate("eventInfo", MyGUI::newDelegate(eventInfo));
	//MyGUI::DelegateManager::getInstance().addDelegate("eventEditorToolTip", MyGUI::newDelegate(this, &EditorState::notifyToolTip));

	interfaceWidgets = MyGUI::LayoutManager::getInstance().loadLayout("interface.layout", "LayoutEditor_");

	// settings window
	mSettingsWindow = new SettingsWindow();
	mSettingsWindow->eventWidgetsUpdate = MyGUI::newDelegate(this, &EditorState::notifyWidgetsUpdate);
	interfaceWidgets.push_back(mSettingsWindow->getMainWidget());

	// properties panelView
	mPropertiesPanelView = new PropertiesPanelView();
	mPropertiesPanelView->eventRecreate = MyGUI::newDelegate(this, &EditorState::notifyRecreate);
	interfaceWidgets.push_back(mPropertiesPanelView->getMainWidget());

	mWidgetsWindow = new WidgetsWindow();
	mWidgetsWindow->eventToolTip = MyGUI::newDelegate(this, &EditorState::notifyToolTip);
	mWidgetsWindow->eventSelectWidget = MyGUI::newDelegate(this, &EditorState::notifySelectWidget);
	interfaceWidgets.push_back(mWidgetsWindow->getMainWidget());

	mSaveLoadWindow = new SaveLoadWindow();
	mSaveLoadWindow->eventLoadFile = MyGUI::newDelegate(this, &EditorState::saveOrLoadLayout<false>);
	mSaveLoadWindow->eventSaveFile = MyGUI::newDelegate(this, &EditorState::saveOrLoadLayout<true>);

	loadSettings(settingsFile, true);
	loadSettings(userSettingsFile, false);

	// создание меню
	createMainMenu();

	mPropertiesPanelView->getMainWidget()->setCoord(
		mGUI->getViewWidth() - mPropertiesPanelView->getMainWidget()->getSize().width,
		bar->getHeight(),
		mPropertiesPanelView->getMainWidget()->getSize().width,
		mGUI->getViewHeight() - bar->getHeight()
		);

	// после загрузки настроек инициализируем
	mWidgetsWindow->initialise();

	if (mSettingsWindow->getEdgeHide())
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
		saveOrLoadLayout<false>(iter->c_str());
	}
}

void EditorState::exit()
{
	saveSettings(userSettingsFile, false);

	delete mPropertiesPanelView;
	mPropertiesPanelView = 0;

	um->shutdown();
	delete um;
	ew->shutdown();
	delete ew;
	wt->shutdown();
	delete wt;
}

void EditorState::createMainMenu()
{
	MyGUI::VectorWidgetPtr menu_items = MyGUI::LayoutManager::getInstance().load("interface_menu.layout");
	MYGUI_ASSERT(menu_items.size() == 1, "Error load main menu");
	bar = menu_items[0]->castType<MyGUI::MenuBar>();
	bar->setCoord(0, 0, mGUI->getViewWidth(), bar->getHeight());

	// главное меню
	MyGUI::MenuItemPtr menu_file = bar->getItemById("File");
	mPopupMenuFile = menu_file->getItemChild();
	// список последних открытых файлов
	if (recentFiles.size()) {
		MyGUI::MenuItemPtr menu_item = mPopupMenuFile->getItemById("File/Quit");
		for (std::vector<Ogre::String>::reverse_iterator iter = recentFiles.rbegin(); iter != recentFiles.rend(); ++iter) {
			mPopupMenuFile->insertItem(menu_item, *iter, MyGUI::MenuItemType::Normal, "File/RecentFiles",  *iter);
		}
		// если есть файлы, то еще один сепаратор
		mPopupMenuFile->insertItem(menu_item, "", MyGUI::MenuItemType::Separator);
	}

	//хак, для меню тест двойная замена
	MyGUI::MenuItemPtr menu_item_test = mPopupMenuFile->getItemById("File/Test");
	menu_item_test->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(menu_item_test->getCaption()));

	// меню для виджетов
	MyGUI::MenuItemPtr menu_widget = bar->getItemById("Widgets");
	mPopupMenuWidgets = menu_widget->createItemChild();
	//FIXME
	mPopupMenuWidgets->setPopupAccept(true);

	bar->eventMenuCtrlAccept = newDelegate(this, &EditorState::notifyPopupMenuAccept);

	interfaceWidgets.push_back(bar);
}

void EditorState::notifyPopupMenuAccept(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)
{
	if (mPopupMenuFile == _item->getMenuCtrlParent()) {

		std::string id = _item->getItemId();

		if (id == "File/Load") {
			mSaveLoadWindow->setLoad(fileName);
		}
		else if (id == "File/Save") {
			notifySave();
		}
		else if (id == "File/SaveAs") {
			mSaveLoadWindow->setSave(fileName);
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
			saveOrLoadLayout<false>(*_item->getItemData<std::string>());
		}
		else if (id == "File/Quit") {
			notifyQuit();
		}
	}
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

	mWidgetsWindow->createNewWidget(x2, y2);

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
	mWidgetsWindow->startNewWidget(x1, y1, id);

	// это чтобы можно было двигать прямоугольник у невидимых виджето (или виджетов за границами)
	//MyGUI::LayerItemInfoPtr rootItem = nullptr;
	//MyGUI::WidgetPtr itemWithRect = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));
	// не стал это доделывать, т.к. неоднозначность выбора виджета получается, если кто скажет как выбирать - сделаю

	MyGUI::WidgetPtr item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(arg.state.X.abs, arg.state.Y.abs);

	// не убираем прямоугольник если нажали на его растягивалку
	if (item && (item->getParent() != mPropertiesPanelView->getWidgetRectangle()))
	{
		// чтобы прямоугольник не мешался
		mPropertiesPanelView->getWidgetRectangle()->setVisible(false);
		item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(arg.state.X.abs, arg.state.Y.abs);
	}

	if (nullptr != item)
	{
		// find widget registered as container
		while ((nullptr == ew->find(item)) && (nullptr != item)) item = item->getParent();
		MyGUI::WidgetPtr oldItem = item;

		// try to selectin depth
		int depth = selectDepth;
		while (depth && (nullptr != item))
		{
			item = item->getParent();
			while ((nullptr == ew->find(item)) && (nullptr != item)) item = item->getParent();
			depth--;
		}
		if (nullptr == item)
		{
			item = oldItem;
			selectDepth = 0;
		}

		// found widget
		if (nullptr != item)
		{
			notifySelectWidget(item);
			if (mWidgetsWindow->getCreatingStatus() != 1){
				mGUI->injectMouseMove(arg);// это чтобы сразу можно было тащить
			}
		}
		mGUI->injectMousePress(arg, id);
	}
	else {
		mGUI->injectMousePress(arg, id);
		notifySelectWidget(nullptr);
	}

	// вернем прямоугольник
	if (current_widget && mWidgetsWindow->getCreatingStatus() == 0)
		mPropertiesPanelView->getWidgetRectangle()->setVisible(true);
	else if (mWidgetsWindow->getCreatingStatus())
		mPropertiesPanelView->getWidgetRectangle()->setVisible(false);

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

		mWidgetsWindow->finishNewWidget(x2, y2);
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
				if (mSettingsWindow->getEdgeHide())
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
				bar->setVisible(true);
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
		if (mSaveLoadWindow->isVisible())
		{
			if (arg.key == OIS::KC_ESCAPE) mSaveLoadWindow->notifyCancel();
			else if (arg.key == OIS::KC_RETURN) mSaveLoadWindow->notifyOk();
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
			if (arg.key == OIS::KC_O || arg.key == OIS::KC_L)
			{
				mSaveLoadWindow->setLoad(fileName);
			}
			else if (arg.key == OIS::KC_S) notifySave();
			else if (arg.key == OIS::KC_Z){
				um->undo();
				notifySelectWidget(nullptr);
			}
			else if ((arg.key == OIS::KC_Y) || ((input.isShiftPressed()) && (arg.key == OIS::KC_Z))){
				um->redo();
				notifySelectWidget(nullptr);
			}
			else if (arg.key == OIS::KC_T)
			{
				notifyTest();
				return true;
			}
			else if (arg.key == OIS::KC_R)
			{
				mPropertiesPanelView->toggleRelativeMode();
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
	if (ew->widgets_changed)
	{
		notifyWidgetsUpdate();
		ew->widgets_changed = false;
	}

	if (recreate)
	{
		recreate = false;
		notifySelectWidget(nullptr); // виджет пересоздался, теперь никто незнает его адреса :)
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
	current_widget = nullptr;
	notifySelectWidget(current_widget1);
}
//===================================================================================
void EditorState::loadSettings(std::string _fileName, bool _ogreResourse)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	std::string file;
	if (_ogreResourse) file = MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (file.empty()) {
		file = _fileName;
	}

	if (false == doc.open(file)) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if ( (nullptr == root) || (root->getName() != "MyGUI") ) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Settings")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator field = root->getElementEnumerator();
			while (field.next()) {
				if (field->getName() == "PropertiesPanelView") mPropertiesPanelView->load(field);
				else if (field->getName() == "SettingsWindow") mSettingsWindow->load(field);
				else if (field->getName() == "WidgetsWindow") mWidgetsWindow->load(field);
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

void EditorState::saveSettings(std::string _fileName, bool _ogreResourse)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	std::string file;
	if (_ogreResourse) file = MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (file.empty()) {
		file = _fileName;
	}

	doc.createDeclaration();
	MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
	root->addAttribute("type", "Settings");

	mPropertiesPanelView->save(root);
	mSettingsWindow->save(root);
	mWidgetsWindow->save(root);

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
		MyGUI::xml::ElementPtr nodeProp = root->createChild("RecentFile");
		nodeProp->addAttribute("name", *iter);
	}

	if (false == doc.save(file)) {
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}
}

void EditorState::notifySave()
{
	if (fileName != "")
	{
		saveOrLoadLayout<true>(fileName);
	}
	else
	{
		mSaveLoadWindow->setSave(fileName);
	}
}

void EditorState::notifySettings()
{
	mSettingsWindow->setVisible(true);
	MyGUI::LayerManager::getInstance().upLayerItem(mSettingsWindow->getMainWidget());
}

void EditorState::notifyTest()
{
	for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
	{
		MyGUI::ControllerManager::getInstance().removeItem(*iter);
		(*iter)->setPosition((*iter)->getPosition() + MyGUI::IntPoint(-2048, -2048));
	}
	bar->setVisible(false);
	testLayout = ew->savexmlDocument();
	ew->clear();
	notifySelectWidget(nullptr);
	ew->loadxmlDocument(testLayout, true);
	testMode = true;
}

void EditorState::notifyClear()
{
	MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), localise("Warn_delete_all_widgets"), MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No, "Overlapped");
	message->eventMessageBoxResult = newDelegate(this, &EditorState::notifyClearMessage);
}

void EditorState::notifyClearMessage(MyGUI::MessagePtr _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes || _result == MyGUI::MessageBoxStyle::Button1)
	{
		clear();
	}
}

void EditorState::clear(bool _clearName)
{
	mWidgetsWindow->clearNewWidget();
	recreate = false;
	if (_clearName) fileName = "";
	testMode = false;
	ew->clear();
	notifySelectWidget(nullptr);
	um->addValue();
	selectDepth = 0;

	if (_clearName) BasisManager::getInstance().setWindowCaption("MyGUI Layout Editor");
}

void EditorState::notifyQuit()
{
	MyGUI::MessagePtr message = MyGUI::Message::createMessageBox("Message", localise("Warning"), localise("Warn_exit"), MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No, "Overlapped");
	message->eventMessageBoxResult = newDelegate(this, &EditorState::notifyQuitMessage);
}

void EditorState::notifyQuitMessage(MyGUI::MessagePtr _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes || _result == MyGUI::MessageBoxStyle::Button1)
	{
		BasisManager::getInstance().eventExit();
	}
}

template <bool Save>
void EditorState::saveOrLoadLayout(const std::string & _file)
{
	if ( (Save && ew->save(_file)) ||
		(!Save && ew->load(_file)) )
	{
		fileName = _file;
		BasisManager::getInstance().setWindowCaption(_file + " - MyGUI Layout Editor");
		recentFiles.push_back(_file);

		mSaveLoadWindow->setVisible(false);

		um->addValue();
	}
	else
	{
		std::string saveLoad = Save ? localise("Save") : localise("Load");
		MyGUI::MessagePtr message = MyGUI::Message::createMessageBox(
			"Message",
			localise("Warning"),
			"Failed to " + saveLoad + " file '" + _file + "'",
			MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok,
			"Overlapped"
			);
	}
}

void EditorState::notifyWidgetsUpdate()
{
	bool print_name = mSettingsWindow->getShowName();
	bool print_type = mSettingsWindow->getShowType();
	bool print_skin = mSettingsWindow->getShowSkin();

	mPopupMenuWidgets->removeAllItems();
	mPopupMenuWidgets->eventMenuCtrlAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);

	for (std::vector<WidgetContainer*>::iterator iter = ew->widgets.begin(); iter != ew->widgets.end(); ++iter )
	{
		createWidgetPopup(*iter, mPopupMenuWidgets, print_name, print_type, print_skin);
	}
}

void EditorState::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrlPtr _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
{
	bool submenu = !_container->childContainers.empty();

	_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
	_parentPopup->setItemDataAt(_parentPopup->getItemCount()-1, _container->widget);

	if (submenu) {
		MyGUI::MenuCtrlPtr child = _parentPopup->createItemChildAt(_parentPopup->getItemCount()-1);
		child->eventMenuCtrlAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);
		child->setPopupAccept(true);

		for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
		{
			createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
		}
	}
}

void EditorState::notifyWidgetsSelect(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)
{
	MyGUI::WidgetPtr widget = *_item->getItemData<MyGUI::WidgetPtr>();
	//MyGUI::WidgetPtr widget = *_widget->castType<MyGUI::PopupMenu>()->getItemInfoAt(_index).data.castType<MyGUI::WidgetPtr>();
	notifySelectWidget(widget);
}

void EditorState::notifySelectWidget(MyGUI::WidgetPtr _sender)
{
	if (_sender == current_widget)
	{
		if (current_widget)
		{
			mPropertiesPanelView->getWidgetRectangle()->setVisible(true);
			MyGUI::InputManager::getInstance().setKeyFocusWidget(mPropertiesPanelView->getWidgetRectangle());
		}
		return;
	}

	current_widget = _sender;

	mPropertiesPanelView->update(_sender);
	mWidgetsWindow->update(_sender);
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
			/*name = _widget->getName();
			if (0 == strncmp("LayoutEditorWidget_", name.c_str(), 19))
			{
					std::string::iterator iter = std::find(name.begin(), name.end(), '_');
					if (iter != name.end()) name.erase(name.begin(), ++iter);
					name = "#{ColourMenuName}" + name;
			}
			name = "#{ColourMenuName}[" + name + "] ";*/
		}
		else {
			name = "#{ColourMenuName}'" + widgetContainer->name + "' ";
		}
	}

	if (_print_type)
	{
		type = "#{ColourMenuType}[" + _widget->getTypeName() + "] ";
	}

	if (_print_skin)
	{
		skin = "#{ColourMenuSkin}" + widgetContainer->skin + " ";
	}
	return MyGUI::LanguageManager::getInstance().replaceTags(type + skin + name);
}

void EditorState::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	if (_info.type == MyGUI::ToolTipInfo::Show) {
		mToolTip->show(_sender, _info.point);
	}
	else if (_info.type == MyGUI::ToolTipInfo::Hide) {
		mToolTip->hide();
	}
}

