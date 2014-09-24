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
#include "DialogManager.h"
#include "StateManager.h"
#include "Localise.h"
#include "WidgetCreatorManager.h"
#include "RecentFilesManager.h"
#include "BackwardCompatibilityManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "ColourManager.h"
#include "EditorToolTip.h"
#include "PropertyFieldManager.h"
#include "GridManager.h"
#include "FactoryManager.h"
#include "ComponentFactory.h"

template <> tools::Application* MyGUI::Singleton<tools::Application>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::Application>::mClassTypeName = "Application";

namespace tools
{

	Application::Application()
	{
		ComponentFactory::Initialise();
	}

	Application::~Application()
	{
		ComponentFactory::Shutdown();
	}

	void Application::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/EditorFramework");
		addResourceLocation(getRootMedia() + "/Tools/LayoutEditor");
		addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Panels");
		addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Themes");
		addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/Settings");
		addResourceLocation(getRootMedia() + "/Tools/LayoutEditor/CodeTemplates");
		addResourceLocation(getRootMedia() + "/Common/Base");
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia() + "/Common/Tools");
		addResourceLocation(getRootMedia() + "/Common/MessageBox");
		addResourceLocation(getRootMedia() + "/Common/Themes");
		setResourceFilename("");
	}

	void Application::createScene()
	{
		if (getStatisticInfo() != nullptr)
			getStatisticInfo()->setVisible(false);

		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>(layerCategory);

		std::string subWidgetCategory = MyGUI::SubWidgetManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>(subWidgetCategory);

		new SettingsManager();
		SettingsManager::getInstance().loadSettingsFile(MyGUI::DataManager::getInstance().getDataPath("Settings.xml"));
		std::string userSettingsFileName = SettingsManager::getInstance().getValue("Editor/UserSettingsFileName");
		if (!userSettingsFileName.empty())
			SettingsManager::getInstance().loadUserSettingsFile(userSettingsFileName);

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		std::string language = SettingsManager::getInstance().getValue("Settings/InterfaceLanguage");
		if (language.empty() || language == "Auto")
		{
			if (!mLocale.empty())
				MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);
		}
		else
		{
			MyGUI::LanguageManager::getInstance().setCurrentLanguage(language);
		}

		new CommandManager();
		CommandManager::getInstance().initialise();

		new WidgetSelectorManager();
		WidgetSelectorManager::getInstance().initialise();

		new WidgetCreatorManager();
		WidgetCreatorManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

		new WidgetTypes();
		WidgetTypes::getInstance().initialise();

		new EditorWidgets();
		EditorWidgets::getInstance().initialise();

		new BackwardCompatibilityManager();
		BackwardCompatibilityManager::getInstance().initialise();

		new UndoManager();
		UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());

		new StateManager();
		StateManager::getInstance().initialise();

		new GroupMessage();
		GroupMessage::getInstance().initialise();

		new RecentFilesManager();
		RecentFilesManager::getInstance().initialise();

		new ColourManager();
		ColourManager::getInstance().initialise();

		new EditorToolTip();
		EditorToolTip::getInstance().initialise();

		new PropertyFieldManager();
		PropertyFieldManager::getInstance().initialise();

		new GridManager();
		GridManager::getInstance().initialise();

		MyGUI::ResourceManager::getInstance().load("Initialise.xml");

		LoadGuiSettings();

		bool maximized = SettingsManager::getInstance().getValue<bool>("Controls/Main/Maximized");
		setWindowMaximized(maximized);
		if (!maximized)
		{
			MyGUI::IntCoord windowCoord = SettingsManager::getInstance().getValue<MyGUI::IntCoord>("Controls/Main/Coord");
			setWindowCoord(windowCoord);
		}

		CommandManager::getInstance().getEvent("Command_StatisticInfo")->connect(this, &Application::command_StatisticInfo);
		CommandManager::getInstance().getEvent("Command_FocusVisible")->connect(this, &Application::command_FocusVisible);
		CommandManager::getInstance().getEvent("Command_ScreenShot")->connect(this, &Application::command_ScreenShot);
		CommandManager::getInstance().getEvent("Command_QuitApp")->connect(this, &Application::command_QuitApp);
		CommandManager::getInstance().getEvent("Command_UpdateAppCaption")->connect(this, &Application::command_UpdateAppCaption);

		CreateControls();
		LoadStates();
	}

	void Application::destroyScene()
	{
		saveSettings();

		StateManager::getInstance().rollbackToState(nullptr);

		DestroyControls();

		GridManager::getInstance().shutdown();
		delete GridManager::getInstancePtr();

		PropertyFieldManager::getInstance().shutdown();
		delete PropertyFieldManager::getInstancePtr();

		EditorToolTip::getInstance().shutdown();
		delete EditorToolTip::getInstancePtr();

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

		SettingsManager::getInstance().saveSettingsFile("SettingsResult.xml");
		SettingsManager::getInstance().saveUserSettingsFile();
		delete SettingsManager::getInstancePtr();

		std::string subWidgetCategory = MyGUI::SubWidgetManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>(subWidgetCategory);

		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>(layerCategory);
	}

	void Application::setWindowMaximized(bool _value)
	{
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (_value)
		{
			size_t handle = getWindowHandle();
			::ShowWindow((HWND)handle, SW_SHOWMAXIMIZED);
		}
	#endif
	}

	bool Application::getWindowMaximized()
	{
		bool result = false;
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t handle = getWindowHandle();
		result = ::IsZoomed((HWND)handle) != 0;
	#endif
		return result;
	}

	void Application::setWindowCoord(const MyGUI::IntCoord& _value)
	{
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (_value.empty())
			return;

		MyGUI::IntCoord coord = _value;
		if (coord.left < 0)
			coord.left = 0;
		if (coord.top < 0)
			coord.top = 0;
		if (coord.width < 640)
			coord.width = 640;
		if (coord.height < 480)
			coord.height = 480;
		if (coord.width > GetSystemMetrics(SM_CXSCREEN))
			coord.width = GetSystemMetrics(SM_CXSCREEN);
		if (coord.height > GetSystemMetrics(SM_CYSCREEN))
			coord.height = GetSystemMetrics(SM_CYSCREEN);
		if (coord.right() > GetSystemMetrics(SM_CXSCREEN))
			coord.left = GetSystemMetrics(SM_CXSCREEN) - coord.width;
		if (coord.bottom() > GetSystemMetrics(SM_CYSCREEN))
			coord.top = GetSystemMetrics(SM_CYSCREEN) - coord.height;

		size_t handle = getWindowHandle();
		::MoveWindow((HWND)handle, coord.left, coord.top, coord.width, coord.height, true);
	#endif
	}

	MyGUI::IntCoord Application::getWindowCoord()
	{
		MyGUI::IntCoord result;
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t handle = getWindowHandle();
		::RECT rect;
		::GetWindowRect((HWND)handle, &rect);
		result.left = rect.left;
		result.top = rect.top;
		result.width = rect.right - rect.left;
		result.height = rect.bottom - rect.top;
	#endif
		return result;
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

	void Application::command_ScreenShot(const MyGUI::UString& _commandName, bool& _result)
	{
		makeScreenShot();

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

	const Application::VectorWString& Application::getParams()
	{
		return mParams;
	}

	void Application::saveSettings()
	{
		SettingsManager::getInstance().setValue("Controls/Main/Maximized", getWindowMaximized());
		SettingsManager::getInstance().setValue("Controls/Main/Coord", getWindowCoord());
	}

	void Application::LoadStates()
	{
		SettingsManager::VectorString values = SettingsManager::getInstance().getValueList("Editor/States/State.List");
		for (SettingsManager::VectorString::const_iterator value = values.begin(); value != values.end(); value ++)
		{
			StateController* state = components::FactoryManager::GetInstance().CreateItem<StateController>(*value);
			if (state != nullptr)
				StateManager::getInstance().registerState(state, *value);
		}

		pugi::xpath_node_set events = SettingsManager::getInstance().getValueNodeList("Editor/States/Event.List");
		for (pugi::xpath_node_set::const_iterator event = events.begin(); event != events.end(); event ++)
		{
			StateManager::getInstance().registerEventState(
				(*event).node().child("From").child_value(),
				(*event).node().child("Name").child_value(),
				(*event).node().child("To").child_value());
		}

		std::string firstState = SettingsManager::getInstance().getValue("Editor/States/FirstState/Name");
		StateManager::getInstance().pushState(firstState);

		std::string firstEvent = SettingsManager::getInstance().getValue("Editor/States/FirstState/Event");
		StateManager::getInstance().stateEvent(firstState, firstEvent);
	}

	void Application::LoadGuiSettings()
	{
		const SettingsManager::VectorString& resources = SettingsManager::getInstance().getValueList("Resources/Resource.List");
		for (SettingsManager::VectorString::const_iterator iter = resources.begin(); iter != resources.end(); ++iter)
			MyGUI::ResourceManager::getInstance().load(*iter);

		const SettingsManager::VectorString& additionalPaths = SettingsManager::getInstance().getValueList("Resources/AdditionalPath.List");
		for (SettingsManager::VectorString::const_iterator iter = additionalPaths.begin(); iter != additionalPaths.end(); ++iter)
			addResourceLocation(*iter);

		const SettingsManager::VectorString& additionalResources = SettingsManager::getInstance().getValueList("Resources/AdditionalResource.List");
		for (SettingsManager::VectorString::const_iterator iter = additionalResources.begin(); iter != additionalResources.end(); ++iter)
			MyGUI::ResourceManager::getInstance().load(*iter);
	}

	void Application::CreateControls()
	{
		const SettingsManager::VectorString& controls = SettingsManager::getInstance().getValueList("Editor/Control.List");
		for (SettingsManager::VectorString::const_iterator controlType = controls.begin(); controlType != controls.end(); controlType ++)
		{
			Control* control = components::FactoryManager::GetInstance().CreateItem<Control>(*controlType);
			if (control != nullptr)
			{
				control->Initialise();
				mControls.push_back(control);
			}
			else
			{
				MYGUI_LOG(Warning, "Control factory '" << (*controlType) << "' not found");
			}
		}
	}

	void Application::DestroyControls()
	{
		for (VectorControl::iterator control = mControls.begin(); control != mControls.end(); control ++)
			delete *control;
		mControls.clear();
	}

}

MYGUI_APP(tools::Application)
