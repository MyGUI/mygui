#include "precompiled.h"
#include "Common.h"
#include "EditorState.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "Base/Main.h"
#include "GroupMessage.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "CommandManager.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "HotKeyManager.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"

const int BAR_HEIGHT = 30;

EditorState::EditorState() :
	mLastClickX(0),
	mLastClickY(0),
	mSelectDepth(0),
	mRecreate(false),
	//mTestMode(false),
	mPropertiesPanelView(nullptr),
	//mSettingsWindow(nullptr),
	mWidgetsWindow(nullptr),
	//mCodeGenerator(nullptr),
	mOpenSaveFileDialog(nullptr),
	mMainMenuControl(nullptr),
	mFileName("unnamed.xml"),
	mDefaultFileName("unnamed.xml")
{
}

EditorState::~EditorState()
{
}

void EditorState::setupResources()
{
	base::BaseManager::setupResources();
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Panels");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Themes");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Settings");
	addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/CodeTemplates");
	addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	setResourceFilename("editor.xml");
}

void EditorState::createScene()
{
	getStatisticInfo()->setVisible(false);

	new tools::SettingsManager();
	tools::SettingsManager::getInstance().initialise();

	new tools::CommandManager();
	tools::CommandManager::getInstance().initialise();

	new tools::WidgetSelectorManager();
	tools::WidgetSelectorManager::getInstance().initialise();

	new tools::HotKeyManager();
	tools::HotKeyManager::getInstance().initialise();

	new tools::MessageBoxManager();
	tools::MessageBoxManager::getInstance().initialise();

	new tools::DialogManager();
	tools::DialogManager::getInstance().initialise();

	// set locale language if it was taken from OS
	if (!mLocale.empty())
		MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);
	// if you want to test LanguageManager uncomment next line
	//MyGUI::LanguageManager::getInstance().setCurrentLanguage("Russian");

	//mTestMode = false;

	new WidgetTypes();
	WidgetTypes::getInstance().initialise();

	new EditorWidgets();
	EditorWidgets::getInstance().initialise();

	new UndoManager();
	UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());

	new GroupMessage();

	MyGUI::ResourceManager::getInstance().load("initialise.xml");

	mInterfaceWidgets = MyGUI::LayoutManager::getInstance().loadLayout("interface.layout", "LayoutEditor_");

	// settings window
	//mSettingsWindow = new SettingsWindow();
	//mSettingsWindow->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifySettingsWindowEndDialog);
	//mInterfaceWidgets.push_back(mSettingsWindow->getMainWidget());

	// properties panelView
	mPropertiesPanelView = new PropertiesPanelView();
	mPropertiesPanelView->eventRecreate = MyGUI::newDelegate(this, &EditorState::notifyRecreate);
	mInterfaceWidgets.push_back(mPropertiesPanelView->getMainWidget());

	mWidgetsWindow = new WidgetsWindow();
	mInterfaceWidgets.push_back(mWidgetsWindow->getMainWidget());

	//mCodeGenerator = new CodeGenerator();
	//mInterfaceWidgets.push_back(mCodeGenerator->getMainWidget());
	//EditorWidgets::getInstance().setCodeGenerator(mCodeGenerator);

	mOpenSaveFileDialog = new tools::OpenSaveFileDialog();
	mOpenSaveFileDialog->setFileMask("*.layout");
	mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &EditorState::notifyEndDialog);

	mMainMenuControl = new tools::MainMenuControl();
	mInterfaceWidgets.push_back(mMainMenuControl->getMainWidget());

	MyGUI::Widget* widget = mPropertiesPanelView->getMainWidget();
	widget->setCoord(
		widget->getParentSize().width - widget->getSize().width,
		BAR_HEIGHT,
		widget->getSize().width,
		widget->getParentSize().height - BAR_HEIGHT
		);

	// после загрузки настроек инициализируем
	mWidgetsWindow->initialise();

	const tools::VectorUString& additionalPaths = tools::SettingsManager::getInstance().getAdditionalPaths();
	for (tools::VectorUString::const_iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
		addResourceLocation(*iter);

	tools::CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &EditorState::commandLoad));
	tools::CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &EditorState::commandSave));
	tools::CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &EditorState::commandSaveAs));
	tools::CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &EditorState::commandClear));
	//tools::CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &EditorState::commandTest));
	tools::CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &EditorState::commandQuit));
	tools::CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &EditorState::commandQuitApp));
	//tools::CommandManager::getInstance().registerCommand("Command_Settings", MyGUI::newDelegate(this, &EditorState::commandSettings));
	//tools::CommandManager::getInstance().registerCommand("Command_CodeGenerator", MyGUI::newDelegate(this, &EditorState::commandCodeGenerator));
	//tools::CommandManager::getInstance().registerCommand("Command_RecentFiles", MyGUI::newDelegate(this, &EditorState::commandRecentFiles));
	//tools::CommandManager::getInstance().registerCommand("Command_StatisticInfo", MyGUI::newDelegate(this, &EditorState::commandStatisticInfo));
	//tools::CommandManager::getInstance().registerCommand("Command_FocusVisible", MyGUI::newDelegate(this, &EditorState::commandFocusVisible));

	// загружаем файлы которые были в командной строке
	/*for (std::vector<std::wstring>::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter)
	{
		load(iter->c_str());
	}*/

	updateCaption();

	for (VectorWString::const_iterator file = getParams().begin(); file != getParams().end(); ++file)
	{
		mFileName = *file;
		tools::addUserTag("CurrentFileName", mFileName);

		load();
		updateCaption();
		break;
	}

	UndoManager::getInstance().eventChanges += MyGUI::newDelegate(this, &EditorState::notifyChanges);
	getGUI()->eventFrameStart += MyGUI::newDelegate(this, &EditorState::notifyFrameStarted);
}

void EditorState::destroyScene()
{
	UndoManager::getInstance().eventChanges -= MyGUI::newDelegate(this, &EditorState::notifyChanges);
	getGUI()->eventFrameStart -= MyGUI::newDelegate(this, &EditorState::notifyFrameStarted);

	delete mMainMenuControl;
	mMainMenuControl = nullptr;

	delete mPropertiesPanelView;
	mPropertiesPanelView = nullptr;

	delete GroupMessage::getInstancePtr();

	tools::DialogManager::getInstance().shutdown();
	delete tools::DialogManager::getInstancePtr();

	tools::MessageBoxManager::getInstance().shutdown();
	delete tools::MessageBoxManager::getInstancePtr();

	UndoManager::getInstance().shutdown();
	delete UndoManager::getInstancePtr();

	EditorWidgets::getInstance().shutdown();
	delete EditorWidgets::getInstancePtr();

	WidgetTypes::getInstance().shutdown();
	delete WidgetTypes::getInstancePtr();

	//delete mSettingsWindow;
	//mSettingsWindow = nullptr;

	//delete mCodeGenerator;
	//mCodeGenerator = nullptr;

	delete mWidgetsWindow;
	mWidgetsWindow = nullptr;

	delete mOpenSaveFileDialog;
	mOpenSaveFileDialog = nullptr;

	tools::HotKeyManager::getInstance().shutdown();
	delete tools::HotKeyManager::getInstancePtr();

	tools::WidgetSelectorManager::getInstance().shutdown();
	delete tools::WidgetSelectorManager::getInstancePtr();

	tools::CommandManager::getInstance().shutdown();
	delete tools::CommandManager::getInstancePtr();

	tools::SettingsManager::getInstance().shutdown();
	delete tools::SettingsManager::getInstancePtr();
}

void EditorState::injectMouseMove(int _absx, int _absy, int _absz)
{
	/*if (mTestMode)
	{
		base::BaseManager::injectMouseMove(_absx, _absy, _absz);
		return;
	}*/

	// drop select depth if we moved mouse
	const int DIST = 2;
	if ((abs(mLastClickX - _absx) > DIST) || (abs(mLastClickY - _absy) > DIST))
	{
		mSelectDepth = 0;
		mLastClickX = _absx;
		mLastClickY = _absy;
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

void EditorState::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
{
	/*if (mTestMode)
	{
		return base::BaseManager::injectMousePress(_absx, _absy, _id);
	}*/

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
		while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item)) item = item->getParent();
		MyGUI::Widget* oldItem = item;

		// try to selectin depth
		int depth = mSelectDepth;
		while (depth && (nullptr != item))
		{
			item = item->getParent();
			while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item)) item = item->getParent();
			depth--;
		}
		if (nullptr == item)
		{
			item = oldItem;
			mSelectDepth = 0;
		}

		// found widget
		if (nullptr != item)
		{
			tools::WidgetSelectorManager::getInstance().setSelectedWidget(item);

			if (mWidgetsWindow->getCreatingStatus() != 1)
			{
				//FIXME
				MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, 0);// это чтобы сразу можно было тащить
			}
		}
		//FIXME
		MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}
	else
	{
		//FIXME
		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);

		tools::WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
	}

	// вернем прямоугольник
	if (tools::WidgetSelectorManager::getInstance().getSelectedWidget() != nullptr && mWidgetsWindow->getCreatingStatus() == 0)
	{
		mPropertiesPanelView->getWidgetRectangle()->setVisible(true);
	}
	else if (mWidgetsWindow->getCreatingStatus())
	{
		mPropertiesPanelView->getWidgetRectangle()->setVisible(false);
	}

	//base::BaseManager::injectMousePress(_absx, _absy, _id);
}

void EditorState::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
{
	/*if (mTestMode)
	{
		base::BaseManager::injectMouseRelease(_absx, _absy, _id);
		return;
	}*/

	mSelectDepth++;

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

	UndoManager::getInstance().dropLastProperty();

	base::BaseManager::injectMouseRelease(_absx, _absy, _id);
}

void EditorState::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
{
	MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

	/*if (mTestMode)
	{
		if (_key == MyGUI::KeyCode::Escape)
		{
			if (input.isModalAny() == false)
			{
				notifyEndTest();
			}
		}

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
		return;
	}*/

	if (!tools::HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key))
		input.injectKeyPress(_key, _text);
	/*if (tools::Dialog::getAnyDialog())
	{
		if (_key == MyGUI::KeyCode::Escape)
			tools::Dialog::endTopDialog(false);
		else if (_key == MyGUI::KeyCode::Return)
			tools::Dialog::endTopDialog(true);
	}
	else if (_key == MyGUI::KeyCode::Escape)
	{
		notifyQuit();
		return;
	}
	else
	{
		tools::HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key);
	}

	MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);*/
}

void EditorState::injectKeyRelease(MyGUI::KeyCode _key)
{
	/*if (mTestMode)
	{
		return base::BaseManager::injectKeyRelease(_key);
	}*/

	/*MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

	if (tools::Dialog::getAnyDialog())
	{
	}
	else if (_key == MyGUI::KeyCode::Escape)
	{
	}
	else
	{
		tools::HotKeyManager::getInstance().onKeyEvent(false, input.isShiftPressed(), input.isControlPressed(), _key);
	}*/

	return base::BaseManager::injectKeyRelease(_key);
}

void EditorState::notifyFrameStarted(float _time)
{
	GroupMessage::getInstance().showMessages();

	if (mRecreate)
	{
		mRecreate = false;
		// виджет пересоздался, теперь никто незнает его адреса :)
		tools::WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
	}
}

/*void EditorState::notifyLoad()
{
	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			localise("Warning"),
			localise("Warn_unsaved_data"),
			MyGUI::MessageBoxStyle::IconWarning |
			MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No | MyGUI::MessageBoxStyle::Cancel
			);
		message->eventMessageBoxResult += newDelegate(this, &EditorState::notifyConfirmLoadMessage);
		message->setUserString("FileName", mFileName);
		return;
	}

	setModeSaveLoadDialog(false, mFileName);
}

bool EditorState::notifySave()
{
	if (mFileName != "")
	{
		return save(mFileName);
	}
	else
	{
		setModeSaveLoadDialog(true, mFileName);
		return false;
	}
}*/

/*void EditorState::notifySettings()
{
	mSettingsWindow->doModal();
	MyGUI::LayerManager::getInstance().upLayerItem(mSettingsWindow->getMainWidget());
}*/

/*void EditorState::notifyEndTest()
{
	for (MyGUI::VectorWidgetPtr::iterator iter = mInterfaceWidgets.begin(); iter != mInterfaceWidgets.end(); ++iter)
	{
		if ((*iter)->getUserString("WasVisible") == "true")
		{
			(*iter)->setVisible(true);
		}
	}
	mTestMode = false;
	clear(false);
	EditorWidgets::getInstance().loadxmlDocument(mTestLayout);
}*/

/*void EditorState::notifyClear()
{
	MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
		localise("Warning"),
		localise("Warn_delete_all_widgets"),
		MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No
		);
	message->eventMessageBoxResult += newDelegate(this, &EditorState::notifyClearMessage);
}

void EditorState::notifyClearMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes || _result == MyGUI::MessageBoxStyle::Button1)
	{
		clear();
	}
}

void EditorState::notifyQuit()
{
	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			localise("Warning"),
			localise("Warn_unsaved_data"),
			MyGUI::MessageBoxStyle::IconWarning |
			MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No | MyGUI::MessageBoxStyle::Cancel
			);
		message->eventMessageBoxResult += newDelegate(this, &EditorState::notifyConfirmQuitMessage);
		message->setUserString("FileName", mFileName);
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
}*/

/*void EditorState::clearWidgetWindow()
{
	WidgetTypes::getInstance().clearAllSkins();
	mWidgetsWindow->clearAllSheets();
}*/

/*void EditorState::loadFile(const std::wstring& _file)
{
	if (!load(true, MyGUI::UString(_file).asUTF8_c_str()))
	{
		MyGUI::ResourceManager::getInstance().load(MyGUI::UString(_file).asUTF8_c_str());
	}
}*/

/*void EditorState::notifyConfirmLoadMessage(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if ( _result == MyGUI::MessageBoxStyle::Yes )
	{
		if (notifySave())
		{
			setModeSaveLoadDialog(false, mFileName);
		}
	}
	else if ( _result == MyGUI::MessageBoxStyle::No )
	{
		setModeSaveLoadDialog(false, mFileName);
	}
}

void EditorState::notifySettingsWindowEndDialog(tools::Dialog* _dialog, bool _result)
{
	_dialog->endModal();
}

void EditorState::notifyOpenSaveEndDialog(tools::Dialog* _dialog, bool _result)
{
	if (_result)
	{
		MyGUI::UString file = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());

		if (mOpenSaveFileDialog->getMode() == "Save")
			save(file);
		else
			load(file);
	}

	mOpenSaveFileDialog->endModal();
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

	mOpenSaveFileDialog->doModal();
	mOpenSaveFileDialog->setMode(_save ? "Save" : "Load");
}*/

void EditorState::prepare()
{
	// устанавливаем локаль из переменной окружения
	// без этого не будут открываться наши файлы
	mLocale = ::setlocale( LC_ALL, "" );
	// erase everything after '_' to get language name
	mLocale.erase(std::find(mLocale.begin(), mLocale.end(), '_'), mLocale.end());
	if (mLocale == "ru") mLocale = "Russian";
	else if (mLocale == "en") mLocale = "English";

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

	// при дропе файл может быть запущен в любой дирректории
	wchar_t buff[MAX_PATH];
	::GetModuleFileNameW(0, buff, MAX_PATH);

	std::wstring dir = buff;
	size_t pos = dir.find_last_of(L"\\/");
	if (pos != dir.npos)
	{
		// устанавливаем правильную дирректорию
		::SetCurrentDirectoryW(dir.substr(0, pos+1).c_str());
	}

	// имена могут содержать пробелы, необходимо
	//склеивать и проверять файлы на существование
	std::wifstream stream;
	std::wstring tmp;
	std::wstring delims = L" ";
	std::wstring source = GetCommandLineW();
	size_t start = source.find_first_not_of(delims);
	while (start != source.npos)
	{
		size_t end = source.find_first_of(delims, start);
		if (end != source.npos)
		{
			tmp += source.substr(start, end-start);

			// имена могут быть в ковычках
			if (tmp.size() > 2)
			{
				if ((tmp[0] == L'"') && (tmp[tmp.size()-1] == L'"'))
				{
					tmp = tmp.substr(1, tmp.size()-2);
				}
			}

			stream.open(tmp.c_str());
			if (stream.is_open())
			{
				if (tmp.size() > 4 && tmp.substr(tmp.size() - 4) != L".exe")
					mParams.push_back(tmp);

				tmp.clear();
				stream.close();
			}
			else
				tmp += delims;
		}
		else
		{
			tmp += source.substr(start);

			// имена могут быть в ковычках
			if (tmp.size() > 2)
			{
				if ((tmp[0] == L'"') && (tmp[tmp.size()-1] == L'"'))
				{
					tmp = tmp.substr(1, tmp.size()-2);
				}
			}

			stream.open(tmp.c_str());
			if (stream.is_open())
			{
				if (tmp.size() > 4 && tmp.substr(tmp.size() - 4) != L".exe")
					mParams.push_back(tmp);

				tmp.clear();
				stream.close();
			}
			else
				tmp += delims;
			break;
		}
		start = source.find_first_not_of(delims, end + 1);
	};

#else
#endif
}

void EditorState::onFileDrop(const std::wstring& _fileName)
{
//	saveOrLoadLayout(false, false, _filename);
	tools::CommandManager::getInstance().setCommandData(_fileName);
	tools::CommandManager::getInstance().executeCommand("Command_FileDrop");
}

bool EditorState::onWinodwClose(size_t _handle)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (::IsIconic((HWND)_handle))
		ShowWindow((HWND)_handle, SW_SHOWNORMAL);
#endif

	tools::CommandManager::getInstance().executeCommand("Command_QuitApp");
	return false;
}

int EditorState::toGrid(int _x)
{
	return _x / grid_step * grid_step;
}

void EditorState::notifyRecreate()
{
	mRecreate = true;
}

/*void EditorState::commandLoad(const MyGUI::UString& _commandName)
{
	notifyLoad();
}

void EditorState::commandSave(const MyGUI::UString& _commandName)
{
	notifySave();
}

void EditorState::commandSaveAs(const MyGUI::UString& _commandName)
{
	setModeSaveLoadDialog(true, mFileName);
}

void EditorState::commandClear(const MyGUI::UString& _commandName)
{
	notifyClear();
}*/

/*void EditorState::commandTest(const MyGUI::UString& _commandName)
{
	mTestLayout = EditorWidgets::getInstance().savexmlDocument();
	EditorWidgets::getInstance().clear();
	tools::WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

	for (MyGUI::VectorWidgetPtr::iterator iter = mInterfaceWidgets.begin(); iter != mInterfaceWidgets.end(); ++iter)
	{
		if ((*iter)->getVisible())
		{
			(*iter)->setUserString("WasVisible", "true");
			(*iter)->setVisible(false);
		}
	}

	EditorWidgets::getInstance().loadxmlDocument(mTestLayout, true);
	mTestMode = true;
}*/

/*void EditorState::commandQuit(const MyGUI::UString& _commandName)
{
	notifyQuit();
}*/

/*void EditorState::commandSettings(const MyGUI::UString& _commandName)
{
	notifySettings();
}

void EditorState::commandCodeGenerator(const MyGUI::UString& _commandName)
{
	mCodeGenerator->getMainWidget()->setVisible(true);
}*/

/*void EditorState::commandRecentFiles(const MyGUI::UString& _commandName)
{
	load(tools::CommandManager::getInstance().getCommandData());
}

void EditorState::commandStatisticInfo(const MyGUI::UString& _commandName)
{
	getStatisticInfo()->setVisible(!getStatisticInfo()->getVisible());
}

void EditorState::commandFocusVisible(const MyGUI::UString& _commandName)
{
	getFocusInput()->setFocusVisible(!getFocusInput()->getFocusVisible());
}*/

void EditorState::commandQuitApp(const MyGUI::UString& _commandName)
{
	if (tools::DialogManager::getInstance().getAnyDialog())
	{
		tools::DialogManager::getInstance().endTopDialog();
	}
	else
	{
		if (tools::MessageBoxManager::getInstance().hasAny())
		{
			tools::MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
		}
		else
		{
			tools::CommandManager::getInstance().executeCommand("Command_Quit");
		}
	}
}

void EditorState::commandLoad(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			tools::replaceTags("Warning"),
			tools::replaceTags("MessageUnsavedData"),
			MyGUI::MessageBoxStyle::IconQuest
				| MyGUI::MessageBoxStyle::Yes
				| MyGUI::MessageBoxStyle::No
				| MyGUI::MessageBoxStyle::Cancel);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultLoad);
	}
	else
	{
		showLoadWindow();
	}
}

void EditorState::commandSave(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	if (UndoManager::getInstance().isUnsaved())
	{
		save();
	}
}

void EditorState::commandSaveAs(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	showSaveAsWindow();
}

void EditorState::commandClear(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			tools::replaceTags("Warning"),
			tools::replaceTags("MessageUnsavedData"),
			MyGUI::MessageBoxStyle::IconQuest
				| MyGUI::MessageBoxStyle::Yes
				| MyGUI::MessageBoxStyle::No
				| MyGUI::MessageBoxStyle::Cancel);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultClear);
	}
	else
	{
		clear();
	}
}

void EditorState::commandQuit(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			tools::replaceTags("Warning"),
			tools::replaceTags("MessageUnsavedData"),
			MyGUI::MessageBoxStyle::IconQuest
				| MyGUI::MessageBoxStyle::Yes
				| MyGUI::MessageBoxStyle::No
				| MyGUI::MessageBoxStyle::Cancel);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultQuit);
	}
	else
	{
		//StateManager::getInstance().stateEvent(this, "Exit");
		quit();
	}
}

void EditorState::commandFileDrop(const MyGUI::UString& _commandName)
{
	if (!checkCommand())
		return;

	mDropFileName = tools::CommandManager::getInstance().getCommandData();
	if (mDropFileName.empty())
		return;

	if (UndoManager::getInstance().isUnsaved())
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			tools::replaceTags("Warning"),
			tools::replaceTags("MessageUnsavedData"),
			MyGUI::MessageBoxStyle::IconQuest
				| MyGUI::MessageBoxStyle::Yes
				| MyGUI::MessageBoxStyle::No
				| MyGUI::MessageBoxStyle::Cancel);
		message->eventMessageBoxResult += MyGUI::newDelegate(this, &EditorState::notifyMessageBoxResultLoadDropFile);
	}
	else
	{
		loadDropFile();
	}
}

void EditorState::clear()
{
	mWidgetsWindow->clearNewWidget();
	mRecreate = false;
	//mFileName = "";
	EditorWidgets::getInstance().clear();

	tools::WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

	UndoManager::getInstance().shutdown();
	UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());
	mSelectDepth = 0;

	//setWindowCaption(L"MyGUI Layout Editor");
	mFileName = mDefaultFileName;
	tools::addUserTag("CurrentFileName", mFileName);

	updateCaption();
}

void EditorState::load(/*const MyGUI::UString& _file*/)
{
	//clear();

	if (EditorWidgets::getInstance().load(mFileName))
	{
		//mFileName = _file;
		//setWindowCaption(_file.asWStr() + L" - MyGUI Layout Editor");
		//tools::SettingsManager::getInstance().addRecentFile(_file);

		UndoManager::getInstance().addValue();
		UndoManager::getInstance().setUnsaved(false);
	}
	else
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			localise("Warning"),
			"Failed to " + localise("Load") + " file '" + mFileName + "'",
			MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok
			);

		mFileName = mDefaultFileName;
		tools::addUserTag("CurrentFileName", mFileName);

		updateCaption();
	}
}

void EditorState::save(/*const MyGUI::UString& _file*/)
{
	if (EditorWidgets::getInstance().save(mFileName))
	{
		//mFileName = _file;
		//setWindowCaption(_file.asWStr() + L" - MyGUI Layout Editor");
		//tools::SettingsManager::getInstance().addRecentFile(_file);

		UndoManager::getInstance().addValue();
		UndoManager::getInstance().setUnsaved(false);
		//return true;
	}
	/*else
	{
		MyGUI::Message* message = tools::MessageBoxManager::getInstance().create(
			localise("Warning"),
			"Failed to " + localise("Save") + " file '" + _file + "'",
			MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok
			);
	}*/

	//return false;
}

void EditorState::updateCaption()
{
	tools::addUserTag("HasChanged", UndoManager::getInstance().isUnsaved() ? "*" : "");
	setCaption(tools::replaceTags("CaptionMainWindow"));
}

void EditorState::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes)
	{
		save();
		clear();

		showLoadWindow();
	}
	else if (_result == MyGUI::MessageBoxStyle::No)
	{
		clear();

		showLoadWindow();
	}
}

void EditorState::notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes)
	{
		save();
		clear();

		loadDropFile();
	}
	else if (_result == MyGUI::MessageBoxStyle::No)
	{
		clear();

		loadDropFile();
	}
}

void EditorState::loadDropFile()
{
	mFileName = mDropFileName;
	tools::addUserTag("CurrentFileName", mFileName);

	load();
	updateCaption();
}

void EditorState::showLoadWindow()
{
	mOpenSaveFileDialog->setDialogInfo(tools::replaceTags("CaptionOpenFile"), tools::replaceTags("ButtonOpenFile"));
	mOpenSaveFileDialog->setMode("Load");
	mOpenSaveFileDialog->doModal();
}

void EditorState::notifyEndDialog(tools::Dialog* _sender, bool _result)
{
	if (_result)
	{
		if (mOpenSaveFileDialog->getMode() == "SaveAs")
		{
			mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
			tools::addUserTag("CurrentFileName", mFileName);

			save();
			updateCaption();
		}
		else if (mOpenSaveFileDialog->getMode() == "Load")
		{
			mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
			tools::addUserTag("CurrentFileName", mFileName);

			load();
			updateCaption();
		}
	}

	mOpenSaveFileDialog->endModal();
}

void EditorState::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes)
	{
		save();
		clear();
	}
	else if (_result == MyGUI::MessageBoxStyle::No)
	{
		clear();
	}
}

void EditorState::showSaveAsWindow()
{
	mOpenSaveFileDialog->setDialogInfo(tools::replaceTags("CaptionSaveFile"), tools::replaceTags("ButtonSaveFile"));
	mOpenSaveFileDialog->setMode("SaveAs");
	mOpenSaveFileDialog->doModal();
}

void EditorState::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
{
	if (_result == MyGUI::MessageBoxStyle::Yes)
	{
		save();
		//StateManager::getInstance().stateEvent(this, "Exit");
		quit();
	}
	else if (_result == MyGUI::MessageBoxStyle::No)
	{
		//StateManager::getInstance().stateEvent(this, "Exit");
		quit();
	}
}

bool EditorState::checkCommand()
{
	if (tools::DialogManager::getInstance().getAnyDialog())
		return false;

	if (tools::MessageBoxManager::getInstance().hasAny())
		return false;

	//if (!StateManager::getInstance().getStateActivate(this))
	//	return false;

	return true;
}

void EditorState::notifyChanges(bool _changes)
{
	updateCaption();
}

void EditorState::setCaption(const MyGUI::UString& _value)
{
	setWindowCaption(_value);
}

MYGUI_APP(EditorState)
