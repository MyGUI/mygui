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
		mScaleValue(1.0),
		mMouseCapture(false)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackground, "Background");

		mTexture->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonClick);
		mTexture->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag += MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
		mTexture->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
	}

	TextureControl::~TextureControl()
	{
		mTexture->eventMouseWheel -= MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
		mTexture->eventMouseButtonPressed -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased -= MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		mTexture->eventMouseDrag -= MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
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
		if (mMouseCapture)
			return true;

		for (std::vector<SelectorControl*>::iterator item = mSelectors.begin(); item != mSelectors.end(); ++item)
			if ((*item)->getCapture())
				return true;
		return false;
	}

	void TextureControl::notifyMouseWheel(MyGUI::Widget* _sender, int _rel)
	{
		if (!mMouseCapture)
		{
			saveMouseRelative();
			onMouseWheel(_rel);
			loadMouseRelative();
		}
	}

	void TextureControl::onMouseWheel(int _rel)
	{
	}

	void TextureControl::registerSelectorControl(SelectorControl* _control)
	{
		mSelectors.push_back(_control);
		_control->setScale(mScaleValue);
		_control->getMainWidget()->eventMouseWheel += MyGUI::newDelegate(this, &TextureControl::notifyMouseWheel);
		_control->getMainWidget()->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonPressed);
		_control->getMainWidget()->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControl::notifyMouseButtonReleased);
		_control->getMainWidget()->eventMouseDrag += MyGUI::newDelegate(this, &TextureControl::notifyMouseDrag);
	}

	void TextureControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mMouseCapture = true;
			mRightMouseClick = MyGUI::InputManager::getInstance().getMousePositionByLayer();
			mViewOffset = mView->getViewOffset();

			mTexture->setPointer("hand");
			MyGUI::PointerManager::getInstance().setPointer("hand");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("hand");
		}
	}

	void TextureControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mMouseCapture = false;

			mTexture->setPointer("arrow");
			MyGUI::PointerManager::getInstance().setPointer("arrow");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("arrow");
		}
	}

	void TextureControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
			MyGUI::IntPoint mouseOffset = mousePoint - mRightMouseClick;

			MyGUI::IntPoint offset = mViewOffset + mouseOffset;
			mView->setViewOffset(offset);
		}
	}

	void TextureControl::saveMouseRelative()
	{
		MyGUI::IntSize canvasSize = mView->getCanvasSize();
		MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
		MyGUI::IntPoint mouseOffset = mousePoint - mTexture->getAbsolutePosition();

		mMouseRelative.set((float)mouseOffset.left / (float)canvasSize.width, (float)mouseOffset.top / (float)canvasSize.height);
	}

	void TextureControl::loadMouseRelative()
	{
		MyGUI::IntPoint viewOffset = mView->getViewOffset();
		MyGUI::IntCoord viewCoord = mView->getViewCoord();
		MyGUI::IntSize canvasSize = mView->getCanvasSize();
		MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();

		// смещение мыши относительно вью
		MyGUI::IntPoint mouseOffset = mousePoint - mView->getAbsolutePosition() - viewCoord.point();
		// смещение нужной точки внутри текстуры в пикселях
		MyGUI::IntPoint canvasPointOffset((int)(mMouseRelative.left * (float)canvasSize.width), (int)(mMouseRelative.top * (float)canvasSize.height));
		// смещение вью в пикселях
		MyGUI::IntPoint canvasOffset = canvasPointOffset - mouseOffset;

		mView->setViewOffset(MyGUI::IntPoint(-canvasOffset.left, -canvasOffset.top));
	}

} // namespace tools
