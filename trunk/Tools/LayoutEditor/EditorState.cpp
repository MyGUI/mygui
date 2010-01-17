#include "precompiled.h"
#include "Common.h"
#include "EditorState.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "Base/Main.h"
#include "GroupMessage.h"

#define ON_EXIT( CODE ) class _OnExit { public: ~_OnExit() { CODE; } } _onExit

const std::string LogSection = "LayoutEditor";

const std::wstring settingsFile = L"settings.xml";
const std::wstring userSettingsFile = L"le_user_settings.xml";

const float POSITION_CONTROLLER_TIME = 0.5f;
const int HIDE_REMAIN_PIXELS = 3;

void eventInfo(MyGUI::Widget* _sender, const std::string& _key, const std::string& _event)
{
	MyGUI::MYGUI_OUT("eventInfo: ", _event);
}

template<typename Type>
class PtrHolder
{
public:
	PtrHolder(Type* _data) : mData(_data) { }
	~PtrHolder() { delete mData; }
private:
	Type* mData;
};

//===================================================================================
void EditorState::setupResources()
{
	base::BaseManager::setupResources();
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Panels");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Themes");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Settings");
	addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	setResourceFilename("editor.xml");
}
//===================================================================================
void EditorState::createScene()
{
	getStatisticInfo()->setVisible(false);

	MyGUI::LogManager::registerSection(LogSection, "MyGUI.log");

	//FIXME
	// set locale language if it was taken from OS
	//if (! BasisManager::getInstance().getLanguage().empty() )
	//	MyGUI::LanguageManager::getInstance().setCurrentLanguage(BasisManager::getInstance().getLanguage());
	// if you want to test LanguageManager uncomment next line
	//MyGUI::LanguageManager::getInstance().setCurrentLanguage("Russian");
	testMode = false;

	wt = new WidgetTypes();
	wt->initialise();
	ew = new EditorWidgets();
	ew->initialise();
	um = new UndoManager();
	um->initialise(ew);
	mGroupMessage = new GroupMessage();

	MyGUI::ResourceManager::getInstance().load("initialise.xml");

	mToolTip = new EditorToolTip();
	//MyGUI::DelegateManager::getInstance().addDelegate("eventInfo", MyGUI::newDelegate(eventInfo));
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

	mMetaSolutionWindow = new MetaSolutionWindow();
	mMetaSolutionWindow->eventLoadFile = MyGUI::newDelegate(this, &EditorState::saveOrLoadLayoutEvent<false>);
	mMetaSolutionWindow->eventSelectWidget = MyGUI::newDelegate(this, &EditorState::notifySelectWidget);
	interfaceWidgets.push_back(mMetaSolutionWindow->getMainWidget());

	mOpenSaveFileDialog = new common::OpenSaveFileDialog();
	mOpenSaveFileDialog->setVisible(false);
	mOpenSaveFileDialog->setFileMask("*.layout");
	mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyOpenSaveEndDialog);

	loadSettings(settingsFile, true);
	loadSettings(userSettingsFile, false);

	// создание меню
	createMainMenu();

	MyGUI::Widget* widget = mPropertiesPanelView->getMainWidget();
	widget->setCoord(
		widget->getParentSize().width - widget->getSize().width,
		bar->getHeight(),
		widget->getSize().width,
		widget->getParentSize().height - bar->getHeight()
		);

	// после загрузки настроек инициализируем
	mWidgetsWindow->initialise();

	if (mSettingsWindow->getEdgeHide())
	{
		for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerEdgeHide::getClassTypeName());
			MyGUI::ControllerEdgeHide* controller = item->castType<MyGUI::ControllerEdgeHide>();

			controller->setTime(POSITION_CONTROLLER_TIME);
			controller->setRemainPixels(HIDE_REMAIN_PIXELS);
			controller->setShadowSize(3);

			MyGUI::ControllerManager::getInstance().addItem(*iter, controller);
		}
	}

	clear();

	/*MyGUI::Widget* mFpsInfo = mGUI->createWidget<MyGUI::Widget>("ButtonSmall", 20, (int)mGUI->getViewHeight() - 80, 120, 70, MyGUI::Align::Left | MyGUI::Align::Bottom, "Main", "fpsInfo");
	mFpsInfo->setColour(Ogre::ColourValue::White);*/

	//FIXME
	/*typedef std::vector<std::string> Params;
	Params params = BasisManager::getInstance().getCommandParams();
	for (Params::iterator iter=params.begin(); iter!=params.end(); ++iter)
	{
		saveOrLoadLayout(false, false, iter->c_str());
	}*/
	getGUI()->eventFrameStart += MyGUI::newDelegate(this, &EditorState::notifyFrameStarted);

	for (std::vector<MyGUI::UString>::iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
	{
		addResourceLocation(*iter);
	}
}

void EditorState::destroyScene()
{
	getGUI()->eventFrameStart -= MyGUI::newDelegate(this, &EditorState::notifyFrameStarted);

	saveSettings(userSettingsFile);

	delete mPropertiesPanelView;	
	mPropertiesPanelView = nullptr;

	delete mGroupMessage;

	um->shutdown();
	delete um;		
	um = nullptr;

	ew->shutdown();
	delete ew;		
	ew = nullptr;

	wt->shutdown();
	delete wt;						
	wt = nullptr;

	delete mToolTip;				
	mToolTip = nullptr;

	delete mSettingsWindow;			
	mSettingsWindow = nullptr;

	delete mMetaSolutionWindow;		
	mMetaSolutionWindow = nullptr;

	delete mWidgetsWindow;			
	mWidgetsWindow = nullptr;

	delete mOpenSaveFileDialog;		
	mOpenSaveFileDialog = nullptr;

	MyGUI::LogManager::unregisterSection(LogSection);
}

void EditorState::createMainMenu()
{
	MyGUI::VectorWidgetPtr menu_items = MyGUI::LayoutManager::getInstance().load("interface_menu.layout");
	MYGUI_ASSERT(menu_items.size() == 1, "Error load main menu");
	bar = menu_items[0]->castType<MyGUI::MenuBar>();
	bar->setCoord(0, 0, bar->getParentSize().width, bar->getHeight());

	// главное меню
	MyGUI::MenuItem* menu_file = bar->getItemById("File");
	mPopupMenuFile = menu_file->getItemChild();
	// список последних открытых файлов
	if (recentFiles.size())
	{
		MyGUI::MenuItem* menu_item = mPopupMenuFile->getItemById("File/Quit");
		for (std::vector<MyGUI::UString>::reverse_iterator iter = recentFiles.rbegin(); iter != recentFiles.rend(); ++iter)
		{
			mPopupMenuFile->insertItem(menu_item, *iter, MyGUI::MenuItemType::Normal, "File/RecentFiles",  *iter);
		}
		// если есть файлы, то еще один сепаратор
		mPopupMenuFile->insertItem(menu_item, "", MyGUI::MenuItemType::Separator);
	}

	//хак, для меню тест двойная замена
	MyGUI::MenuItem* menu_item_test = mPopupMenuFile->getItemById("File/Test");
	menu_item_test->setCaption(MyGUI::LanguageManager::getInstance().replaceTags(menu_item_test->getCaption()));

	// меню для виджетов
	MyGUI::MenuItem* menu_widget = bar->getItemById("Widgets");
	mPopupMenuWidgets = menu_widget->createItemChild();
	//FIXME
	mPopupMenuWidgets->setPopupAccept(true);

	bar->eventMenuCtrlAccept = newDelegate(this, &EditorState::notifyPopupMenuAccept);

	interfaceWidgets.push_back(bar);
}

void EditorState::notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
{
	if (mPopupMenuFile == _item->getMenuCtrlParent())
	{
		std::string id = _item->getItemId();

		if (id == "File/Load")
		{
			notifyLoad();
		}
		else if (id == "File/Save")
		{
			notifySave();
		}
		else if (id == "File/SaveAs")
		{
			setModeSaveLoadDialog(true, fileName);
		}
		else if (id == "File/Clear")
		{
			notifyClear();
		}
		else if (id == "File/Settings")
		{
			notifySettings();
		}
		else if (id == "File/Test")
		{
			notifyTest();
		}
		else if (id == "File/RecentFiles")
		{
			saveOrLoadLayout(false, false, *_item->getItemData<MyGUI::UString>());
		}
		else if (id == "File/Quit")
		{
			notifyQuit();
		}
	}
}

//===================================================================================
void EditorState::injectMouseMove(int _absx, int _absy, int _absz)
{
	if (testMode)
	{
		base::BaseManager::injectMouseMove(_absx, _absy, _absz);
		return;
	}

	// drop select depth if we moved mouse
	const int DIST = 2;
	if ((abs(x - _absx) > DIST) || (abs(y - _absy) > DIST))
	{
		selectDepth = 0;
		x = _absx;
		y = _absy;
	}

	// align to grid if shift not pressed
	int x2, y2;
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x2 = toGrid(_absx);
		y2 = toGrid(_absy);
	}
	else
	{
		x2 = _absx;
		y2 = _absy;
	}

	mWidgetsWindow->createNewWidget(x2, y2);

	base::BaseManager::injectMouseMove(_absx, _absy, _absz);
}
//===================================================================================
void EditorState::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
{
	if (testMode)
	{
		return base::BaseManager::injectMousePress(_absx, _absy, _id);
	}

	if (MyGUI::InputManager::getInstance().isModalAny())
	{
		// if we have modal widgets we can't select any widget
		base::BaseManager::injectMousePress(_absx, _absy, _id);
		return;
	}

	// align to grid if shift not pressed
	int x1, y1;
	if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
	{
		x1 = toGrid(_absx);
		y1 = toGrid(_absy);
	}
	else
	{
		x1 = _absx;
		y1 = _absy;
	}

	// юбилейный комит  =)
	mWidgetsWindow->startNewWidget(x1, y1, _id);

	// это чтобы можно было двигать прямоугольник у невидимых виджето (или виджетов за границами)
	//MyGUI::LayerItemInfoPtr rootItem = nullptr;
	//MyGUI::Widget* itemWithRect = static_cast<MyGUI::Widget*>(MyGUI::LayerManager::getInstance().findWidgetItem(_absx, _absy, rootItem));
	// не стал это доделывать, т.к. неоднозначность выбора виджета получается, если кто скажет как выбирать - сделаю

	MyGUI::Widget* item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);

	// не убираем прямоугольник если нажали на его растягивалку
	if (item && (item->getParent() != mPropertiesPanelView->getWidgetRectangle()))
	{
		// чтобы прямоугольник не мешался
		mPropertiesPanelView->getWidgetRectangle()->setVisible(false);
		item = MyGUI::LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);
	}

	if (nullptr != item)
	{
		// find widget registered as container
		while ((nullptr == ew->find(item)) && (nullptr != item)) item = item->getParent();
		MyGUI::Widget* oldItem = item;

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
			if (mWidgetsWindow->getCreatingStatus() != 1)
			{
				//FIXME
				getGUI()->injectMouseMove(_absx, _absy, 0);// это чтобы сразу можно было тащить
			}
		}
		//FIXME
		getGUI()->injectMousePress(_absx, _absy, _id);
	}
	else
	{
		//FIXME
		getGUI()->injectMousePress(_absx, _absy, _id);
		notifySelectWidget(nullptr);
	}

	// вернем прямоугольник
	if (current_widget && mWidgetsWindow->getCreatingStatus() == 0)
	{
		mPropertiesPanelView->getWidgetRectangle()->setVisible(true);
	}
	else if (mWidgetsWindow->getCreatingStatus())
	{
		mPropertiesPanelView->getWidgetRectangle()->setVisible(false);
	}

	//base::BaseManager::injectMousePress(_absx, _absy, _id);
}
//===================================================================================
void EditorState::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
{
	selectDepth++;
	if (testMode)
	{
		base::BaseManager::injectMouseRelease(_absx, _absy, _id);
		return;
	}

	if (MyGUI::InputManager::getInstance().isModalAny())
	{
	}
	else
	{
		// align to grid if shift not pressed
		int x2, y2;
		if (MyGUI::InputManager::getInstance().isShiftPressed() == false)
		{
			x2 = toGrid(_absx);
			y2 = toGrid(_absy);
		}
		else
		{
			x2 = _absx;
			y2 = _absy;
		}

		mWidgetsWindow->finishNewWidget(x2, y2);
	}

	um->dropLastProperty();

	base::BaseManager::injectMouseRelease(_absx, _absy, _id);
}
//===================================================================================
void EditorState::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
{
	MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

	if (testMode)
	{
		if (input.isModalAny() == false)
		{
			if (_key == MyGUI::KeyCode::Escape)
			{
				notifyEndTest();
			}
		}

		getGUI()->injectKeyPress(_key, _text);
		//base::BaseManager::injectKeyPress(_key, _text);
		return;
	}

	if (input.isModalAny())
	{
		if (mOpenSaveFileDialog->isVisible())
		{
			if (_key == MyGUI::KeyCode::Escape)
				mOpenSaveFileDialog->eventEndDialog(false);
			else if (_key == MyGUI::KeyCode::Return)
				mOpenSaveFileDialog->eventEndDialog(true);
		}
	}
	else
	{
		if (_key == MyGUI::KeyCode::Escape)
		{
			notifyQuit();
			return;
		}

		if (input.isControlPressed())
		{
			if (_key == MyGUI::KeyCode::O
				|| _key == MyGUI::KeyCode::L)
				notifyLoad();
			else if (_key == MyGUI::KeyCode::S)
				notifySave();
			else if (_key == MyGUI::KeyCode::Z)
			{
				um->undo();
				notifySelectWidget(nullptr);
			}
			else if ((_key == MyGUI::KeyCode::Y) || ((input.isShiftPressed()) && (_key == MyGUI::KeyCode::Z)))
			{
				um->redo();
				notifySelectWidget(nullptr);
			}
			else if (_key == MyGUI::KeyCode::T)
			{
				notifyTest();
				return;
			}
			else if (_key == MyGUI::KeyCode::R)
			{
				mPropertiesPanelView->toggleRelativeMode();
				return;
			}
		}
	}

	getGUI()->injectKeyPress(_key, _text);
	//base::BaseManager::injectKeyPress(_key, _text);
}
//===================================================================================
void EditorState::injectKeyRelease(MyGUI::KeyCode _key)
{
	if (testMode)
	{
		return base::BaseManager::injectKeyRelease(_key);
	}

	return base::BaseManager::injectKeyRelease(_key);
}
//===================================================================================
void EditorState::notifyFrameStarted(float _time)
{
	GroupMessage::getInstance().showMessages();

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

	//return base::BaseManager::frameStarted(evt);
	//return true;
}
//===================================================================================
/*void EditorState::windowResize()
{
	if (testMode)
		return;

	// force update
	MyGUI::Widget* current_widget1 = current_widget;
	current_widget = nullptr;
	notifySelectWidget(current_widget1);
}*/
//===================================================================================
bool EditorState::isNeedSolutionLoad(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();
	while (field.next())
	{
		std::string key, value;

		if (field->getName() == "Property")
		{
			if (!field->findAttribute("key", key)) continue;
			if (!field->findAttribute("value", value)) continue;

			if (key == "MetaSolutionName")
			{
				return !value.empty();
			}
		}
	}
	return false;
}
//===================================================================================
void EditorState::loadSettings(const MyGUI::UString& _fileName, bool _internal)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	if (_internal)
	{
		MyGUI::IDataStream* data = MyGUI::DataManager::getInstance().getData(_fileName);
		if (data)
		{
			PtrHolder<MyGUI::IDataStream> holder = PtrHolder<MyGUI::IDataStream>(data);

			if (!doc.open(data))
			{
				MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
				return;
			}
		}
	}
	else
	{
		if (!doc.open(_fileName))
		{
			MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
			return;
		}
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if (!root || (root->getName() != "MyGUI"))
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return;
	}

	std::string type;
	if (root->findAttribute("type", type))
	{
		if (type == "Settings")
		{
			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator field = root->getElementEnumerator();
			while (field.next())
			{
				if (field->getName() == "PropertiesPanelView") mPropertiesPanelView->load(field);
				else if (field->getName() == "SettingsWindow") mSettingsWindow->load(field);
				else if (field->getName() == "WidgetsWindow") mWidgetsWindow->load(field);
				else if (field->getName() == "MetaSolutionWindow")
				{
					if (isNeedSolutionLoad(field))
					{
						clearWidgetWindow();
						mMetaSolutionWindow->load(field);
					}
				}
				else if (field->getName() == "RecentFile")
				{
					std::string name;
					if (!field->findAttribute("name", name)) continue;
					recentFiles.push_back(name);
				}
				else if (field->getName() == "AdditionalPath")
				{
					std::string name;
					if (!field->findAttribute("name", name)) continue;
					additionalPaths.push_back(name);
				}
			}
		}
	}
}

void EditorState::saveSettings(const MyGUI::UString& _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::Document doc;
	doc.createDeclaration();
	MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
	root->addAttribute("type", "Settings");

	mPropertiesPanelView->save(root);
	mSettingsWindow->save(root);
	mWidgetsWindow->save(root);
	mMetaSolutionWindow->save(root);

	// cleanup for duplicates

	std::reverse(recentFiles.begin(), recentFiles.end());
	for (size_t i = 0; i < recentFiles.size(); ++i)
		recentFiles.erase(std::remove(recentFiles.begin() + i + 1, recentFiles.end(), recentFiles[i]), recentFiles.end());

	// remove old files
	while (recentFiles.size() > MAX_RECENT_FILES)
		recentFiles.pop_back();
	std::reverse(recentFiles.begin(), recentFiles.end());

	for (std::vector<MyGUI::UString>::iterator iter = recentFiles.begin(); iter != recentFiles.end(); ++iter)
	{
		MyGUI::xml::ElementPtr nodeProp = root->createChild("RecentFile");
		nodeProp->addAttribute("name", *iter);
	}

	for (std::vector<MyGUI::UString>::iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
	{
		MyGUI::xml::ElementPtr nodeProp = root->createChild("AdditionalPath");
		nodeProp->addAttribute("name", *iter);
	}

	if (!doc.save(_fileName))
	{
		MYGUI_LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return;
	}
}

void EditorState::notifyLoad()
{
	if (um->isUnsaved())
	{
		MyGUI::Message* message = MyGUI::Message::createMessageBox(
			"Message",
			localise("Warning"),
			localise("Warn_unsaved_data"),
			MyGUI::MessageBoxStyle::IconWarning |
			MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No | MyGUI::MessageBoxStyle::Cancel,
			"Overlapped"
			);
		message->eventMessageBoxResult = newDelegate(this, &EditorState::notifyConfirmLoadMessage);
		message->setUserString("FileName", fileName);
		return;
	}

	setModeSaveLoadDialog(false, fileName);
}

bool EditorState::notifySave()
{
	if (fileName != "")
	{
		return saveOrLoadLayout(true, false, fileName);
	}
	else
	{
		setModeSaveLoadDialog(true, fileName);
		return false;
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
		if ((*iter)->isVisible())
		{
			(*iter)->setUserString("WasVisible", "true");
			(*iter)->setVisible(false);
		}
	}
	testLayout = ew->savexmlDocument();
	ew->clear();
	notifySelectWidget(nullptr);
	ew->loadxmlDocument(testLayout, true);
	testMode = true;
}

void EditorState::notifyEndTest()
{
	for (MyGUI::VectorWidgetPtr::iterator iter = interfaceWidgets.begin(); iter != interfaceWidgets.end(); ++iter)
	{
		if ((*iter)->getUserString("WasVisible") == "true")
		{
			(*iter)->setVisible(true);
		}
	}
	testMode = false;
	clear(false);
	ew->loadxmlDocument(testLayout);
}

void EditorState::notifyClear()
{
	MyGUI::Message* message = MyGUI::Message::createMessageBox("Message", localise("Warning"), localise("Warn_delete_all_widgets"), MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No, "Overlapped");
	message->eventMessageBoxResult = newDelegate(this, &EditorState::notifyClearMessage);
}

void EditorState::notifyClearMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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
	um->shutdown();
	um->initialise(ew);
	selectDepth = 0;

	if (_clearName)
		setWindowCaption("MyGUI Layout Editor");
}

void EditorState::notifyQuit()
{
	if (um->isUnsaved())
	{
		MyGUI::Message* message = MyGUI::Message::createMessageBox(
			"Message",
			localise("Warning"),
			localise("Warn_unsaved_data"),
			MyGUI::MessageBoxStyle::IconWarning |
			MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No | MyGUI::MessageBoxStyle::Cancel,
			"Overlapped"
			);
		message->eventMessageBoxResult = newDelegate(this, &EditorState::notifyConfirmQuitMessage);
		message->setUserString("FileName", fileName);
		return;
	}

	// выходим
	quit();
}

void EditorState::notifyConfirmQuitMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if ( _result == MyGUI::MessageBoxStyle::Yes )
	{
		if (notifySave())
		{
			// выходим
			quit();
		}
	}
	else if ( _result == MyGUI::MessageBoxStyle::No )
	{
		// выходим
		quit();
	}
	/*else if ( _result == MyGUI::MessageBoxStyle::Cancel )
	{
		// do nothing
	}
	*/
}

bool EditorState::isMetaSolution(const MyGUI::UString& _fileName)
{
	MyGUI::xml::Document doc;
	if (!doc.open(_fileName))
	{
		return false;
	}

	MyGUI::xml::ElementPtr root = doc.getRoot();
	if (!root || (root->getName() != "MyGUI"))
	{
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type))
	{
		if (type == "MetaSolution")
		{
			return true;
		}
	}

	return false;
}

void EditorState::clearWidgetWindow()
{
	WidgetTypes::getInstance().clearAllSkins();
	mWidgetsWindow->clearAllSheets();
}

void EditorState::loadFile(const std::wstring& _file)
{
	// если солюшен, то очищаем
	bool solution = isMetaSolution(MyGUI::UString(_file).asUTF8_c_str());
	if (solution)
	{
		clearWidgetWindow();
	}

	if (!saveOrLoadLayout(false, true, MyGUI::UString(_file).asUTF8_c_str()))
	{
		MyGUI::ResourceManager::getInstance().load(MyGUI::UString(_file).asUTF8_c_str()/*, ""*/);
	}

	if (solution)
	{
		this->mWidgetsWindow->initialise();
	}
}

void EditorState::notifyConfirmLoadMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if ( _result == MyGUI::MessageBoxStyle::Yes )
	{
		if (notifySave())
		{
			setModeSaveLoadDialog(false, fileName);
		}
	}
	else if ( _result == MyGUI::MessageBoxStyle::No )
	{
		setModeSaveLoadDialog(false, fileName);
	}
	/*else if ( _result == MyGUI::MessageBoxStyle::Cancel )
	{
		// do nothing
	}
	*/
}

void EditorState::notifyWidgetsUpdate()
{
	if (mMetaSolutionWindow->isVisible())
		mMetaSolutionWindow->updateList();

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

void EditorState::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
{
	bool submenu = !_container->childContainers.empty();

	_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
	_parentPopup->setItemDataAt(_parentPopup->getItemCount()-1, _container->widget);

	if (submenu)
	{
		MyGUI::MenuCtrl* child = _parentPopup->createItemChildAt(_parentPopup->getItemCount()-1);
		child->eventMenuCtrlAccept = MyGUI::newDelegate(this, &EditorState::notifyWidgetsSelect);
		child->setPopupAccept(true);

		for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
		{
			createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
		}
	}
}

void EditorState::notifyWidgetsSelect(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
{
	MyGUI::Widget* widget = *_item->getItemData<MyGUI::Widget*>();
	//MyGUI::Widget* widget = *_widget->castType<MyGUI::PopupMenu>()->getItemInfoAt(_index).data.castType<MyGUI::Widget*>();
	notifySelectWidget(widget);
}

void EditorState::notifySelectWidget(MyGUI::Widget* _sender)
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
	mMetaSolutionWindow->update(_sender);
}

std::string EditorState::getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin)
{
	std::string name = "";
	std::string type = "";
	std::string skin = "";

	WidgetContainer * widgetContainer = ew->find(_widget);
	if (_print_name)
	{
		if (widgetContainer->name.empty())
		{
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
		else
		{
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

void EditorState::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo & _info)
{
	if (_info.type == MyGUI::ToolTipInfo::Show)
	{
		mToolTip->show(_sender, _info.point);
	}
	else if (_info.type == MyGUI::ToolTipInfo::Hide)
	{
		mToolTip->hide();
	}
}

void EditorState::notifyOpenSaveEndDialog(bool _result)
{
	if (_result)
	{
		MyGUI::UString file = mOpenSaveFileDialog->getCurrentFolder() + L"/" + mOpenSaveFileDialog->getFileName();
		saveOrLoadLayout(mModeSaveDialog, false, file);
	}
	else
	{
		mOpenSaveFileDialog->setVisible(false);
	}
}

void EditorState::setModeSaveLoadDialog(bool _save, const MyGUI::UString& _filename)
{
	if (_save)
		mOpenSaveFileDialog->setDialogInfo(localise("Save"), localise("Save"));
	else
		mOpenSaveFileDialog->setDialogInfo(localise("Load"), localise("Load"));

	size_t pos = _filename.find_last_of(L"\\/");
	if (pos == MyGUI::UString::npos)
	{
		mOpenSaveFileDialog->setFileName(_filename);
	}
	else
	{
		mOpenSaveFileDialog->setCurrentFolder(_filename.substr(0, pos));
		mOpenSaveFileDialog->setFileName(_filename.substr(pos + 1));
	}

	mOpenSaveFileDialog->setVisible(true);
	mModeSaveDialog = _save;
}

bool EditorState::saveOrLoadLayout(bool Save, bool Silent, const MyGUI::UString& _file)
{
	if (!Save) clear();

	if ( (Save && ew->save(_file)) ||
		(!Save && ew->load(_file)) )
	{
		fileName = _file;
		setWindowCaption(_file + " - MyGUI Layout Editor");
		recentFiles.push_back(_file);

		mOpenSaveFileDialog->setVisible(false);

		um->addValue();
		um->setUnsaved(false);
		return true;
	}
	else if (!Silent)
	{
		std::string saveLoad = Save ? localise("Save") : localise("Load");
		/*MyGUI::Message* message =*/ MyGUI::Message::createMessageBox(
			"Message",
			localise("Warning"),
			"Failed to " + saveLoad + " file '" + _file + "'",
			MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok,
			"Overlapped"
			);
	}

	return false;
}

MYGUI_APP(EditorState)
