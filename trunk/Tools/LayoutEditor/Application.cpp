#include "Precompiled.h"
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
#include "Tools/DialogManager.h"
#include "StateManager.h"
#include "Localise.h"
#include "WidgetCreatorManager.h"
#include "RecentFilesManager.h"
#include "BackwardCompatibilityManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "ColourManager.h"

template <> tools::Application* MyGUI::Singleton<tools::Application>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::Application>::mClassTypeName("Application");

namespace tools
{
	Application::Application() :
		mEditorState(nullptr),
		mTestState(nullptr)
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
		addResourceLocation(getRootMedia() + "/Common/Tools");
		setResourceFilename("Editor.xml");
	}

	void Application::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		MyGUI::ResourceManager::getInstance().load("EditorLayer.xml");
		MyGUI::ResourceManager::getInstance().load("EditorSettings.xml");

		getStatisticInfo()->setVisible(false);

		// set locale language if it was taken from OS
		if (!mLocale.empty())
			MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);
		// if you want to test LanguageManager uncomment next line
		//MyGUI::LanguageManager::getInstance().setCurrentLanguage("Russian");
		//MyGUI::LanguageManager::getInstance().setCurrentLanguage("English");

		new SettingsManager();
		SettingsManager::getInstance().initialise("le_user_settings.xml");

		new CommandManager();
		CommandManager::getInstance().initialise();

		new WidgetSelectorManager();
		WidgetSelectorManager::getInstance().initialise();

		new WidgetCreatorManager();
		WidgetCreatorManager::getInstance().initialise();

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

		new WidgetTypes();
		WidgetTypes::getInstance().initialise();

		new EditorWidgets();
		EditorWidgets::getInstance().initialise();

		new UndoManager();
		UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());

		new StateManager();
		StateManager::getInstance().initialise();

		new GroupMessage();
		GroupMessage::getInstance().initialise();

		new RecentFilesManager();
		RecentFilesManager::getInstance().initialise();

		new BackwardCompatibilityManager();
		BackwardCompatibilityManager::getInstance().initialise();

		new ColourManager();
		ColourManager::getInstance().initialise();

		MyGUI::ResourceManager::getInstance().load("Initialise.xml");

		const SettingsSector::VectorUString& additionalPaths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalPaths");
		for (SettingsSector::VectorUString::const_iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
			addResourceLocation(*iter);

		const SettingsSector::VectorUString& additionalResources = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalResources");
		for (SettingsSector::VectorUString::const_iterator iter = additionalResources.begin(); iter != additionalResources.end(); ++iter)
			MyGUI::ResourceManager::getInstance().load(*iter);

		CommandManager::getInstance().registerCommand("Command_StatisticInfo", MyGUI::newDelegate(this, &Application::command_StatisticInfo));
		CommandManager::getInstance().registerCommand("Command_FocusVisible", MyGUI::newDelegate(this, &Application::command_FocusVisible));
		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &Application::command_QuitApp));
		CommandManager::getInstance().registerCommand("Command_UpdateAppCaption", MyGUI::newDelegate(this, &Application::command_UpdateAppCaption));

		mEditorState = new EditorState();
		mTestState = new TestState();

		StateManager::getInstance().registerState(this, "Application");
		StateManager::getInstance().registerState(mEditorState, "EditorState");
		StateManager::getInstance().registerState(mTestState, "TestState");

		StateManager::getInstance().registerEventState("Application", "Start", "EditorState");
		StateManager::getInstance().registerEventState("EditorState", "Test", "TestState");
		StateManager::getInstance().registerEventState("EditorState", "Exit", "Application");
		StateManager::getInstance().registerEventState("TestState", "Exit", "EditorState");

		StateManager::getInstance().pushState(this);
		StateManager::getInstance().stateEvent(this, "Start");
	}

	void Application::destroyScene()
	{
		StateManager::getInstance().rollbackToState(nullptr);

		delete mEditorState;
		mEditorState = nullptr;

		delete mTestState;
		mTestState = nullptr;

		ColourManager::getInstance().shutdown();
		delete ColourManager::getInstancePtr();

		BackwardCompatibilityManager::getInstance().shutdown();
		delete BackwardCompatibilityManager::getInstancePtr();

		RecentFilesManager::getInstance().shutdown();
		delete RecentFilesManager::getInstancePtr();

		StateManager::getInstance().shutdown();
		delete StateManager::getInstancePtr();

		GroupMessage::getInstance().shutdown();
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

		HotKeyManager::getInstance().shutdown();
		delete HotKeyManager::getInstancePtr();

		WidgetCreatorManager::getInstance().shutdown();
		delete WidgetCreatorManager::getInstancePtr();

		WidgetSelectorManager::getInstance().shutdown();
		delete WidgetSelectorManager::getInstancePtr();

		CommandManager::getInstance().shutdown();
		delete CommandManager::getInstancePtr();

		SettingsManager::getInstance().shutdown();
		delete SettingsManager::getInstancePtr();

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");
	}

	void Application::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

		if (!HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key))
			input.injectKeyPress(_key, _text);
	}

	void Application::injectKeyRelease(MyGUI::KeyCode _key)
	{
		return base::BaseManager::injectKeyRelease(_key);
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
			::SetCurrentDirectoryW(dir.substr(0, pos + 1).c_str());
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
				tmp += source.substr(start, end - start);

				// имена могут быть в ковычках
				if (tmp.size() > 2)
				{
					if ((tmp[0] == L'"') && (tmp[tmp.size()-1] == L'"'))
					{
						tmp = tmp.substr(1, tmp.size() - 2);
					}
				}

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && !defined(STLPORT)
				stream.open(tmp.c_str());
#else
				stream.open(MyGUI::UString(tmp).asUTF8_c_str());
#endif
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
						tmp = tmp.substr(1, tmp.size() - 2);
					}
				}

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && !defined(STLPORT)
				stream.open(tmp.c_str());
#else
				stream.open(MyGUI::UString(tmp).asUTF8_c_str());
#endif
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

	void Application::command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result)
	{
		getStatisticInfo()->setVisible(!getStatisticInfo()->getVisible());

		_result = true;
	}

	void Application::command_FocusVisible(const MyGUI::UString& _commandName, bool& _result)
	{
		getFocusInput()->setFocusVisible(!getFocusInput()->getFocusVisible());

		_result = true;
	}

	void Application::command_QuitApp(const MyGUI::UString& _commandName, bool& _result)
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

		_result = true;
	}

	void Application::command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result)
	{
		setWindowCaption(replaceTags("CaptionMainWindow"));

		_result = true;
	}

	void Application::resumeState()
	{
		quit();
	}

	const Application::VectorWString& Application::getParams()
	{
		return mParams;
	}

} // namespace tools

MYGUI_APP(tools::Application)
