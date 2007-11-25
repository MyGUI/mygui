/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	const float WINDOW_ALPHA_ACTIVE = 1.0;
	const float WINDOW_ALPHA_FOCUS = 0.7;
	const float WINDOW_ALPHA_DEACTIVE = 0.3;

	const int WINDOW_TO_STICK = 10;

	Window::Window(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetX(null), mWidgetResize(null), mWidgetClient(null),
		mIsListenerAlpha(false),
		mIsDestroy(false),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(true),
		mIsToStick(false)
	{
		// ��������� �������
		mMinmax.set(50, 50, 2050, 2050);

		// ���������� ������ �����
		IntSize size = _info->getSize();

		// ����� � �������������� ���������
		setAlpha(0.0f);
		setDoAlpha(WINDOW_ALPHA_DEACTIVE);

		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("WindowToStick");
		if (iter != param.end()) mIsToStick = util::parseBool(iter->second);

		// ������ ���������
		mWidgetCaption = parseSubWidget(param, "Button", "SkinCaption", "OffsetCaption", "AlignCaption", size);
		if (mWidgetCaption != null) {
			// �������� ��� �������
			mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);
		}

		// ������ �������
		mWidgetX = parseSubWidget(param, "Button", "SkinX", "OffsetX", "AlignX", size);
		if (mWidgetX != null) {
			// �������� ��� �������
			mWidgetX->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressedX);
		}

		// ������ ��������
		mWidgetResize = parseSubWidget(param, "Button", "SkinResize", "OffsetResize", "AlignResize", size);
		if (mWidgetResize != null) {
			// �������� ��� �������
			mWidgetResize->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetResize->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedResize);
		}

		// ������ ������ � ��������� �������
		mWidgetClient = parseSubWidget(param, "Widget", "SkinClient", "OffsetClient", "AlignClient", size);

	}

	// �������������� ��� ���������� �������
	WidgetPtr Window::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidget(_type, _skin, _left, _top, _width, _height, _align, _name);
		return Widget::createWidget(_type, _skin, _left, _top, _width, _height, _align, _name);
	}

	void Window::_onMouseChangeRootFocus(bool _focus)
	{
		Widget::_onMouseChangeRootFocus(_focus);
		mMouseRootFocus = _focus;
		updateAlpha();
	}

	void Window::_onKeyChangeRootFocus(bool _focus)
	{
		Widget::_onKeyChangeRootFocus(_focus);
		mKeyRootFocus = _focus;
		updateAlpha();
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (!_left) return;
		mPreActionRect.set(mLeft, mTop, mWidth, mHeight);
	}

	void Window::notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left)
	{
		eventWindowXPressed(this);
	}

	void Window::notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		move(mPreActionRect.left + (_left - point.left), mPreActionRect.top + (_top - point.top));
	}

	void Window::notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		size(mPreActionRect.right + (_left - point.left), mPreActionRect.bottom + (_top - point.top));
	}

	void Window::setDoAlpha(float _alpha)
	{
		if (mIsDestroy) return;

		mDoAlpha = _alpha;
		if (mDoAlpha == getAlpha()) {
			// ���� �� ���� ����������, �� ������������
			if (mIsListenerAlpha) {
				Ogre::Root::getSingleton().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
			// �������
			return;
		}

		// ���� ����, �� �������������
		if (!mIsListenerAlpha) {
			Ogre::Root::getSingleton().addFrameListener(this);
			mIsListenerAlpha = true;
		}
	}

	bool Window::frameStarted(const Ogre::FrameEvent& evt)
	{
		const float COEF = 3.0f;
		// ��� ���������� ����� �������
		if (!mIsListenerAlpha) return true;

		float alpha = getAlpha();
		if (alpha == mDoAlpha) {
			Ogre::Root::getSingleton().removeFrameListener(this);
			mIsListenerAlpha = false;
			return true;

		} else if (alpha > mDoAlpha) {
			alpha -= evt.timeSinceLastFrame * COEF;
			if (alpha <= mDoAlpha) {
				alpha = mDoAlpha;
				Ogre::Root::getSingleton().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
		} else {
			alpha += evt.timeSinceLastFrame * COEF;
			if (alpha >= mDoAlpha) {
				alpha = mDoAlpha;
				Ogre::Root::getSingleton().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
		}

		if (alpha == 0.0) {
			if (mIsDestroy) {
				WidgetPtr destroy = this;
				WidgetManager::getInstance().destroyWidget(destroy);
			} else {
				Widget::show(false);
				mEnable = true;
			}
			return true;
		}

		// ������������� ������� �����
		setAlpha(alpha);
		return true;
	}

	bool Window::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

	void Window::show(bool _smoot, bool _reset)
	{
		Widget::show(true);
		mEnable = true; // ��� ���� �� ����� ����������, � ��� ��� � �� ��������
		if (!_smoot) setAlpha(1.0f);
		else if (!_reset) setDoAlpha(1.0f);
		else {
			setAlpha(0.0f);
			setDoAlpha(1.0f);
		}
	}

	void Window::hide(bool _smoot, bool _destroy)
	{
		if (!_smoot) {
			if (_destroy) {
				WidgetPtr destroy = this;
				WidgetManager::getInstance().destroyWidget(destroy);
			} else Widget::show(false);
		} else {
			mEnable = false;
			setDoAlpha(0.0);
			mIsDestroy = _destroy;
			InputManager::getInstance().resetKeyFocusWidget();
			InputManager::getInstance().resetMouseFocusWidget();
		}
	}

	void Window::updateAlpha()
	{
		if (!mIsAutoAlpha) return;

		if (mKeyRootFocus) setDoAlpha(WINDOW_ALPHA_ACTIVE);
		else if (mMouseRootFocus) setDoAlpha(WINDOW_ALPHA_FOCUS);
		else setDoAlpha(WINDOW_ALPHA_DEACTIVE);
	}

	void Window::move(int _left, int _top)
	{
		// ���������� � �����
		if (mIsToStick) {
			if (_left > 0) {if ( (_left - WINDOW_TO_STICK) <= 0) _left = 0;}
			else {if ( (_left + WINDOW_TO_STICK) >= 0) _left = 0;}
			if (_top > 0) {if ( (_top - WINDOW_TO_STICK) <= 0) _top = 0;}
			else {	if ( (_top + WINDOW_TO_STICK) >= 0) _top = 0;}

			int width = Gui::getInstance().getWidth();
			int height = Gui::getInstance().getHeight();

			if ( (_left + mWidth) < width) {if ( (_left + mWidth + WINDOW_TO_STICK) > width ) _left = width - mWidth;	}
			else {	if ( (_left + mWidth - WINDOW_TO_STICK) < width ) _left = width - mWidth;}
			if ( (_top + mHeight) < height) {if ( (_top + mHeight + WINDOW_TO_STICK) > height ) _top = height - mHeight;}
			else {	if ( (_top + mHeight - WINDOW_TO_STICK) < height ) _top = height - mHeight;}
		}
		Widget::move(_left, _top);
	}

	void Window::move(int _left, int _top, int _width, int _height)
	{
		Widget::move(_left, _top, _width, _height);
	}

	void Window::size(int _width, int _height)
	{
		// ���������� � �����
		if (mIsToStick) {
			int width = Gui::getInstance().getWidth();
			int height = Gui::getInstance().getHeight();

			if ( (mLeft + _width) < width) {if ( (mLeft + _width + WINDOW_TO_STICK) > width ) _width = width - mLeft;	}
			else {	if ( (mLeft + _width - WINDOW_TO_STICK) < width ) _width = width - mLeft;}
			if ( (mTop + _height) < height) {if ( (mTop + _height + WINDOW_TO_STICK) > height ) _height = height - mTop;}
			else {	if ( (mTop + _height - WINDOW_TO_STICK) < height ) _height = height - mTop;}
		}

		if (_width < mMinmax.left) _width = mMinmax.left;
		else if (_width > mMinmax.right) _width = mMinmax.right;
		if (_height < mMinmax.top) _height = mMinmax.top;
		else if (_height > mMinmax.bottom) _height = mMinmax.bottom;
		if ((_width == mWidth) && (_height == mHeight) ) return;

		Widget::size(_width, _height);
	}

} // namespace MyGUI