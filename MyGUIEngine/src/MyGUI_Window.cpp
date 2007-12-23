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

	Window::Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetClient(null),
		mIsListenerAlpha(false),
		mIsDestroy(false),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mIsToStick(false)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		// дефолтные размеры
		mMinmax.set(50, 50, 2050, 2050);

		// запоминаем размер скина
		//IntSize size = _info->getSize();

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("WindowToStick");
		if (iter != param.end()) mIsToStick = util::parseBool(iter->second);

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->getInternalString() == "Client") {
				mWidgetClient = (*iter);
			}
			else if ((*iter)->getInternalString() == "Caption") {
				mWidgetCaption = (*iter);
				// делегаты дл€ событий
				mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedAction);
				//mWidgetCaption->setUserString("Scale", child[pos].findValue("scale"));
			}
			else if ((*iter)->getInternalString() == "Button") {
				//(*iter)->setUserString("Button", child[pos].findValue("event"));
				(*iter)->eventMouseButtonClick = newDelegate(this, &Window::notifyPressedButtonEvent);
			}
			else if ((*iter)->getInternalString() == "Action") {
				(*iter)->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				(*iter)->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedAction);
				//(*iter)->setUserString("Scale", child[pos].findValue("scale"));
			}
		}

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
		if (_left) {
			mPreActionCoord = mCoord;
			mCurrentActionScale = IntCoord::parse(_sender->getUserString("Scale"));
		}
	}

	void Window::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (false == _double) eventWindowButtonPressed(this, _sender->getUserString("Button"));
	}

	void Window::notifyMouseMovedAction(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		IntCoord coord = mCurrentActionScale;
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);
		
		setPosition(mPreActionCoord + coord);
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

	void Window::_frameEntered(float _frame)
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
		setPosition(_coord.point());
		setSize(_coord.size());
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

	void Window::hideSmooth()
	{
		// если нужно то запускаем скрытие
		if ((mShow) && (mAlpha != WINDOW_ALPHA_MIN)) {
			// блокируем доступ
			mEnabled = false;
			setDoAlpha(WINDOW_ALPHA_MIN);
		}
	}

	void Window::destroySmooth()
	{
		// мы уже скрыты удал€ем
		if ((false == mShow) || (mAlpha == WINDOW_ALPHA_MIN)) {
			WidgetPtr destroy = this;
			WidgetManager::getInstance().destroyWidget(destroy);
		}
		// доходим до минимума
		else {
			mEnabled = false;
			setDoAlpha(WINDOW_ALPHA_MIN);
			mIsDestroy = true;
			InputManager::getInstance().unlinkWidget(this);
		}
	}

	const IntCoord& Window::getClientRect()
	{
		if (null == mWidgetClient) return Widget::getClientRect();
		return mWidgetClient->getClientRect();
	}

} // namespace MyGUI