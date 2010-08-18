/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TextureControl.h"
#include "SkinManager.h"
#include "Localise.h"

namespace tools
{

	TextureControl::TextureControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("TextureControl.layout", _parent),
		mView(nullptr),
		mTexture(nullptr),
		mBackgroundColour(nullptr),
		mBackground(nullptr),
		mScale(nullptr),
		mScaleValue(1),
		mBackgroundButton(nullptr),
		mColourPanel(nullptr)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");
		assignWidget(mScale, "Scale");
		assignWidget(mBackgroundButton, "BackgroundColourButton");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);

		fillScale();
		mScale->eventComboChangePosition += MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);

		mBackgroundButton->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonClick);

		mColourPanel = new ColourPanel();
		mColourPanel->setVisible(false);
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &TextureControl::notifyEndDialog);
	}

	TextureControl::~TextureControl()
	{
		delete mColourPanel;
		mColourPanel = nullptr;

		mBackgroundButton->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonClick);

		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);
		mScale->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);
	}

	void TextureControl::fillColours(MyGUI::ComboBox* _combo)
	{
		_combo->removeAllItems();

		_combo->addItem(replaceTags("#{ColourZero}"), MyGUI::Colour::Zero);
		_combo->addItem(replaceTags("#{ColourBlack}"), MyGUI::Colour::Black);
		_combo->addItem(replaceTags("#{ColourWhite}"), MyGUI::Colour::White);
		_combo->addItem(replaceTags("#{ColourRed}"), MyGUI::Colour::Red);
		_combo->addItem(replaceTags("#{ColourGreen}"), MyGUI::Colour::Green);
		_combo->addItem(replaceTags("#{ColourBlue}"), MyGUI::Colour::Blue);

		_combo->setIndexSelected(0);

		updateColour(_combo);
	}

	void TextureControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		updateColour(_sender);
	}

	void TextureControl::updateColour(MyGUI::ComboBox* _sender)
	{
		if (_sender == mBackgroundColour)
		{
			size_t index = mBackgroundColour->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				mCurrentColour = *mBackgroundColour->getItemDataAt<MyGUI::Colour>(index);
				updateColours();
			}
		}
		else if (_sender == mScale)
		{
			size_t index = mScale->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				mScaleValue = *mScale->getItemDataAt<double>(index);
				updateScale();
			}
		}
	}

	void TextureControl::fillScale()
	{
		mScale->removeAllItems();

		mScale->addItem(replaceTags("#{Scale50}"), (double)0.5);
		mScale->addItem(replaceTags("#{Scale100}"), (double)1);
		mScale->addItem(replaceTags("#{Scale200}"), (double)2);
		mScale->addItem(replaceTags("#{Scale400}"), (double)4);
		mScale->addItem(replaceTags("#{Scale800}"), (double)8);
		mScale->addItem(replaceTags("#{Scale1600}"), (double)16);

		size_t index = 1;

		mScale->setIndexSelected(index);
		mScaleValue = *mScale->getItemDataAt<double>(index);

		updateScale();
	}

	void TextureControl::updateScale()
	{
		double width = (double)mTextureRegion.width * mScaleValue;
		double height = (double)mTextureRegion.height * mScaleValue;

		mView->setCanvasSize(MyGUI::IntSize((int)width, (int)height));

		for (std::vector<SelectorControl*>::iterator item=mSelectors.begin(); item!=mSelectors.end(); ++item)
			(*item)->setScale(mScaleValue);
	}

	void TextureControl::setTextureName(const MyGUI::UString& _value)
	{
		mTextureSize = MyGUI::texture_utility::getTextureSize(_value);
		mTexture->setImageTexture(_value);

		mTextureRegion.set(0, 0, mTextureSize.width, mTextureSize.height);

		updateScale();
	}

	const MyGUI::IntSize& TextureControl::getTextureSize()
	{
		return mTextureSize;
	}

	void TextureControl::setTextureRegion(const MyGUI::IntCoord& _value)
	{
		mTextureRegion = _value;
		mTexture->setImageCoord(mTextureRegion);
		mTexture->setImageTile(mTextureRegion.size());
		mTexture->setImageIndex(0);

		updateScale();
	}

	const MyGUI::IntCoord& TextureControl::getTextureRegion()
	{
		return mTextureRegion;
	}

	void TextureControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		mColourPanel->setColour(mCurrentColour);
		mColourPanel->setVisible(true);
	}

	void TextureControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
			mCurrentColour = mColourPanel->getColour();
			mCurrentColour.alpha = 1;
			updateColours();
		}

		mColourPanel->setVisible(false);
	}

	void TextureControl::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
		mBackgroundButton->setColour(mCurrentColour);
		mBackgroundButton->setAlpha(mCurrentColour.alpha);
	}

} // namespace tools
