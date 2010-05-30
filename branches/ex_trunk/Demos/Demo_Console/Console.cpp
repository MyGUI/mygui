/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "Console.h"

namespace demo
{

	Console * Console::m_instance = 0;

	Console * Console::getInstancePtr() { return m_instance; }
	Console & Console::getInstance() { assert(m_instance); return *m_instance; }


	Console::Console() : BaseLayout("Console.layout")
	{
		assert(!m_instance);
		m_instance = this;

		//assignWidget(mListHistory, "list_History");
		assignWidget(mListBoxHistory, "listBox_History");
		assignWidget(mComboCommand, "combo_Command");
		assignWidget(mButtonSubmit, "button_Submit");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr) window->eventWindowButtonPressed = newDelegate(this, &Console::notifyWindowButtonPressed);

		mStringCurrent = mMainWidget->getUserString("Current");
		mStringError = mMainWidget->getUserString("Error");
		mStringSuccess = mMainWidget->getUserString("Success");
		mStringUnknow = mMainWidget->getUserString("Unknown");
		mStringFormat = mMainWidget->getUserString("Format");

		mAutocomleted = false;

		mComboCommand->eventComboAccept = newDelegate(this, &Console::notifyComboAccept);
		mComboCommand->eventKeyButtonPressed = newDelegate(this, &Console::notifyButtonPressed);
		mButtonSubmit->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);
		//mListHistory->setOverflowToTheLeft(true);

		mListBoxHistory->requestCreateWidgetItem = MyGUI::newDelegate(this, &Console::notifyCreateWidgetItem);
		mListBoxHistory->requestDrawItem = MyGUI::newDelegate(this, &Console::notifyDrawItem);

		mMainWidget->setVisible(false);
	}

	Console::~Console()
	{
		m_instance = 0;
	}

	void Console::notifyCreateWidgetItem(MyGUI::ListCtrl* _sender, MyGUI::Widget* _item)
	{
		const MyGUI::IntSize& size = _item->getSize();

		MyGUI::Edit* text = _item->createWidget<MyGUI::Edit>("EditTest", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Stretch);
		text->setEditWordWrap(true);

		//text->setNeedMouseFocus(false);

		_item->setUserData(text);
	}

	void Console::notifyDrawItem(MyGUI::ListCtrl* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info, MyGUI::IntCoord& _coord)
	{
		MyGUI::Edit* text = *_item->getUserData<MyGUI::Edit*>();

		if (_info.update)
		{
			text->setCaption(mLines[_info.index]);//*mListBoxHistory->getItemDataAt<MyGUI::UString>(_info.index));

			MyGUI::IntSize size = text->getTextSize() + (text->getSize() - text->getTextRegion().size());
			//size.height = 20;
			_coord.set(0, 0, size.width, size.height);
		}

		//text->setButtonPressed(_info.select);
		//text->_setMouseFocus(_info.active);
	}

	void Console::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _button)
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
		const MyGUI::UString & command = _sender->getCaption();
		if (command == "") return;

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

		_sender->setCaption("");
	}


	void Console::notifyButtonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
		size_t len = _sender->getCaption().length();
		MyGUI::Edit* edit = _sender->castType<MyGUI::Edit>();
		if ((_key == MyGUI::KeyCode::Backspace) && (len > 0) && (mAutocomleted))
		{
			edit->deleteTextSelection();
			len = _sender->getCaption().length();
			edit->eraseText(len-1);
		}

		MyGUI::UString command = _sender->getCaption();
		if (command.length() == 0) return;

		for (MapDelegate::iterator iter = mDelegates.begin(); iter != mDelegates.end(); ++iter)
		{
			if (iter->first.find(command) == 0)
			{
				if (command == iter->first) break;
				edit->setCaption(iter->first);
				edit->setTextSelection(command.length(), iter->first.length());
				mAutocomleted = true;
				return;
			}
		}
		mAutocomleted = false;
	}

	void Console::addToConsole(const MyGUI::UString & _line)
	{
		/*if (mListHistory->getCaption().empty())
		{
			mListHistory->addText(_line);
			mListBoxHistory->addItem(_line);
		}
		else
		{
			mListHistory->addText("\n" + _line);*/
		mLines.push_back(_line);
		mListBoxHistory->addItem();
		//}

		//mListHistory->setTextCursor(0);
		//mListHistory->setTextSelection(mListHistory->getTextLength(), mListHistory->getTextLength());
	}

	void Console::clearConsole()
	{
		//mListHistory->setCaption("");
		mListBoxHistory->removeAllItems();
		mLines.clear();
	}

	void Console::registerConsoleDelegate(const MyGUI::UString & _command, CommandDelegate::IDelegate * _delegate)
	{
		mComboCommand->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");
		}
		mDelegates[_command] = _delegate;
	}

	void Console::internalCommand(MyGUI::Widget* _sender, const MyGUI::UString & _key, const MyGUI::UString & _value)
	{
		if (_key == "clear")
		{
			clearConsole();
		}
	}

} // namespace demo
