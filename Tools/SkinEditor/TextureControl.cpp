/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
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

		mTexture->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonClick);
		mTexture->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
	}

	TextureControl::~TextureControl()
	{
		mTexture->eventMouseWheel -= MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
		mTexture->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonClick);
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

	void TextureControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getLastLeftPressed() - mTexture->getAbsolutePosition();
		point.left = (int)((double)point.left / mScaleValue);
		point.top = (int)((double)point.top / mScaleValue);
		onMouseButtonClick(point);
	}

	void TextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
	}

	bool TextureControl::getSelectorsCapture()
	{
		for (std::vector<SelectorControl*>::iterator item = mSelectors.begin(); item != mSelectors.end(); ++item)
			if ((*item)->getCapture())
				return true;
		return false;
	}

	void TextureControl::notifyMouseWheel(MyGUI::Widget* _sender, int _rel)
	{
		onMouseWheel(_rel);
	}

	void TextureControl::onMouseWheel(int _rel)
	{
	}

	void TextureControl::registerSelectorControl(SelectorControl* _control)
	{
		mSelectors.push_back(_control);
		_control->setScale(mScaleValue);
		_control->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifySelecetionWheel);
	}

	void TextureControl::notifySelecetionWheel(SelectorControl* _control, int _wheelValue)
	{
		onMouseWheel(_wheelValue);
	}

} // namespace tools
