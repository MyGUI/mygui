/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TextureControl.h"
#include "Localise.h"

namespace tools
{

	TextureControl::TextureControl(const std::string& _layout, MyGUI::Widget* _parent) :
		wraps::BaseLayout(_layout, _parent),
		mView(nullptr),
		mTexture(nullptr),
		mBackground(nullptr),
		mScaleValue(1.0)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackground, "Background");
	}

	TextureControl::~TextureControl()
	{
	}

	void TextureControl::updateScale()
	{
		double width = (double)mTextureRegion.width * mScaleValue;
		double height = (double)mTextureRegion.height * mScaleValue;

		mView->setCanvasSize(MyGUI::IntSize((int)width, (int)height));

		for (std::vector<SelectorControl*>::iterator item = mSelectors.begin(); item != mSelectors.end(); ++item)
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

	void TextureControl::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
	}

	void TextureControl::setColour(MyGUI::Colour _value)
	{
		mCurrentColour = _value;
		updateColours();
	}

	MyGUI::Colour TextureControl::getColour()
	{
		return mCurrentColour;
	}

	void TextureControl::setScale(double _value)
	{
		mScaleValue = _value;
		updateScale();
	}

} // namespace tools
