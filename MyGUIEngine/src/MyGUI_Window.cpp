/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	const float WINDOW_ALPHA_MAX = 1.0;
	const float WINDOW_ALPHA_MIN = 0.0;
	const float WINDOW_ALPHA_ACTIVE = 1.0;
	const float WINDOW_ALPHA_FOCUS = 0.7;
	const float WINDOW_ALPHA_DEACTIVE = 0.3;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_TO_STICK = 10;

	Window::Window(int _left, int _top, int _width, int _height, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetX(null), mWidgetResize(null), mWidgetClient(null),
		mIsListenerAlpha(false),
		mIsDestroy(false),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mIsToStick(false)
	{
		// дефолтные размеры
		mMinmax.set(50, 50, 2050, 2050);

		// запоминаем размер скина
		IntSize size = _info->getSize();

		// альфа в первоначальное положение
		//setAlpha(0.0f);
		//setDoAlpha(WINDOW_ALPHA_DEACTIVE);

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("WindowToStick");
		if (iter != param.end()) mIsToStick = util::parseBool(iter->second);

		// парсим заголовок
		mWidgetCaption = parseSubWidget(param, "Button", "SkinCaption", "OffsetCaption", "AlignCaption", size);
		if (mWidgetCaption != null) {
			// делегаты дл€ событий
			mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);
		}

		// парсим крестик
		mWidgetX = parseSubWidget(param, "Button", "SkinX", "OffsetX", "AlignX", size);
		if (mWidgetX != null) {
			// делегаты дл€ событий
			mWidgetX->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressedX);
		}

		// парсим ресайзер
		mWidgetResize = parseSubWidget(param, "Button", "SkinResize", "OffsetResize", "AlignResize", size);
		if (mWidgetResize != null) {
			// делегаты дл€ событий
			mWidgetResize->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetResize->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedResize);
		}

		// парсим клиент в последнюю очередь
		mWidgetClient = parseSubWidget(param, "Widget", "SkinClient", "OffsetClient", "AlignClient", size);

	}

	Window::~Window()
	{
		Gui::getInstance().removeFrameListener(this);
	}

	// переопредел€ем дл€ присвоению клиенту
	WidgetPtr Window::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidget(_type, _skin, _left, _top, _width, _height, _align, _name);
		return Widget::createWidget(_type, _skin, _left, _top, _width, _height, _align, _name);
	}

	void Window::_onMouseChangeRootFocus(bool _focus)
	{
		mMouseRootFocus = _focus;
		updateAlpha();

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseChangeRootFocus(_focus);
	}

	void Window::_onKeyChangeRootFocus(bool _focus)
	{
		mKeyRootFocus = _focus;
		updateAlpha();

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onKeyChangeRootFocus(_focus);
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;
		mPreActionRect.set(mLeft, mTop, mWidth, mHeight);
	}

	void Window::notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left)
	{
		eventWindowXPressed(this);
	}

	void Window::notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		setPosition(mPreActionRect.left + (_left - point.left), mPreActionRect.top + (_top - point.top));
	}

	void Window::notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		setSize(mPreActionRect.right + (_left - point.left), mPreActionRect.bottom + (_top - point.top));
	}

	void Window::setDoAlpha(float _alpha)
	{
		if (mIsDestroy) return;

		mDoAlpha = _alpha;
		if (mDoAlpha == getAlpha()) {
			// если мы были подписанны, то отписываемс€
			if (mIsListenerAlpha) {
				Gui::getInstance().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
			// выходим
			return;
		}

		// если надо, то подписываемс€
		if (false == mIsListenerAlpha) {
			Gui::getInstance().addFrameListener(this);
			mIsListenerAlpha = true;
		}
	}

	void Window::frameStarted(float _frame, float _event)
	{
		// огр отписывает после прохода
		if (false == mIsListenerAlpha) return;

		float alpha = getAlpha();
		if (alpha == mDoAlpha) {
			Gui::getInstance().removeFrameListener(this);
			mIsListenerAlpha = false;
			return;

		} else if (alpha > mDoAlpha) {
			alpha -= _frame * WINDOW_SPEED_COEF;
			if (alpha <= mDoAlpha) {
				alpha = mDoAlpha;
				Gui::getInstance().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
		} else {
			alpha += _frame * WINDOW_SPEED_COEF;
			if (alpha >= mDoAlpha) {
				alpha = mDoAlpha;
				Gui::getInstance().removeFrameListener(this);
				mIsListenerAlpha = false;
			}
		}

		if (alpha == WINDOW_ALPHA_MIN) {
			if (mIsDestroy) {
				WidgetPtr destroy = this;
				WidgetManager::getInstance().destroyWidget(destroy);
			} else {
				Widget::hide();
				mEnabled = true;
			}
			return;
		}

		// устанавливаем текущую альфу
		setAlpha(alpha);
	}

	void Window::updateAlpha()
	{
		if (false == mIsAutoAlpha) return;

		if (mKeyRootFocus) setDoAlpha(WINDOW_ALPHA_ACTIVE);
		else if (mMouseRootFocus) setDoAlpha(WINDOW_ALPHA_FOCUS);
		else setDoAlpha(WINDOW_ALPHA_DEACTIVE);
	}

	void Window::setPosition(int _left, int _top)
	{
		// прилепл€ем к кра€м
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
		Widget::setPosition(_left, _top);
	}

	void Window::setPosition(int _left, int _top, int _width, int _height)
	{
		Widget::setPosition(_left, _top, _width, _height);
	}

	void Window::setSize(int _width, int _height)
	{
		// прилепл€ем к кра€м
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

		Widget::setSize(_width, _height);
	}

	// дл€ мееедленного показа и скрыти€
	void Window::showSmooth()
	{
		// разблокируем на вс€кий
		mEnabled = true;

		// если мы с автоальфой, то поднимаем альфу в зависимости от активности
		float doAlpha = (mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX;

		// если мы скрыты, то нуно показать и опустить альфу
		if (false == mShow) {
			setAlpha(0);
			show();
		}

		// поднимаем альфу
		setDoAlpha(doAlpha);
	}

	void Window::hideSmooth(bool _destroy)
	{
		if (_destroy) {
			// мы уже скрыты удал€ем
			if ((false == mShow) || (mAlpha == WINDOW_ALPHA_MIN)) {
				WidgetPtr destroy = this;
				WidgetManager::getInstance().destroyWidget(destroy);
			}
			// доходим до минимума
			else {
				mEnabled = false;
				setDoAlpha(WINDOW_ALPHA_MIN);
				mIsDestroy = _destroy;
				InputManager::getInstance().widgetUnlink(this);
			}
		}
		// удал€ть не надо, просто меедленно скрываем
		else {
			// если нужно то запускаем скрытие
			if ((mShow) && (mAlpha != WINDOW_ALPHA_MIN)) {
				// блокируем доступ
				mEnabled = false;
				setDoAlpha(WINDOW_ALPHA_MIN);
			}
		}
	}

} // namespace MyGUI