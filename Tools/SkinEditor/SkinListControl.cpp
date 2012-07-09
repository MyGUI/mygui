/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SkinListControl.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include <limits>
#include "Localise.h"
#include "MessageBoxManager.h"
#include "SettingsManager.h"

namespace tools
{

	SkinListControl::SkinListControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinListControl.layout", _parent),
		mCreate(nullptr),
		mRename(nullptr),
		mDelete(nullptr),
		mTextFieldControl(nullptr),
		mListBoxControl(nullptr)
	{
		assignWidget(mCreate, "Create");
		assignWidget(mRename, "Rename");
		assignWidget(mDelete, "Delete");
		assignBase(mListBoxControl, "ListBoxControl");

		mListBoxControl->eventChangeItemPosition += MyGUI::newDelegate(this, &SkinListControl::notifyChangeItemPosition);
		mListBoxControl->eventRelocationItem += MyGUI::newDelegate(this, &SkinListControl::notifyRelocationItem);
		mCreate->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyCreate);
		mRename->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyRename);
		mDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SkinListControl::notifyDelete);

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->eventEndDialog = MyGUI::newDelegate(this, &SkinListControl::notifyEndDialog);

		SkinManager::getInstance().eventChangeList += MyGUI::newDelegate(this, &SkinListControl::notifyChangeList);
	}

	SkinListControl::~SkinListControl()
	{
		SkinManager::getInstance().eventChangeList -= MyGUI::newDelegate(this, &SkinListControl::notifyChangeList);

		mCreate->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyCreate);
		mRename->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyRename);
		mDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SkinListControl::notifyDelete);
		mListBoxControl->eventChangeItemPosition -= MyGUI::newDelegate(this, &SkinListControl::notifyChangeItemPosition);
		mListBoxControl->eventRelocationItem -= MyGUI::newDelegate(this, &SkinListControl::notifyRelocationItem);

		delete mTextFieldControl;
		mTextFieldControl = nullptr;
	}

	void SkinListControl::notifyCreate(MyGUI::Widget* _sender)
	{
		//showTextField(nullptr);
		createItem(getNextFreeName());
	}

	void SkinListControl::notifyRename(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
			showTextField(item);
	}

	void SkinListControl::notifyDelete(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			MyGUI::Message* message = MessageBoxManager::getInstance().create(
				replaceTags("Warning"),
				replaceTags("MessageDeleteSkin"),
				MyGUI::MessageBoxStyle::IconQuest | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &SkinListControl::notifyDeleteMessageBoxResult);
		}
	}

	void SkinListControl::notifyDeleteMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _style)
	{
		if (_style == MyGUI::MessageBoxStyle::Yes)
		{
			SkinItem* item = SkinManager::getInstance().getItemSelected();

			// выделяем следующий за удаляемым
			SkinItem* prev = nullptr;
			SkinItem* next = nullptr;

			ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
			while (items.next())
			{
				SkinItem* current = items.current();
				if (current == item)
				{
					if (items.next())
						next = items.current();
					break;
				}
				prev = current;
			}

			mListBoxControl->removeItem(item);

			if (next != nullptr)
			{
				SkinManager::getInstance().setItemSelected(next);
				mListBoxControl->setItemSelected(next);
			}
			else if (prev != nullptr)
			{
				SkinManager::getInstance().setItemSelected(prev);
				mListBoxControl->setItemSelected(prev);
			}

			SkinManager::getInstance().destroyChild(item);

			ActionManager::getInstance().setChanges(true);
		}
	}

	void SkinListControl::updateList()
	{
		mListBoxControl->setIndexSelected(MyGUI::ITEM_NONE);

		while (mListBoxControl->getItemCount() > SkinManager::getInstance().getItemCount())
			mListBoxControl->removeItemAt(0);

		while (mListBoxControl->getItemCount() < SkinManager::getInstance().getItemCount())
			mListBoxControl->addItem(nullptr);

		SkinItem* selectedItem = SkinManager::getInstance().getItemSelected();

		size_t index = 0;
		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			SkinItem* item = items.current();

			size_t count = getNameCount(item->getName());
			mListBoxControl->setItemAt(index, item);

			if (item == selectedItem)
				mListBoxControl->setIndexSelected(index);

			index ++;
		}
	}

	void SkinListControl::showTextField(SkinItem* _item)
	{
		mTextFieldControl->setCaption(replaceTags("CaptionEnterName"));
		mTextFieldControl->setTextField(_item == nullptr ? getNextFreeName() : _item->getName());
		mTextFieldControl->setUserData(_item);
		mTextFieldControl->doModal();
	}

	MyGUI::UString SkinListControl::getNextFreeName()
	{
		for (size_t index = 0; index < (std::numeric_limits<size_t>::max)(); index++)
		{
			addUserTag("SkinNameIndex", MyGUI::utility::toString(index));
			MyGUI::UString name = replaceTags("SkinNamePattern");
			if (!isNameExist(name))
				return name;
		}

		return "";
	}

	bool SkinListControl::isNameExist(const MyGUI::UString& _value)
	{
		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			SkinItem* item = items.current();
			if (item->getName() == _value)
				return true;
		}
		return false;
	}

	size_t SkinListControl::getNameCount(const MyGUI::UString& _value)
	{
		size_t result = 0;
		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			SkinItem* item = items.current();
			if (item->getName() == _value)
				++result;
		}
		return result;
	}

	void SkinListControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		_sender->endModal();

		if (_result)
		{
			SkinItem* item = *mTextFieldControl->getUserData<SkinItem*>();
			if (item != nullptr)
				renameItem(item, mTextFieldControl->getTextField());
			else
				createItem(mTextFieldControl->getTextField());
		}
	}

	void SkinListControl::renameItem(SkinItem* _item, const MyGUI::UString& _value)
	{
		_item->setName(_value);

		mListBoxControl->redrawAllItems();

		ActionManager::getInstance().setChanges(true);
	}

	void SkinListControl::createItem(const MyGUI::UString& _value)
	{
		std::string textureName = getLastTextureName();

		SkinItem* item = SkinManager::getInstance().createChild();
		item->setName(_value);
		SkinManager::getInstance().setItemSelected(item);

		updateList();

		if (!textureName.empty())
			item->getPropertySet()->setPropertyValue("Texture", textureName, "");

		ActionManager::getInstance().setChanges(true);
	}

	void SkinListControl::notifyChangeList()
	{
		updateList();
	}

	std::string SkinListControl::getLastTextureName()
	{
		if (!SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("SaveLastTexture"))
			return "";

		SkinItem* lastCurrent = nullptr;

		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
			lastCurrent = items.current();

		if (lastCurrent != nullptr)
			return lastCurrent->getPropertySet()->getPropertyValue("Texture");

		return "";
	}

	void SkinListControl::notifyChangeItemPosition(ListBoxControl* _sender, SkinItem* _data)
	{
		SkinManager::getInstance().setItemSelected(_data);
	}

	void SkinListControl::notifyRelocationItem(ListBoxControl* _sender, size_t _indexFrom, size_t _indexTo)
	{
		SkinManager::getInstance().moveItem(_indexFrom, _indexTo);
		updateList();
	}

} // namespace tools
