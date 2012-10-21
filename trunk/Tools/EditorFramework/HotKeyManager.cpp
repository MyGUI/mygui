/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "HotKeyManager.h"
#include "CommandManager.h"

template <> tools::HotKeyManager* MyGUI::Singleton<tools::HotKeyManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::HotKeyManager>::mClassTypeName = "HotKeyManager";

namespace tools
{

	HotKeyManager::HotKeyManager()
	{
		#define BIND_KEY(name) mKeyNames[#name] = MyGUI::KeyCode::name

		BIND_KEY(None);
		BIND_KEY(Escape);
		BIND_KEY(One);
		BIND_KEY(Two);
		BIND_KEY(Three);
		BIND_KEY(Four);
		BIND_KEY(Five);
		BIND_KEY(Six);
		BIND_KEY(Seven);
		BIND_KEY(Eight);
		BIND_KEY(Nine);
		BIND_KEY(Zero);
		BIND_KEY(Minus);
		BIND_KEY(Equals);
		BIND_KEY(Backspace);
		BIND_KEY(Tab);
		BIND_KEY(Q);
		BIND_KEY(W);
		BIND_KEY(E);
		BIND_KEY(R);
		BIND_KEY(T);
		BIND_KEY(Y);
		BIND_KEY(U);
		BIND_KEY(I);
		BIND_KEY(O);
		BIND_KEY(P);
		BIND_KEY(LeftBracket);
		BIND_KEY(RightBracket);
		BIND_KEY(Return);
		BIND_KEY(LeftControl);
		BIND_KEY(A);
		BIND_KEY(S);
		BIND_KEY(D);
		BIND_KEY(F);
		BIND_KEY(G);
		BIND_KEY(H);
		BIND_KEY(J);
		BIND_KEY(K);
		BIND_KEY(L);
		BIND_KEY(Semicolon);
		BIND_KEY(Apostrophe);
		BIND_KEY(Grave);
		BIND_KEY(LeftShift);
		BIND_KEY(Backslash);
		BIND_KEY(Z);
		BIND_KEY(X);
		BIND_KEY(C);
		BIND_KEY(V);
		BIND_KEY(B);
		BIND_KEY(N);
		BIND_KEY(M);
		BIND_KEY(Comma);
		BIND_KEY(Period);
		BIND_KEY(Slash);
		BIND_KEY(RightShift);
		BIND_KEY(Multiply);
		BIND_KEY(LeftAlt);
		BIND_KEY(Space);
		BIND_KEY(Capital);
		BIND_KEY(F1);
		BIND_KEY(F2);
		BIND_KEY(F3);
		BIND_KEY(F4);
		BIND_KEY(F5);
		BIND_KEY(F6);
		BIND_KEY(F7);
		BIND_KEY(F8);
		BIND_KEY(F9);
		BIND_KEY(F10);
		BIND_KEY(NumLock);
		BIND_KEY(ScrollLock);
		BIND_KEY(Numpad7);
		BIND_KEY(Numpad8);
		BIND_KEY(Numpad9);
		BIND_KEY(Subtract);
		BIND_KEY(Numpad4);
		BIND_KEY(Numpad5);
		BIND_KEY(Numpad6);
		BIND_KEY(Add);
		BIND_KEY(Numpad1);
		BIND_KEY(Numpad2);
		BIND_KEY(Numpad3);
		BIND_KEY(Numpad0);
		BIND_KEY(Decimal);
		BIND_KEY(OEM_102);
		BIND_KEY(F11);
		BIND_KEY(F12);
		BIND_KEY(F13);
		BIND_KEY(F14);
		BIND_KEY(F15);
		BIND_KEY(Kana);
		BIND_KEY(ABNT_C1);
		BIND_KEY(Convert);
		BIND_KEY(NoConvert);
		BIND_KEY(Yen);
		BIND_KEY(ABNT_C2);
		BIND_KEY(NumpadEquals);
		BIND_KEY(PrevTrack);
		BIND_KEY(At);
		BIND_KEY(Colon);
		BIND_KEY(Underline);
		BIND_KEY(Kanji);
		BIND_KEY(Stop);
		BIND_KEY(AX);
		BIND_KEY(Unlabeled);
		BIND_KEY(NextTrack);
		BIND_KEY(NumpadEnter);
		BIND_KEY(RightControl);
		BIND_KEY(Mute);
		BIND_KEY(Calculator);
		BIND_KEY(PlayPause);
		BIND_KEY(MediaStop);
		BIND_KEY(VolumeDown);
		BIND_KEY(VolumeUp);
		BIND_KEY(WebHome);
		BIND_KEY(NumpadComma);
		BIND_KEY(Divide);
		BIND_KEY(SysRq);
		BIND_KEY(RightAlt);
		BIND_KEY(Pause);
		BIND_KEY(Home);
		BIND_KEY(ArrowUp);
		BIND_KEY(PageUp);
		BIND_KEY(ArrowLeft);
		BIND_KEY(ArrowRight);
		BIND_KEY(End);
		BIND_KEY(ArrowDown);
		BIND_KEY(PageDown);
		BIND_KEY(Insert);
		BIND_KEY(Delete);
		BIND_KEY(LeftWindows);
		BIND_KEY(RightWindow);
		BIND_KEY(RightWindows);
		BIND_KEY(AppMenu);
		BIND_KEY(Power);
		BIND_KEY(Sleep);
		BIND_KEY(Wake);
		BIND_KEY(WebSearch);
		BIND_KEY(WebFavorites);
		BIND_KEY(WebRefresh);
		BIND_KEY(WebStop);
		BIND_KEY(WebForward);
		BIND_KEY(WebBack);
		BIND_KEY(MyComputer);
		BIND_KEY(Mail);
		BIND_KEY(MediaSelect);

		#undef BIND_KEY
	}

	HotKeyManager::~HotKeyManager()
	{
		mKeyNames.clear();
	}

	void HotKeyManager::initialise()
	{
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("HotKeys") = MyGUI::newDelegate(this, &HotKeyManager::loadXml);
	}

	void HotKeyManager::shutdown()
	{
		MyGUI::ResourceManager::getInstance().unregisterLoadXmlDelegate("HotKeys");
	}

	void HotKeyManager::loadXml(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("Command"))
		{
			HotKeyCommand command;

			MyGUI::xml::ElementEnumerator nodeCommand = node->getElementEnumerator();
			while (nodeCommand.next())
			{
				if (nodeCommand->getName() == "Name")
				{
					command.setCommand(nodeCommand->getContent());
				}
				/*else if (nodeCommand->getName() == "Pressed")
				{
					command.setPressed(MyGUI::utility::parseValue<bool>(nodeCommand->getContent()));
				}*/
				else if (nodeCommand->getName() == "KeyCode")
				{
					MapKeys::const_iterator item = mKeyNames.find(nodeCommand->getContent());
					if (item != mKeyNames.end())
					{
						command.setKey((*item).second);
					}
					else
					{
						// log
					}
				}
				else if (nodeCommand->getName() == "Modifier")
				{
					command.setShift(nodeCommand->getContent().find("Shift") != std::string::npos);
					command.setControl(nodeCommand->getContent().find("Control") != std::string::npos);
				}
			}

			addCommand(command);
		}
	}

	void HotKeyManager::addCommand(HotKeyCommand& _command)
	{
		MapCommand::iterator section = mCommands.find(_command.getKey());
		if (section == mCommands.end())
			section = mCommands.insert(std::make_pair(_command.getKey(), VectorCommand())).first;

		(*section).second.push_back(_command);
	}

	bool HotKeyManager::onKeyEvent(bool _pressed, bool _shift, bool _control, MyGUI::KeyCode _key)
	{
		bool result = false;

		MapCommand::const_iterator section = mCommands.find(_key);
		if (section == mCommands.end())
			return result;

		const VectorCommand& commands = (*section).second;
		for (VectorCommand::const_iterator item = commands.begin(); item != commands.end(); ++item)
		{
			const HotKeyCommand& command = (*item);
			if (command.getPressed() == _pressed
				&& command.getShift() == _shift
				&& command.getControl() == _control)
			{
				if (CommandManager::getInstance().executeCommand(command.getCommand()))
					result = true;
			}
		}

		return result;
	}

}
