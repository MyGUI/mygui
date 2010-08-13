/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TextureControl.h"
#include "SkinManager.h"

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
		mRightMousePressed(false)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");
		assignWidget(mScale, "Scale");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);

		fillScale();
		mScale->eventComboChangePosition += MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);

		mTexture->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag += MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
	}

	TextureControl::~TextureControl()
	{
		mTexture->eventMouseButtonPressed -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag -= MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);

		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);
		mScale->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureControl::notifyComboChangePosition);
	}

	void TextureControl::fillColours(MyGUI::ComboBox* _combo)
	{
		_combo->removeAllItems();

		_combo->addItem("Zero", MyGUI::Colour::Zero);
		_combo->addItem("Black", MyGUI::Colour::Black);
		_combo->addItem("White", MyGUI::Colour::White);
		_combo->addItem("Red", MyGUI::Colour::Red);
		_combo->addItem("Green", MyGUI::Colour::Green);
		_combo->addItem("Blue", MyGUI::Colour::Blue);

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
				MyGUI::Colour colour = *mBackgroundColour->getItemDataAt<MyGUI::Colour>(index);
				mBackground->setColour(colour);
				mBackground->setAlpha(colour.alpha);
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

		mScale->addItem("50 %", (double)0.5);
		mScale->addItem("100 %", (double)1);
		mScale->addItem("200 %", (double)2);
		mScale->addItem("400 %", (double)4);
		mScale->addItem("800 %", (double)8);
		mScale->addItem("1600 %", (double)16);

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

	void TextureControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		/*if (_id == MyGUI::MouseButton::Left)
		{
			mRightMousePressed = true;
			mTexture->setPointer("hand");
			MyGUI::PointerManager::getInstance().setPointer("hand");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("hand");
		}*/
	}

	void TextureControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		/*if (_id == MyGUI::MouseButton::Left)
		{
			if (mRightMousePressed)
			{
				mRightMousePressed = false;
				mTexture->setPointer("arrow");
				MyGUI::PointerManager::getInstance().setPointer("arrow");
				MyGUI::PointerManager::getInstance().eventChangeMousePointer("arrow");
			}
		}*/
	}

	void TextureControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top)
	{
	}

} // namespace tools
