/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "TextureBrowseControl.h"

namespace tools
{
	TextureBrowseControl::TextureBrowseControl() :
		Dialog(),
		mTextures(nullptr)
	{
		initialiseByAttributes(this);

		assignBase(mTextures, "Textures");

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickCancel);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TextureBrowseControl::notifyWindowButtonPressed);

		mMainWidget->setVisible(false);
	}

	TextureBrowseControl::~TextureBrowseControl()
	{
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

	const std::string& TextureBrowseControl::getTextureName()
	{
		return mCurrentTextureName;
	}

	void TextureBrowseControl::setTextureName(const std::string& _value)
	{
		mCurrentTextureName = _value;
	}

	void TextureBrowseControl::setTextures(const MyGUI::VectorString& _textures)
	{
		mTextures->removeAllItems();

		for (MyGUI::VectorString::const_iterator item = _textures.begin(); item != _textures.end(); ++item)
			mTextures->addItem((*item));
	}

} // namespace tools
