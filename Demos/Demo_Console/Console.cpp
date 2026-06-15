/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "Console.h"

namespace demo
{

	MYGUI_SINGLETON_DEFINITION(Console);

	Console::Console() :
		BaseLayout("Console.layout"),
		mSingletonHolder(this)
	{
		assignWidget(mListHistory, "list_History");
		assignWidget(mComboCommand, "combo_Command");
		assignWidget(mButtonSubmit, "button_Submit");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += newDelegate(this, &Console::notifyWindowButtonPressed);

		mStringCurrent.assign(mMainWidget->getUserString("Current"));
		mStringError.assign(mMainWidget->getUserString("Error"));
		mStringSuccess.assign(mMainWidget->getUserString("Success"));
		mStringUnknow.assign(mMainWidget->getUserString("Unknown"));
		mStringFormat.assign(mMainWidget->getUserString("Format"));

		mAutocomleted = false;

		mComboCommand->eventComboAccept += newDelegate(this, &Console::notifyComboAccept);
		mComboCommand->eventKeyButtonPressed += newDelegate(this, &Console::notifyButtonPressed);
		mButtonSubmit->eventMouseButtonClick += newDelegate(this, &Console::notifyMouseButtonClick);
		mListHistory->setOverflowToTheLeft(true);

		mMainWidget->setVisible(false);

		registerConsoleDelegate("clear", MyGUI::newDelegate(this, &Console::internalCommand));
	}

	void Console::notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _button)
	{
		if (_button == "close")
		{
			mMainWidget->setVisible(false);
		}
	}

	void Console::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		notifyComboAccept(mComboCommand, MyGUI::ITEM_NONE);
	}

	void Console::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		const MyGUI::UString& command = _sender->getOnlyText();
		if (command.empty())
			return;

		MyGUI::UString key = command;
		MyGUI::UString value;

		size_t pos = command.find(' ');
		if (pos != MyGUI::UString::npos)
		{
			key = command.substr(0, pos);
			value = command.substr(pos + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(key);
		if (iter != mDelegates.end())
		{
			iter->second(key, value);
		}
		else
		{
			if (eventConsoleUnknowCommand.empty())
			{
				addToConsole(mStringUnknow + "'" + key + "'");
			}
			else
			{
				eventConsoleUnknowCommand(key, value);
			}
		}

		_sender->setCaption(MyGUI::UString());
	}


	void Console::notifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		MyGUI::EditBox* edit = _sender->castType<MyGUI::EditBox>();
		size_t len = edit->getCaption().length();
		if ((_key == MyGUI::KeyCode::Backspace) && (len > 0) && (mAutocomleted))
		{
			edit->deleteTextSelection();
			len = edit->getCaption().length();
			edit->eraseText(len - 1);
		}

		MyGUI::UString command = edit->getCaption();
		if (command.length() == 0)
			return;

		for (auto& delegate : mDelegates)
		{
			if (delegate.first.find(command) == 0)
			{
				if (command == delegate.first)
					break;
				edit->setCaption(delegate.first);
				edit->setTextSelection(command.length(), delegate.first.length());
				mAutocomleted = true;
				return;
			}
		}
		mAutocomleted = false;
	}

	void Console::addToConsole(const MyGUI::UString& _line)
	{
		if (mListHistory->getCaption().empty())
			mListHistory->addText(_line);
		else
			mListHistory->addText("\n" + _line);

		//mListHistory->setTextCursor(0);
		mListHistory->setTextSelection(mListHistory->getTextLength(), mListHistory->getTextLength());
	}

	void Console::clearConsole()
	{
		mListHistory->setCaption(MyGUI::UString());
	}

	void Console::registerConsoleDelegate(const MyGUI::UString& _command, CommandDelegate::IDelegate* _delegate)
	{
		mComboCommand->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");
		}
		mDelegates[_command] = _delegate;
	}

	void Console::unregisterConsoleDelegate(const MyGUI::UString& _command)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			mDelegates.erase(iter);
			for (size_t i = 0; i < mComboCommand->getItemCount(); ++i)
			{
				if (mComboCommand->getItemNameAt(i) == _command)
				{
					mComboCommand->removeItemAt(i);
					break;
				}
			}
		}
		else
			MYGUI_LOG(Warning, "console - command '" << _command << "' doesn't exist");
	}

	void Console::internalCommand(const MyGUI::UString& _key, const MyGUI::UString& _value)
	{
		if (_key == "clear")
		{
			clearConsole();
		}
	}

	void Console::addToConsole(const MyGUI::UString& _reason, const MyGUI::UString& _key, const MyGUI::UString& _value)
	{
		addToConsole(MyGUI::utility::toString(_reason, "'", _key, " ", _value, "'"));
	}

	const MyGUI::UString& Console::getConsoleStringCurrent() const
	{
		return mStringCurrent;
	}

	const MyGUI::UString& Console::getConsoleStringError() const
	{
		return mStringError;
	}

	const MyGUI::UString& Console::getConsoleStringSuccess() const
	{
		return mStringSuccess;
	}

	const MyGUI::UString& Console::getConsoleStringUnknow() const
	{
		return mStringUnknow;
	}

	const MyGUI::UString& Console::getConsoleStringFormat() const
	{
		return mStringFormat;
	}

	bool Console::getVisible()
	{
		return mMainWidget->getVisible();
	}

	void Console::setVisible(bool _visible)
	{
		mMainWidget->setVisible(_visible);
	}

} // namespace demo
