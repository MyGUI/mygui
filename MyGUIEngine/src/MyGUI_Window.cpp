/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"

namespace MyGUI
{

	const float WINDOW_ALPHA_MAX = ALPHA_MAX;
	const float WINDOW_ALPHA_MIN = ALPHA_MIN;
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_TO_STICK = 10;
	const std::string WINDOW_CURSOR_POINTER_RESIZE = "size_left";

	Window::Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
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
			mWidgetResize->eventMouseSetFocus = newDelegate(this, &Window::notifyMouseSetFocus);
			mWidgetResize->eventMouseLostFocus = newDelegate(this, &Window::notifyMouseLostFocus);
		}

		// парсим клиент в последнюю очередь
		mWidgetClient = parseSubWidget(param, "Widget", "SkinClient", "OffsetClient", "AlignClient", size);

	}

	Window::~Window()
	{
		Gui::getInstance().removeFrameListener(this);
	}

	// переопредел€ем дл€ присвоению клиенту
	WidgetPtr Window::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidgetT(_type, _skin, _coord, _align, _name);
		return Widget::createWidgetT(_type, _skin, _coord, _align, _name);
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
		mPreActionRect.set(mCoord.left, mCoord.top, mCoord.width, mCoord.height);
	}

	void Window::notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left)
	{
		eventWindowXPressed(this);
	}

	void Window::notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		Widget::setPosition(mPreActionRect.left + (_left - point.left), mPreActionRect.top + (_top - point.top));
	}

	void Window::notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		Widget::setSize(mPreActionRect.right + (_left - point.left), mPreActionRect.bottom + (_top - point.top));
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

	void Window::_frameStarted(float _frame, float _event)
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

	void Window::setPosition(const IntPoint& _pos)
	{
		IntPoint pos = _pos;
		// прилепл€ем к кра€м
		if (mIsToStick) {
			if (pos.left > 0) {if ( (pos.left - WINDOW_TO_STICK) <= 0) pos.left = 0;}
			else {if ( (pos.left + WINDOW_TO_STICK) >= 0) pos.left = 0;}
			if (pos.top > 0) {if ( (pos.top - WINDOW_TO_STICK) <= 0) pos.top = 0;}
			else {	if ( (pos.top + WINDOW_TO_STICK) >= 0) pos.top = 0;}

			int width = Gui::getInstance().getViewWidth();
			int height = Gui::getInstance().getViewHeight();

			if ( (pos.left + mCoord.width) < width) {if ( (pos.left + mCoord.width + WINDOW_TO_STICK) > width ) pos.left = width - mCoord.width;	}
			else {	if ( (pos.left + mCoord.width - WINDOW_TO_STICK) < width ) pos.left = width - mCoord.width;}
			if ( (pos.top + mCoord.height) < height) {if ( (pos.top + mCoord.height + WINDOW_TO_STICK) > height ) pos.top = height - mCoord.height;}
			else {	if ( (pos.top + mCoord.height - WINDOW_TO_STICK) < height ) pos.top = height - mCoord.height;}
		}
		Widget::setPosition(pos);
	}

	void Window::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
	}

	void Window::setSize(const IntSize& _size)
	{
		IntSize size = _size;
		// прилепл€ем к кра€м
		if (mIsToStick) {
			int width = Gui::getInstance().getViewWidth();
			int height = Gui::getInstance().getViewHeight();

			if ( (mCoord.left + size.width) < width) {if ( (mCoord.left + size.width + WINDOW_TO_STICK) > width ) size.width = width - mCoord.left;	}
			else {	if ( (mCoord.left + size.width - WINDOW_TO_STICK) < width ) size.width = width - mCoord.left;}
			if ( (mCoord.top + size.height) < height) {if ( (mCoord.top + size.height + WINDOW_TO_STICK) > height ) size.height = height - mCoord.top;}
			else {	if ( (mCoord.top + size.height - WINDOW_TO_STICK) < height ) size.height = height - mCoord.top;}
		}

		if (size.width < mMinmax.left) size.width = mMinmax.left;
		else if (size.width > mMinmax.right) size.width = mMinmax.right;
		if (size.height < mMinmax.top) size.height = mMinmax.top;
		else if (size.height > mMinmax.bottom) size.height = mMinmax.bottom;
		if ((size.width == mCoord.width) && (size.height == mCoord.height) ) return;

		Widget::setSize(size);
	}

	// дл€ мееедленного показа и скрыти€
	void Window::showSmooth(bool _reset)
	{
		// разблокируем на вс€кий
		mEnabled = true;

		// если мы с автоальфой, то поднимаем альфу в зависимости от активности
		float doAlpha = (mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX;

		// если мы скрыты, то нуно показать и опустить альфу
		if ((false == mShow) || (_reset)) {
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
				InputManager::getInstance().unlinkWidget(this);
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

	void Window::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if (mWidgetResize == _sender) PointerManager::getInstance().setPointer(WINDOW_CURSOR_POINTER_RESIZE, this);
	}

	void Window::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if (mWidgetResize == _sender) PointerManager::getInstance().defaultPointer();
	}

	const IntCoord& Window::getClientRect()
	{
		if (null == mWidgetClient) return Widget::getClientRect();
		return mWidgetClient->getClientRect();
	}

} // namespace MyGUI