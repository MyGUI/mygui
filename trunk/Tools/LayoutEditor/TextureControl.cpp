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

	TextureControlLE::TextureControlLE(const std::string& _layout, MyGUI::Widget* _parent) :
		wraps::BaseLayout(_layout, _parent),
		mView(nullptr),
		mTexture(nullptr),
		mBackground(nullptr),
		mScaleValue(1.0),
		mMouseCapture(false),
		mMouseLeftPressed(false)
	{
		assignWidget(mView, "View");
		assignWidget(mTexture, "Texture");
		assignWidget(mBackground, "Background");

		mTexture->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonReleased);
		mTexture->eventMouseDrag += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseDrag);
		mTexture->eventMouseMove += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseMove);
		mTexture->eventMouseWheel += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseWheel);
	}

	TextureControlLE::~TextureControlLE()
	{
		mTexture->eventMouseWheel -= MyGUI::newDelegate(this, &TextureControlLE::notifyMouseWheel);
		mTexture->eventMouseButtonPressed -= MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonPressed);
		mTexture->eventMouseButtonReleased -= MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonReleased);
		mTexture->eventMouseDrag -= MyGUI::newDelegate(this, &TextureControlLE::notifyMouseDrag);
		mTexture->eventMouseMove -= MyGUI::newDelegate(this, &TextureControlLE::notifyMouseMove);
	}

	void TextureControlLE::updateScale()
	{
		double width = (double)mTextureRegion.width * mScaleValue;
		double height = (double)mTextureRegion.height * mScaleValue;

		mView->setCanvasSize(MyGUI::IntSize((int)width, (int)height));

		for (std::vector<SelectorControlLE*>::iterator item = mSelectors.begin(); item != mSelectors.end(); ++item)
			(*item)->setScale(mScaleValue);
	}

	void TextureControlLE::setTextureName(const MyGUI::UString& _value)
	{
		mTextureSize = MyGUI::texture_utility::getTextureSize(_value);
		mTexture->setImageTexture(_value);

		mTextureRegion.set(0, 0, mTextureSize.width, mTextureSize.height);

		updateScale();
	}

	const MyGUI::IntSize& TextureControlLE::getTextureSize() const
	{
		return mTextureSize;
	}

	void TextureControlLE::setTextureRegion(const MyGUI::IntCoord& _value)
	{
		mTextureRegion = _value;
		mTexture->setImageCoord(mTextureRegion);
		mTexture->setImageTile(mTextureRegion.size());
		mTexture->setImageIndex(0);

		updateScale();
	}

	const MyGUI::IntCoord& TextureControlLE::getTextureRegion() const
	{
		return mTextureRegion;
	}

	void TextureControlLE::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
	}

	void TextureControlLE::setColour(MyGUI::Colour _value)
	{
		mCurrentColour = _value;
		updateColours();
	}

	MyGUI::Colour TextureControlLE::getColour() const
	{
		return mCurrentColour;
	}

	void TextureControlLE::setScale(double _value)
	{
		mScaleValue = _value;
		updateScale();

		onChangeScale();
	}

	void TextureControlLE::onMouseMove()
	{
	}

	void TextureControlLE::onMouseWheel(int _rel)
	{
	}

	void TextureControlLE::onMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControlLE::onMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControlLE::onMouseDrag(const MyGUI::IntPoint& _point)
	{
	}

	void TextureControlLE::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
	}

	bool TextureControlLE::getSelectorsCapture()
	{
		if (mMouseCapture)
			return true;

		for (std::vector<SelectorControlLE*>::iterator item = mSelectors.begin(); item != mSelectors.end(); ++item)
			if ((*item)->getCapture())
				return true;
		return false;
	}

	void TextureControlLE::notifyMouseWheel(MyGUI::Widget* _sender, int _rel)
	{
		mMouseLeftPressed = false;

		if (!getSelectorsCapture())
		{
			saveMouseRelative();
			onMouseWheel(_rel);
			loadMouseRelative();
		}
	}

	void TextureControlLE::registerSelectorControl(SelectorControlLE* _control)
	{
		mSelectors.push_back(_control);
		_control->setScale(mScaleValue);
		_control->getMainWidget()->eventMouseWheel += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseWheel);
		_control->getMainWidget()->eventMouseButtonPressed += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonPressed);
		_control->getMainWidget()->eventMouseButtonReleased += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseButtonReleased);
		_control->getMainWidget()->eventMouseDrag += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseDrag);
		_control->getMainWidget()->eventMouseMove += MyGUI::newDelegate(this, &TextureControlLE::notifyMouseMove);
	}

	void TextureControlLE::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
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
		else if (_id == MyGUI::MouseButton::Left)
		{
			mMouseLeftPressed = true;
			onMouseButtonPressed(getMousePosition());
		}
	}

	void TextureControlLE::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mMouseCapture = false;

			mTexture->setPointer("arrow");
			MyGUI::PointerManager::getInstance().setPointer("arrow");
			MyGUI::PointerManager::getInstance().eventChangeMousePointer("arrow");
		}
		else if (_id == MyGUI::MouseButton::Left)
		{
			if (!mMouseCapture && mMouseLeftPressed)
			{
				mMouseLeftPressed = false;
				onMouseButtonClick(getMousePosition());
			}
			onMouseButtonReleased(getMousePosition());
		}
	}

	void TextureControlLE::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		mMouseLeftPressed = false;

		if (_id == MyGUI::MouseButton::Right)
		{
			MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
			MyGUI::IntPoint mouseOffset = mousePoint - mRightMouseClick;

			MyGUI::IntPoint offset = mViewOffset + mouseOffset;
			mView->setViewOffset(offset);
		}
		else if (_id == MyGUI::MouseButton::Left)
		{
			onMouseDrag(getMousePosition());
		}
	}

	void TextureControlLE::notifyMouseMove(MyGUI::Widget* _sender, int _left, int _top)
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getLastPressedPosition(MyGUI::MouseButton::Left);
		if (point.left != _left && point.top != _top)
			onMouseMove();
	}

	void TextureControlLE::saveMouseRelative()
	{
		MyGUI::IntSize canvasSize = mView->getCanvasSize();
		MyGUI::IntPoint mousePoint = MyGUI::InputManager::getInstance().getMousePositionByLayer();
		MyGUI::IntPoint mouseOffset = mousePoint - mTexture->getAbsolutePosition();

		mMouseRelative.set((float)mouseOffset.left / (float)canvasSize.width, (float)mouseOffset.top / (float)canvasSize.height);
	}

	void TextureControlLE::loadMouseRelative()
	{
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

	MyGUI::IntPoint TextureControlLE::getMousePosition()
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition() - mTexture->getAbsolutePosition();
		point.left = (int)((double)point.left / mScaleValue);
		point.top = (int)((double)point.top / mScaleValue);

		return point;
	}

	void TextureControlLE::onChangeScale()
	{
	}

	double TextureControlLE::getScale() const
	{
		return mScaleValue;
	}

}
