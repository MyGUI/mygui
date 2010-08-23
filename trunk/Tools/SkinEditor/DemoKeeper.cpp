/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include "CommandManager.h"
#include "ExportManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "Localise.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "HotKeyManager.h"

namespace tools
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr),
		mChanges(false),
		mFileName("unnamed.xml"),
		mDefaultFileName("unnamed.xml"),
		mOpenSaveFileDialog(nullptr),
		mTestWindow(nullptr),
		mMessageBoxFadeControl(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
		setResourceFilename("editor.xml");
	}

	void DemoKeeper::createScene()
	{
		getStatisticInfo()->setVisible(false);

		addUserTag("\\n", "\n");
		addUserTag("SE_CurrentFileName", mFileName);

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

		mMainPane = new MainPane();
		mMessageBoxFadeControl = new MessageBoxFadeControl();

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");

		mTestWindow = new TestWindow();
		mTestWindow->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialogTest);

		MyGUI::ResourceManager::getInstance().load("initialise.xml");

		CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &DemoKeeper::commandLoad));
		CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &DemoKeeper::commandSave));
		CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &DemoKeeper::commandSaveAs));
		CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &DemoKeeper::commandClear));
		CommandManager::getInstance().registerCommand("Command_Test", MyGUI::newDelegate(this, &DemoKeeper::commandTest));
		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &DemoKeeper::commandQuit));
		CommandManager::getInstance().registerCommand("Command_FileDrop", MyGUI::newDelegate(this, &DemoKeeper::commandFileDrop));

		ActionManager::getInstance().eventChanges += MyGUI::newDelegate(this, &DemoKeeper::notifyChanges);

		updateCaption();

		for (VectorWString::const_iterator file = mParams.begin(); file != mParams.end(); ++file)
		{
			mFileName = *file;
			addUserTag("SE_CurrentFileName", mFileName);

			load();
			updateCaption();
			break;
		}
	}

	void DemoKeeper::destroyScene()
	{
		ActionManager::getInstance().eventChanges -= MyGUI::newDelegate(this, &DemoKeeper::notifyChanges);

		delete mTestWindow;
		mTestWindow = nullptr;

		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mMainPane;
		mMainPane = nullptr;

		delete mMessageBoxFadeControl;
		mMessageBoxFadeControl = nullptr;

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

	void DemoKeeper::prepare()
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

	void DemoKeeper::onFileDrop(const std::wstring& _fileName)
	{
		CommandManager::getInstance().setCommandData(_fileName);
		CommandManager::getInstance().executeCommand("Command_FileDrop");
	}

	bool DemoKeeper::onWinodwClose(size_t _handle)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (::IsIconic((HWND)_handle))
			ShowWindow((HWND)_handle, SW_SHOWNORMAL);
#endif

		CommandManager::getInstance().executeCommand("Command_QuitApp");
		return false;
	}

	void DemoKeeper::updateCaption()
	{
		addUserTag("SE_HasChanged", mChanges ? "*" : "");
		setWindowCaption(replaceTags("CaptionMainWindow"));
	}

	void DemoKeeper::commandLoad(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultLoad);
		}
		else
		{
			showLoadWindow();
		}
	}

	void DemoKeeper::commandSave(const MyGUI::UString& _commandName)
	{
		if (ActionManager::getInstance().getChanges())
		{
			save();
		}
	}

	void DemoKeeper::commandSaveAs(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		showSaveAsWindow();
	}

	void DemoKeeper::commandClear(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultClear);
		}
		else
		{
			clear();
		}
	}

	void DemoKeeper::commandQuit(const MyGUI::UString& _commandName)
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
				if (mChanges)
				{
					MyGUI::Message* message = MessageBoxManager::getInstance().create(
						replaceTags("Warning"),
						replaceTags("MessageUnsavedData"),
						MyGUI::MessageBoxStyle::IconQuest
							| MyGUI::MessageBoxStyle::Yes
							| MyGUI::MessageBoxStyle::No
							| MyGUI::MessageBoxStyle::Cancel);
					message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultQuit);
				}
				else
				{
					quit();
				}
			}
		}
	}

	void DemoKeeper::commandFileDrop(const MyGUI::UString& _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (MessageBoxManager::getInstance().hasAny())
			return;

		mDropFileName = CommandManager::getInstance().getCommandData();
		if (mDropFileName.empty())
			return;

		if (ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageUnsavedData"),
				MyGUI::MessageBoxStyle::IconQuest
					| MyGUI::MessageBoxStyle::Yes
					| MyGUI::MessageBoxStyle::No
					| MyGUI::MessageBoxStyle::Cancel);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &DemoKeeper::notifyMessageBoxResultLoadDropFile);
		}
		else
		{
			loadDropFile();
		}
	}

	void DemoKeeper::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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

	void DemoKeeper::notifyMessageBoxResultLoadDropFile(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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

	void DemoKeeper::loadDropFile()
	{
		mFileName = mDropFileName;
		addUserTag("SE_CurrentFileName", mFileName);

		load();
		updateCaption();
	}

	void DemoKeeper::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFile"), replaceTags("ButtonOpenFile"));
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->doModal();
	}

	void DemoKeeper::save()
	{
		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(mFileName);

		ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::clear()
	{
		SkinManager::getInstance().clear();
		ActionManager::getInstance().setChanges(false);

		mFileName = mDefaultFileName;
		addUserTag("SE_CurrentFileName", mFileName);

		updateCaption();
	}

	void DemoKeeper::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("SE_CurrentFileName", mFileName);

				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				addUserTag("SE_CurrentFileName", mFileName);

				load();
				updateCaption();
			}
		}

		mOpenSaveFileDialog->endModal();
	}

	void DemoKeeper::load()
	{
		SkinManager::getInstance().clear();

		MyGUI::xml::Document doc;
		if (doc.open(mFileName))
		{
			bool result = false;
			MyGUI::xml::Element* root = doc.getRoot();
			if (root->getName() == "Root")
			{
				MyGUI::xml::ElementEnumerator nodes = root->getElementEnumerator();
				while (nodes.next("SkinManager"))
				{
					SkinManager::getInstance().deserialization(nodes.current(), MyGUI::Version());
					result = true;
					break;
				}
			}

			if (!result)
			{
				MyGUI::Message* message = MessageBoxManager::getInstance().create(
					replaceTags("Error"),
					replaceTags("MessageIncorrectFileFormat"),
					MyGUI::MessageBoxStyle::IconError
						| MyGUI::MessageBoxStyle::Yes);

				mFileName = mDefaultFileName;
				addUserTag("SE_CurrentFileName", mFileName);

				updateCaption();
			}
		}
		else
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Error"),
				doc.getLastError(),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}

		ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
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

	void DemoKeeper::showSaveAsWindow()
	{
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionSaveFile"), replaceTags("ButtonSaveFile"));
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->doModal();
	}

	void DemoKeeper::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			quit();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			quit();
		}
	}

	void DemoKeeper::commandTest(const MyGUI::UString & _commandName)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			mTestWindow->setSkinItem(item);
			mTestWindow->doModal();
		}
	}

	void DemoKeeper::notifyEndDialogTest(Dialog* _sender, bool _result)
	{
		_sender->endModal();
	}

	void DemoKeeper::notifyChanges(bool _changes)
	{
		mChanges = _changes;
		updateCaption();
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

		if (!HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key))
			input.injectKeyPress(_key, _text);
	}

} // namespace tools

MYGUI_APP(tools::DemoKeeper)
