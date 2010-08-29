#include "precompiled.h"
#include "Common.h"
#include "Application.h"
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
#include "Localise.h"

namespace tools
{
	const int BAR_HEIGHT = 30;

	Application::Application() :
		mLastClickX(0),
		mLastClickY(0),
		mSelectDepth(0),
		mRecreate(false),
		//mTestMode(false),
		mPropertiesPanelView(nullptr),
		mSettingsWindow(nullptr),
		mWidgetsWindow(nullptr),
		mCodeGenerator(nullptr),
		mOpenSaveFileDialog(nullptr),
		mMainMenuControl(nullptr),
		mFileName("unnamed.xml"),
		mDefaultFileName("unnamed.xml"),
		mMessageBoxFadeControl(nullptr),
		mGridStep(0)
	{
	}

	Application::~Application()
	{
	}

	void Application::setupResources()
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

	void Application::createScene()
	{
		getStatisticInfo()->setVisible(false);

		new SettingsManager();
		SettingsManager::getInstance().initialise();

		new CommandManager();
		CommandManager::getInstance().initialise();

		new WidgetSelectorManager();
		WidgetSelectorManager::getInstance().initialise();

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

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
		mSettingsWindow = new SettingsWindow();
		mSettingsWindow->eventEndDialog = MyGUI::newDelegate(this, &Application::notifySettingsWindowEndDialog);
		//mInterfaceWidgets.push_back(mSettingsWindow->getMainWidget());

		// properties panelView
		mPropertiesPanelView = new PropertiesPanelView();
		mPropertiesPanelView->eventRecreate = MyGUI::newDelegate(this, &Application::notifyRecreate);
		mInterfaceWidgets.push_back(mPropertiesPanelView->getMainWidget());

		mWidgetsWindow = new WidgetsWindow();
		mInterfaceWidgets.push_back(mWidgetsWindow->getMainWidget());

		mCodeGenerator = new CodeGenerator();
		mCodeGenerator->eventEndDialog = MyGUI::newDelegate(this, &Application::notifyEndDialogCodeGenerator);
		//mInterfaceWidgets.push_back(mCodeGenerator->getMainWidget());

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->setFileMask("*.layout");
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &Application::notifyEndDialogOpenSaveFile);

		mMainMenuControl = new MainMenuControl();
		mInterfaceWidgets.push_back(mMainMenuControl->getMainWidget());

		mMessageBoxFadeControl = new MessageBoxFadeControl();

		MyGUI::Widget* widget = mPropertiesPanelView->getMainWidget();
		widget->setCoord(
			widget->getParentSize().width - widget->getSize().width,
			BAR_HEIGHT,
			widget->getSize().width,
			widget->getParentSize().height - BAR_HEIGHT
			);

		// после загрузки настроек инициализируем
		mWidgetsWindow->initialise();

		const VectorUString& additionalPaths = SettingsManager::getInstance().getAdditionalPaths();
		for (VectorUString::const_iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
			addResourceLocation(*iter);

		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &Application::commandLoad));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &Application::commandSave));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &Application::commandSaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &Application::commandClear));
		//CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &Application::commandTest));
		CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &Application::commandQuit));
		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &Application::commandQuitApp));
		CommandManager::getInstance().registerCommand("Command_Settings", MyGUI::newDelegate(this, &Application::commandSettings));
		CommandManager::getInstance().registerCommand("Command_CodeGenerator", MyGUI::newDelegate(this, &Application::commandCodeGenerator));
		CommandManager::getInstance().registerCommand("Command_RecentFiles", MyGUI::newDelegate(this, &Application::commandRecentFiles));
		CommandManager::getInstance().registerCommand("Command_StatisticInfo", MyGUI::newDelegate(this, &Application::commandStatisticInfo));
		CommandManager::getInstance().registerCommand("Command_FocusVisible", MyGUI::newDelegate(this, &Application::commandFocusVisible));
		CommandManager::getInstance().registerCommand("Command_FileDrop", MyGUI::newDelegate(this, &Application::commandFileDrop));

		updateCaption();

		if (!getParams().empty())
		{
			mFileName = getParams().front();
			addUserTag("CurrentFileName", mFileName);

			load();
			updateCaption();
		}

		mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &Application::notifySettingsChanged);
		UndoManager::getInstance().eventChanges += MyGUI::newDelegate(this, &Application::notifyChanges);
		getGUI()->eventFrameStart += MyGUI::newDelegate(this, &Application::notifyFrameStarted);
	}

	void Application::destroyScene()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &Application::notifySettingsChanged);
		UndoManager::getInstance().eventChanges -= MyGUI::newDelegate(this, &Application::notifyChanges);
		getGUI()->eventFrameStart -= MyGUI::newDelegate(this, &Application::notifyFrameStarted);

		delete mMessageBoxFadeControl;
		mMessageBoxFadeControl = nullptr;

		delete mMainMenuControl;
		mMainMenuControl = nullptr;

		delete mPropertiesPanelView;
		mPropertiesPanelView = nullptr;

		delete GroupMessage::getInstancePtr();

		DialogManager::getInstance().shutdown();
		delete DialogManager::getInstancePtr();

		MessageBoxManager::getInstance().shutdown();
		delete MessageBoxManager::getInstancePtr();

		UndoManager::getInstance().shutdown();
		delete UndoManager::getInstancePtr();

		EditorWidgets::getInstance().shutdown();
		delete EditorWidgets::getInstancePtr();

		WidgetTypes::getInstance().shutdown();
		delete WidgetTypes::getInstancePtr();

		delete mSettingsWindow;
		mSettingsWindow = nullptr;

		delete mCodeGenerator;
		mCodeGenerator = nullptr;

		delete mWidgetsWindow;
		mWidgetsWindow = nullptr;

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		HotKeyManager::getInstance().shutdown();
		delete HotKeyManager::getInstancePtr();

		WidgetSelectorManager::getInstance().shutdown();
		delete WidgetSelectorManager::getInstancePtr();

		CommandManager::getInstance().shutdown();
		delete CommandManager::getInstancePtr();

		SettingsManager::getInstance().shutdown();
		delete SettingsManager::getInstancePtr();
	}

	void Application::injectMouseMove(int _absx, int _absy, int _absz)
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

	void Application::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
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
				WidgetSelectorManager::getInstance().setSelectedWidget(item);

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

			WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
		}

		// вернем прямоугольник
		if (WidgetSelectorManager::getInstance().getSelectedWidget() != nullptr && mWidgetsWindow->getCreatingStatus() == 0)
		{
			mPropertiesPanelView->getWidgetRectangle()->setVisible(true);
		}
		else if (mWidgetsWindow->getCreatingStatus())
		{
			mPropertiesPanelView->getWidgetRectangle()->setVisible(false);
		}

		//base::BaseManager::injectMousePress(_absx, _absy, _id);
	}

	void Application::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
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

	void Application::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
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

		if (!HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key))
			input.injectKeyPress(_key, _text);
	}

	void Application::injectKeyRelease(MyGUI::KeyCode _key)
	{
		/*if (mTestMode)
		{
			return base::BaseManager::injectKeyRelease(_key);
		}*/

		return base::BaseManager::injectKeyRelease(_key);
	}

	void Application::notifyFrameStarted(float _time)
	{
		GroupMessage::getInstance().showMessages();

		if (mRecreate)
		{
			mRecreate = false;
			// виджет пересоздался, теперь никто незнает его адреса :)
			WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
		}
	}

	/*void Application::notifyEndTest()
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

	void Application::notifySettingsWindowEndDialog(Dialog* _dialog, bool _result)
	{
		MYGUI_ASSERT(mSettingsWindow == _dialog, "mSettingsWindow == _sender");

		if (_result)
		{
			mSettingsWindow->saveSettings();
		}

		mSettingsWindow->endModal();
	}

	void Application::prepare()
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

	void Application::onFileDrop(const std::wstring& _fileName)
	{
		CommandManager::getInstance().setCommandData(_fileName);
		CommandManager::getInstance().executeCommand("Command_FileDrop");
	}

	bool Application::onWinodwClose(size_t _handle)
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (::IsIconic((HWND)_handle))
			ShowWindow((HWND)_handle, SW_SHOWNORMAL);
	#endif

		CommandManager::getInstance().executeCommand("Command_QuitApp");
		return false;
	}

	int Application::toGrid(int _x)
	{
		if (mGridStep < 1)
			return _x;
		return _x / mGridStep * mGridStep;
	}

	void Application::notifyRecreate()
	{
		mRecreate = true;
	}

	/*void Application::commandTest(const MyGUI::UString& _commandName)
	{
		mTestLayout = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

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

	void Application::commandSettings(const MyGUI::UString& _commandName)
	{
		mSettingsWindow->doModal();
	}

	void Application::commandCodeGenerator(const MyGUI::UString& _commandName)
	{
		mCodeGenerator->loadTemplate();
		mCodeGenerator->doModal();
	}

	void Application::commandRecentFiles(const MyGUI::UString& _commandName)
	{
		commandFileDrop(_commandName);
	}

	void Application::commandStatisticInfo(const MyGUI::UString& _commandName)
	{
		getStatisticInfo()->setVisible(!getStatisticInfo()->getVisible());
	}

	void Application::commandFocusVisible(const MyGUI::UString& _commandName)
	{
		getFocusInput()->setFocusVisible(!getFocusInput()->getFocusVisible());
	}

	void Application::commandQuitApp(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
		{
			DialogManager::getInstance().endTopDialog();
		}
		else
		{
			if (MessageBoxManager::getInstance().hasAny())
			{
				MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
			}
			else
			{
				CommandManager::getInstance().executeCommand("Command_Quit");
			}
		}
	}

	void Application::commandLoad(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &Application::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}
	}

	void Application::commandSave(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			save();
		}
	}

	void Application::commandSaveAs(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		showSaveAsWindow();
	}

	void Application::commandClear(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &Application::notifyMessageBoxResultClear);
		}
		else
		{
			clear();
		}
	}

	void Application::commandQuit(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &Application::notifyMessageBoxResultQuit);
		}
		else
		{
			//StateManager::getInstance().stateEvent(this, "Exit");
			quit();
		}
	}

	void Application::commandFileDrop(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mDropFileName = CommandManager::getInstance().getCommandData();
		if (mDropFileName.empty())
			return;

		if (UndoManager::getInstance().isUnsaved())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &Application::notifyMessageBoxResultLoadDropFile);
		}
		else
		{
			clear();

			loadDropFile();
		}
	}

	void Application::clear()
	{
		mWidgetsWindow->clearNewWidget();
		mRecreate = false;
		EditorWidgets::getInstance().clear();

		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		UndoManager::getInstance().shutdown();
		UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());
		mSelectDepth = 0;

		mFileName = mDefaultFileName;
		addUserTag("CurrentFileName", mFileName);

		updateCaption();
	}

	void Application::load()
	{
		if (EditorWidgets::getInstance().load(mFileName))
		{
			SettingsManager::getInstance().addRecentFile(mFileName);

			UndoManager::getInstance().addValue();
			UndoManager::getInstance().setUnsaved(false);
		}
		else
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageFailedLoadFile"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
				);

			mFileName = mDefaultFileName;
			addUserTag("CurrentFileName", mFileName);

			updateCaption();
		}
	}

	bool Application::save()
	{
		if (EditorWidgets::getInstance().save(mFileName))
		{
			SettingsManager::getInstance().addRecentFile(mFileName);

			UndoManager::getInstance().addValue();
			UndoManager::getInstance().setUnsaved(false);
			return true;
		}
		else
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				replaceTags("MessageFailedSaveFile"),
				MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok
				);
		}
		return false;
	}

	void Application::updateCaption()
	{
		addUserTag("HasChanged", UndoManager::getInstance().isUnsaved() ? "*" : "");
		setCaption(replaceTags("CaptionMainWindow"));
	}

	void Application::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();

				showLoadWindow();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();

			showLoadWindow();
		}
	}

	void Application::notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();

				loadDropFile();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();

			loadDropFile();
		}
	}

	void Application::loadDropFile()
	{
		mFileName = mDropFileName;
		addUserTag("CurrentFileName", mFileName);

		load();
		updateCaption();
	}

	void Application::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFile"), replaceTags("ButtonOpenFile"));
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->doModal();
	}

	void Application::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("CurrentFileName", mFileName);

				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("CurrentFileName", mFileName);

				load();
				updateCaption();
			}
		}

		mOpenSaveFileDialog->endModal();
	}

	void Application::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				clear();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			clear();
		}
	}

	void Application::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionSaveFile"), replaceTags("ButtonSaveFile"));
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->doModal();
	}

	void Application::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			if (save())
			{
				//StateManager::getInstance().stateEvent(this, "Exit");
				quit();
			}
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			//StateManager::getInstance().stateEvent(this, "Exit");
			quit();
		}
	}

	bool Application::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		//if (!StateManager::getInstance().getStateActivate(this))
		//	return false;

		return true;
	}

	void Application::notifyChanges(bool _changes)
	{
		updateCaption();
	}

	void Application::setCaption(const MyGUI::UString& _value)
	{
		setWindowCaption(_value);
	}

	void Application::notifyEndDialogCodeGenerator(Dialog* _dialog, bool _result)
	{
		mCodeGenerator->endModal();
		if (_result)
			mCodeGenerator->saveTemplate();
	}

	void Application::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "SettingsWindow")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("SettingsWindow")->getPropertyValue<int>("Grid");
		}
	}

} // namespace tools

MYGUI_APP(tools::Application)
