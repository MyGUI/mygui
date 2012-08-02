/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "TextureBrowseControl.h"

namespace tools
{

	TextureBrowseControl::TextureBrowseControl() :
		mOk(nullptr),
		mCancel(nullptr),
		mTextures(nullptr)
	{
	}

	TextureBrowseControl::~TextureBrowseControl()
	{
		MyGUI::ItemBox* box = mTextures->getItemBox();
		box->eventChangeItemPosition -= MyGUI::newDelegate(this, &TextureBrowseControl::notifyChangeItemPosition);
		box->eventSelectItemAccept -= MyGUI::newDelegate(this, &TextureBrowseControl::notifySelectItemAccept);
	}

	void TextureBrowseControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, GetLayoutName(this));

		InitialiseByAttributes(this);

		setDialogRoot(mMainWidget);

		assignBase(mTextures, "Textures");

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickCancel);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TextureBrowseControl::notifyWindowButtonPressed);

		MyGUI::ItemBox* box = mTextures->getItemBox();
		box->eventChangeItemPosition += MyGUI::newDelegate(this, &TextureBrowseControl::notifyChangeItemPosition);
		box->eventSelectItemAccept += MyGUI::newDelegate(this, &TextureBrowseControl::notifySelectItemAccept);

		mMainWidget->setVisible(false);
	}

	void TextureBrowseControl::onDoModal()
	{
		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void TextureBrowseControl::onEndModal()
	{
	}

	void TextureBrowseControl::notifyMouseButtonClickCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

	void TextureBrowseControl::notifyMouseButtonClickOk(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, true);
	}

	void TextureBrowseControl::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		eventEndDialog(this, false);
	}

	const std::string& TextureBrowseControl::getTextureName() const
	{
		return mCurrentTextureName;
	}

	void TextureBrowseControl::setTextureName(const std::string& _value)
	{
		mCurrentTextureName = _value;

		MyGUI::ItemBox* box = mTextures->getItemBox();

		size_t indexSelected = MyGUI::ITEM_NONE;
		for (size_t index = 0; index < box->getItemCount(); ++index)
		{
			if (*box->getItemDataAt<std::string>(index) == mCurrentTextureName)
			{
				indexSelected = index;
				break;
			}
		}

		box->setIndexSelected(indexSelected);
	}

	void TextureBrowseControl::setTextures(const MyGUI::VectorString& _textures)
	{
		mTextures->removeAllItems();

		for (MyGUI::VectorString::const_iterator item = _textures.begin(); item != _textures.end(); ++item)
			mTextures->addItem((*item));
	}

	void TextureBrowseControl::notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			mCurrentTextureName = *_sender->getItemDataAt<std::string>(_index);
		else
			mCurrentTextureName = "";
	}

	void TextureBrowseControl::notifySelectItemAccept(MyGUI::ItemBox* _sender, size_t _index)
	{
		if (mCurrentTextureName != "")
			eventEndDialog(this, true);
	}

}
