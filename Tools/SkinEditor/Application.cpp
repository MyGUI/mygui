/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "Application.h"
#include "Base/Main.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include "CommandManager.h"
#include "ExportManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "HotKeyManager.h"
#include "StateManager.h"

template <> tools::Application* MyGUI::Singleton<tools::Application>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::Application>::mClassTypeName("Application");

namespace tools
{

	Application::Application() :
		mEditorState(nullptr)
	{
	}

	Application::~Application()
	{
	}

	void Application::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
		setResourceFilename("editor.xml");
	}

	void Application::createScene()
	{
		getStatisticInfo()->setVisible(false);

		if (!mLocale.empty())
			MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		new CommandManager();
		CommandManager::getInstance().initialise();

		new SkinManager();
		SkinManager::getInstance().initialise();

		new ActionManager();
		ActionManager::getInstance().initialise();

		new ExportManager();
		ExportManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		new StateManager();
		StateManager::getInstance().initialise();

		MyGUI::ResourceManager::getInstance().load("initialise.xml");

		mEditorState = new EditorState();
		StateManager::getInstance().pushState(mEditorState);
	}

	void Application::destroyScene()
	{
		StateManager::getInstance().popState();
		delete mEditorState;
		mEditorState = nullptr;

		StateManager::getInstance().shutdown();
		delete StateManager::getInstancePtr();

		HotKeyManager::getInstance().shutdown();
		delete HotKeyManager::getInstancePtr();

		DialogManager::getInstance().shutdown();
		delete DialogManager::getInstancePtr();

		MessageBoxManager::getInstance().shutdown();
		delete MessageBoxManager::getInstancePtr();

		ExportManager::getInstance().shutdown();
		delete ExportManager::getInstancePtr();

		ActionManager::getInstance().shutdown();
		delete ActionManager::getInstancePtr();

		SkinManager::getInstance().shutdown();
		delete SkinManager::getInstancePtr();

		CommandManager::getInstance().shutdown();
		delete CommandManager::getInstancePtr();

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void Application::prepare()
	{
		// устанавливаем локаль из переменной окружения
		// без этого не будут открываться наши файлы
		mLocale = ::setlocale( LC_ALL, "" );
		// erase everything after '_' to get language name
		mLocale.erase(std::find(mLocale.begin(), mLocale.end(), '_'), mLocale.end());
		if (mLocale == "ru")
			mLocale = "Russian";
		else if (mLocale == "en")
			mLocale = "English";

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

	void Application::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

		if (!HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key))
			input.injectKeyPress(_key, _text);
	}

	void Application::setCaption(const MyGUI::UString& _value)
	{
		setWindowCaption(_value);
	}

} // namespace tools

MYGUI_APP(tools::Application)
