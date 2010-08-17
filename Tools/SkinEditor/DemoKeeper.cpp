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
#include "MyGUI_FilterNoneSkin.h"
#include "FileSystemInfo/FileSystemInfo.h"

template <> demo::DemoKeeper* MyGUI::Singleton<demo::DemoKeeper>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<demo::DemoKeeper>::mClassTypeName("DemoKeeper");

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr),
		mChanges(false),
		mOpenSaveFileDialog(nullptr),
		mDefaultFileName("unnamed.xml"),
		mFileName("unnamed.xml")
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		MyGUI::ResourceManager::getInstance().load("SE_skins.xml");
		MyGUI::ResourceManager::getInstance().load("colour_slider_skin.xml");

		tools::CommandManager* commandManager = new tools::CommandManager();
		commandManager->initialise();

		tools::SkinManager* skinManager = new tools::SkinManager();
		skinManager->initialise();

		tools::ActionManager* actionManager = new tools::ActionManager();
		actionManager->initialise();

		mMainPane = new tools::MainPane();

		mOpenSaveFileDialog = new common::OpenSaveFileDialog();
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialog);
		mOpenSaveFileDialog->setFileMask("*.xml");

		tools::CommandManager::getInstance().registerCommand("Command_FileLoad", MyGUI::newDelegate(this, &DemoKeeper::commandLoad));
		tools::CommandManager::getInstance().registerCommand("Command_FileSave", MyGUI::newDelegate(this, &DemoKeeper::commandSave));
		tools::CommandManager::getInstance().registerCommand("Command_FileSaveAs", MyGUI::newDelegate(this, &DemoKeeper::commandSaveAs));
		tools::CommandManager::getInstance().registerCommand("Command_FileExport", MyGUI::newDelegate(this, &DemoKeeper::commandExport));
		tools::CommandManager::getInstance().registerCommand("Command_ClearAll", MyGUI::newDelegate(this, &DemoKeeper::commandClear));
		tools::CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &DemoKeeper::commandQuit));

		updateCaption();
	}

	void DemoKeeper::destroyScene()
	{
		mOpenSaveFileDialog->eventEndDialog = nullptr;
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;

		delete mMainPane;
		mMainPane = nullptr;

		tools::ActionManager* actionManager = tools::ActionManager::getInstancePtr();
		actionManager->shutdown();
		delete actionManager;

		tools::SkinManager* skinManager = tools::SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;

		tools::CommandManager* commandManager = tools::CommandManager::getInstancePtr();
		commandManager->shutdown();
		delete commandManager;

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void DemoKeeper::onFileDrop(const std::wstring& _filename)
	{
	}

	bool DemoKeeper::onWinodwClose(size_t _handle)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (::IsIconic((HWND)_handle))
		ShowWindow((HWND)_handle, SW_SHOWNORMAL);
#endif

		tools::CommandManager::getInstance().executeCommand("Command_QuitApp");
		return false;
	}

	void DemoKeeper::setChanges(bool _value)
	{
		mChanges = _value;
		updateCaption();
	}

	void DemoKeeper::updateCaption()
	{
		std::wstring result = L"Skin editor - '";
		result += mFileName;
		result += L"' ";
		result += mChanges ? L"*" : L"";

		setWindowCaption(result);
	}

	void DemoKeeper::commandLoad(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
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

	void DemoKeeper::commandSave(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			save();
		}
	}

	void DemoKeeper::commandSaveAs(const MyGUI::UString & _commandName)
	{
		showSaveAsWindow();
	}

	void DemoKeeper::commandExport(const MyGUI::UString & _commandName)
	{
		showExportWindow();
	}

	void DemoKeeper::commandClear(const MyGUI::UString & _commandName)
	{
		if (tools::ActionManager::getInstance().getChanges())
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
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

	void DemoKeeper::commandQuit(const MyGUI::UString & _commandName)
	{
		if (mChanges)
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Внимание",
				L"Сохранить изменения?",
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

	void DemoKeeper::notifyMessageBoxResultLoad(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
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

	void DemoKeeper::showLoadWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("Load", "Load");
		mOpenSaveFileDialog->setMode("Load");
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::save()
	{
		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Root");
		MyGUI::xml::Element* skins = root->createChild("SkinManager");

		tools::SkinManager::getInstance().serialization(skins, MyGUI::Version());

		doc.save(mFileName);

		tools::ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::clear()
	{
		tools::SkinManager::getInstance().clear();
		tools::ActionManager::getInstance().setChanges(false);

		mFileName = mDefaultFileName;
		updateCaption();
	}

	void DemoKeeper::notifyEndDialog(wraps::BaseLayout* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "SaveAs")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				save();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Load")
			{
				mFileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				load();
				updateCaption();
			}
			else if (mOpenSaveFileDialog->getMode() == "Export")
			{
				MyGUI::UString fileName = common::concatenatePath(mOpenSaveFileDialog->getCurrentFolder(), mOpenSaveFileDialog->getFileName());
				exportSkin(fileName);
			}

		}

		mOpenSaveFileDialog->setVisible(false);
	}

	void DemoKeeper::load()
	{
		tools::SkinManager::getInstance().clear();

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
					tools::SkinManager::getInstance().deserialization(nodes.current(), MyGUI::Version());
					result = true;
					break;
				}
			}

			if (!result)
			{
				MyGUI::UString text = L"Файл '" + mFileName + L"' не соответсвует формату.";
				MyGUI::Message* message = MyGUI::Message::createMessageBox(
					"Message",
					L"Ошибка",
					text,
					MyGUI::MessageBoxStyle::IconError
						| MyGUI::MessageBoxStyle::Yes);

				mFileName = mDefaultFileName;
				updateCaption();
			}
		}
		else
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"Ошибка",
				doc.getLastError(),
				MyGUI::MessageBoxStyle::IconError
					| MyGUI::MessageBoxStyle::Yes);
		}

		tools::ActionManager::getInstance().setChanges(false);
	}

	void DemoKeeper::notifyMessageBoxResultClear(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
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
		mOpenSaveFileDialog->setDialogInfo("SaveAs", "Save");
		mOpenSaveFileDialog->setMode("SaveAs");
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::showExportWindow()
	{
		mOpenSaveFileDialog->setDialogInfo("Export", "Save");
		mOpenSaveFileDialog->setMode("Export");
		mOpenSaveFileDialog->setVisible(true);
	}

	void DemoKeeper::notifyMessageBoxResultQuit(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		if (_result == MyGUI::MessageBoxStyle::Cancel)
		{
		}
		else if (_result == MyGUI::MessageBoxStyle::Yes)
		{
			save();
			quit();
		}
		else if (_result == MyGUI::MessageBoxStyle::No)
		{
			quit();
		}
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		if (_key == MyGUI::KeyCode::Escape)
		{
			if (MyGUI::InputManager::getInstance().isModalAny())
			{
				// FIXME тут может быть любой ваще
				mOpenSaveFileDialog->eventEndDialog(nullptr, false);
			}
			else
			{
				tools::CommandManager::getInstance().executeCommand("Command_QuitApp");
			}
			return;
		}

		if (MyGUI::InputManager::getInstance().isControlPressed())
		{
			if (_key == MyGUI::KeyCode::O || _key == MyGUI::KeyCode::L)
				tools::CommandManager::getInstance().executeCommand("Command_FileLoad");
			else if (_key == MyGUI::KeyCode::S)
				tools::CommandManager::getInstance().executeCommand("Command_FileSave");

			return;
		}

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	void DemoKeeper::exportSkin(const MyGUI::UString& _fileName)
	{
		MyGUI::xml::Document doc;
		MyGUI::xml::Element* root = doc.createRoot("Root");

		tools::SkinManager::getInstance().serialization(root, MyGUI::Version());

		MyGUI::xml::Document docOut;
		docOut.createDeclaration();
		MyGUI::xml::Element* rootOut = docOut.createRoot("MyGUI");
		rootOut->addAttribute("type", "Resource");
		rootOut->addAttribute("version", "1.2");

		MyGUI::xml::ElementEnumerator skins = root->getElementEnumerator();
		while (skins.next())
			convertSkin(skins.current(), rootOut->createChild("Resource"));

		docOut.save(_fileName);
	}

	void DemoKeeper::convertSkin(MyGUI::xml::Element* _from, MyGUI::xml::Element* _to)
	{
		_to->addAttribute("type", "ResourceSkin");
		_to->addAttribute("name", _from->findAttribute("name"));

		MyGUI::xml::ElementEnumerator nodes = _from->getElementEnumerator();
		while (nodes.next())
		{
			MyGUI::UString coordValue;
			MyGUI::UString textureValue;

			MyGUI::xml::Element* node = nodes.current();
			if (node->getName() == "PropertySet")
			{
				MyGUI::xml::ElementEnumerator propertyNodes = node->getElementEnumerator();
				while (propertyNodes.next("Property"))
				{
					MyGUI::xml::Element* propertyNode = propertyNodes.current();

					std::string name;
					if (propertyNode->findAttribute("name", name))
					{
						if (name == "Coord")
						{
							MyGUI::IntCoord coord = MyGUI::IntCoord::parse(propertyNode->getContent());
							coordValue = coord.size().print();
						}
						else if (name == "Texture")
						{
							textureValue = propertyNode->getContent();
						}
					}
				}

				_to->addAttribute("size", coordValue);
				_to->addAttribute("texture", textureValue);

				break;
			}
			else if (node->getName() == "RegionItem")
			{
				bool regionVisible = true;

				MyGUI::xml::ElementEnumerator regionNodes = node->getElementEnumerator();
				while (regionNodes.next("PropertySet"))
				{
					MyGUI::xml::ElementEnumerator propertyNodes = regionNodes->getElementEnumerator();
					while (propertyNodes.next("Property"))
					{
						MyGUI::xml::Element* propertyNode = propertyNodes.current();

						std::string name;
						if (propertyNode->findAttribute("name", name))
						{
							if (name == "Visible")
							{
								if (propertyNode->getContent() != "True")
									regionVisible = false;
							}
							else if (name == "Enabled")
							{
								if (propertyNode->getContent() != "True")
									regionVisible = false;
							}
						}
					}
					break;
				}

				if (!regionVisible)
					continue;

				MyGUI::xml::Element* region = _to->createChild("Region");
				MyGUI::IntCoord regionCoord;

				MyGUI::UString regionTypeValue;
				MyGUI::UString regionOffsetValue;
				MyGUI::UString alignValue;

				regionNodes = node->getElementEnumerator();
				while (regionNodes.next("PropertySet"))
				{
					MyGUI::xml::ElementEnumerator propertyNodes = regionNodes->getElementEnumerator();
					while (propertyNodes.next("Property"))
					{
						MyGUI::xml::Element* propertyNode = propertyNodes.current();

						std::string name;
						if (propertyNode->findAttribute("name", name))
						{
							if (name == "RegionType")
							{
								regionTypeValue = propertyNode->getContent();
							}
							else if (name == "Position")
							{
								regionCoord = MyGUI::IntCoord::parse(propertyNode->getContent());
								regionOffsetValue = regionCoord.print();
							}
							else if (name == "Align")
							{
								alignValue = propertyNode->getContent();
							}
						}
					}

					bool tileHor = true;
					bool tileVer = true;

					if (regionTypeValue == "TileRect Hor")
					{
						regionTypeValue = "TileRect";
						tileVer = false;
					}
					else if (regionTypeValue == "TileRect Ver")
					{
						regionTypeValue = "TileRect";
						tileHor = false;
					}

					region->addAttribute("type", regionTypeValue);
					region->addAttribute("offset", regionOffsetValue);
					region->addAttribute("align", alignValue);

					MyGUI::xml::ElementEnumerator stateNodes = _from->getElementEnumerator();
					while (stateNodes.next("StateItem"))
					{
						bool stateVisible = true;

						MyGUI::xml::ElementEnumerator propertySetNodes = stateNodes->getElementEnumerator();
						while (propertySetNodes.next("PropertySet"))
						{
							MyGUI::xml::ElementEnumerator propertyNodes = propertySetNodes->getElementEnumerator();
							while (propertyNodes.next("Property"))
							{
								MyGUI::xml::Element* propertyNode = propertyNodes.current();

								std::string name;
								if (propertyNode->findAttribute("name", name))
								{
									if (name == "Visible")
									{
										if (propertyNode->getContent() != "True")
											stateVisible = false;
									}
								}
							}
							break;
						}

						if (!stateVisible)
							continue;

						MyGUI::xml::Element* state = region->createChild("State");

						MyGUI::UString stateOffsetValue;
						MyGUI::UString textColourValue;
						MyGUI::UString textShiftValue;

						propertySetNodes = stateNodes->getElementEnumerator();
						while (propertySetNodes.next("PropertySet"))
						{
							MyGUI::xml::ElementEnumerator propertyNodes = propertySetNodes->getElementEnumerator();
							while (propertyNodes.next("Property"))
							{
								MyGUI::xml::Element* propertyNode = propertyNodes.current();

								std::string name;
								if (propertyNode->findAttribute("name", name))
								{
									if (name == "Position")
									{
										MyGUI::IntCoord coord = regionCoord + MyGUI::IntPoint::parse(propertyNode->getContent());
										stateOffsetValue = coord.print();
									}
									else if (name == "TextColour")
									{
										textColourValue = propertyNode->getContent();
									}
									else if (name == "TextShift")
									{
										textShiftValue = propertyNode->getContent();
									}
								}
							}
							break;
						}

						MyGUI::UString stateNameValue = stateNodes->findAttribute("name");
						state->addAttribute("name", stateNameValue);

						if (regionTypeValue == "SimpleText" || regionTypeValue == "EditText")
						{
							state->addAttribute("colour", textColourValue);
							state->addAttribute("shift", textShiftValue);
						}
						else
						{
							state->addAttribute("offset", stateOffsetValue);

							if (stateNameValue == "Normal")
							{
								if (regionTypeValue == "TileRect")
								{
									MyGUI::xml::Element* prop = state->createChild("Property");
									prop->addAttribute("key", "TileSize");
									prop->addAttribute("value", regionCoord.size().print());
									prop = state->createChild("Property");
									prop->addAttribute("key", "TileH");
									prop->addAttribute("value", MyGUI::utility::toString(tileHor));
									prop = state->createChild("Property");
									prop->addAttribute("key", "TileV");
									prop->addAttribute("value", MyGUI::utility::toString(tileVer));
								}
							}
						}
					}
					break;
				}
			}
		}
		
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
