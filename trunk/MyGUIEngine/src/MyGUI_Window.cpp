/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Window.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	Ogre::String Window::WidgetTypeName = "Window";

	const float WINDOW_ALPHA_MAX = ALPHA_MAX;
	const float WINDOW_ALPHA_MIN = ALPHA_MIN;
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_SNAP_DISTANSE = 10;

	Window::Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mWidgetCaption(null),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mSnap(false)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		// дефолтные размеры
		mMinmax.set(0, 0, 3000, 3000);

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("Snap");
		if (iter != param.end()) mSnap = utility::parseBool(iter->second);
		iter = param.find("MainMove");
		if (iter != param.end()) setUserString("Scale", "1 1 0 0");

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
			}
			else if ((*iter)->_getInternalString() == "Caption") {
				mWidgetCaption = (*iter);
				mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				mWidgetCaption->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
			}
			else if ((*iter)->_getInternalString() == "Button") {
				(*iter)->eventMouseButtonClick = newDelegate(this, &Window::notifyPressedButtonEvent);
			}
			else if ((*iter)->_getInternalString() == "Action") {
				(*iter)->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				(*iter)->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
			}
		}

	}

	// переопредел€ем дл€ присвоению клиенту
	WidgetPtr Window::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidgetT(_type, _skin, _coord, _align, _name);
		return Widget::_createWidget(_type, _skin, _coord, _align, _layer, _name);
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

	void Window::_onMouseDrag(int _left, int _top)
	{
		// на тот случай, если двигать окно, можно за любое место виджета
		notifyMouseDrag(this, _left, _top);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseDrag(_left, _top);
	}

	void Window::_onMouseButtonPressed(bool _left)
	{
		notifyMousePressed(this, _left);
		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonPressed(_left);
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (_left) {
			mPreActionCoord = mCoord;
			mCurrentActionScale = IntCoord::parse(_sender->getUserString("Scale"));
		}
	}

	void Window::notifyPressedButtonEvent(MyGUI::WidgetPtr _sender)
	{
		eventWindowButtonPressed(this, _sender->getUserString("Event"));
	}

	void Window::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		IntCoord coord = mCurrentActionScale;
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);

		setPosition(mPreActionCoord + coord);
		
		// посылаем событие о изменении позиции и размере
		eventWindowChangeCoord(this);
	}

	void Window::updateAlpha()
	{
		if (false == mIsAutoAlpha) return;

		float alpha;
		if (mKeyRootFocus) alpha = WINDOW_ALPHA_ACTIVE;
		else if (mMouseRootFocus) alpha = WINDOW_ALPHA_FOCUS;
		else alpha = WINDOW_ALPHA_DEACTIVE;

		ControllerFadeAlpha * controller = new ControllerFadeAlpha(alpha, WINDOW_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::setAutoAlpha(bool _auto)
	{
		mIsAutoAlpha = _auto;
		if (false == _auto) setAlpha(ALPHA_MAX);
		else {
			if (mKeyRootFocus) setAlpha(WINDOW_ALPHA_ACTIVE);
			else if (mMouseRootFocus) setAlpha(WINDOW_ALPHA_FOCUS);
			else setAlpha(WINDOW_ALPHA_DEACTIVE);
		}
	}

	void Window::setPosition(const IntPoint& _pos)
	{
		IntPoint pos = _pos;
		// прилепл€ем к кра€м
		if (mSnap) {
			if (abs(pos.left) <= WINDOW_SNAP_DISTANSE) pos.left = 0;
			if (abs(pos.top) <= WINDOW_SNAP_DISTANSE) pos.top = 0;

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(pos.left + mCoord.width - width) < WINDOW_SNAP_DISTANSE) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < WINDOW_SNAP_DISTANSE) pos.top = height - mCoord.height;
		}
		Widget::setPosition(pos);
	}

	void Window::setPosition(const IntCoord& _coord)
	{
		IntPoint pos = _coord.point();
		IntSize size = _coord.size();
		// прилепл€ем к кра€м
		if (mSnap) {
			if (abs(pos.left) <= WINDOW_SNAP_DISTANSE) pos.left = 0;
			if (abs(pos.top) <= WINDOW_SNAP_DISTANSE) pos.top = 0;

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(pos.left + mCoord.width - width) < WINDOW_SNAP_DISTANSE) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < WINDOW_SNAP_DISTANSE) pos.top = height - mCoord.height;

			if ( abs(mCoord.left + size.width - width) < WINDOW_SNAP_DISTANSE) size.width = width - mCoord.left;
			if ( abs(mCoord.top + size.height - height) < WINDOW_SNAP_DISTANSE) size.height = height - mCoord.top;
		}

		if (size.width < mMinmax.left) {
			int offset = mMinmax.left - size.width;
			size.width = mMinmax.left;
			if ((pos.left - mCoord.left) > offset) pos.left -= offset;
			else pos.left = mCoord.left;
		}
		else if (size.width > mMinmax.right) {
			int offset = mMinmax.right - size.width;
			size.width = mMinmax.right;
			if ((pos.left - mCoord.left) < offset) pos.left -= offset;
			else pos.left = mCoord.left;
		}
		if (size.height < mMinmax.top) {
			int offset = mMinmax.top - size.height;
			size.height = mMinmax.top;
			if ((pos.top - mCoord.top) > offset) pos.top -= offset;
			else pos.top = mCoord.top;
		}
		else if (size.height > mMinmax.bottom) {
			int offset = mMinmax.bottom - size.height;
			size.height = mMinmax.bottom;
			if ((pos.top - mCoord.top) < offset) pos.top -= offset;
			else pos.top = mCoord.top;
		}

		IntCoord coord(pos, size);
		if (coord == mCoord) return;

		Widget::setPosition(coord);
	}

	void Window::setSize(const IntSize& _size)
	{
		IntSize size = _size;
		// прилепл€ем к кра€м
		if (mSnap) {
			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(mCoord.left + size.width - width) < WINDOW_SNAP_DISTANSE) size.width = width - mCoord.left;
			if ( abs(mCoord.top + size.height - height) < WINDOW_SNAP_DISTANSE) size.height = height - mCoord.top;
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
		if (_reset) {
			setAlpha(ALPHA_MIN);
			show();
		}

		ControllerFadeAlpha * controller = new ControllerFadeAlpha((mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX, WINDOW_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::hideSmooth()
	{
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::destroySmooth()
	{
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetDestroy);
		ControllerManager::getInstance().addItem(this, controller);
	}

	VectorWidgetPtr Window::getChilds()
	{
		if (null == mWidgetClient) return Widget::getChilds();
		return mWidgetClient->getChilds();
	}

	void Window::setCaption(const Ogre::UTFString & _caption)
	{
		if (mWidgetCaption != null) mWidgetCaption->setCaption(_caption);
		else Widget::setCaption(_caption);
	}

	const Ogre::UTFString & Window::getCaption()
	{
		if (mWidgetCaption != null) return mWidgetCaption->getCaption();
		return Widget::getCaption();
	}

	void Window::setTextAlign(Align _align)
	{
		if (mWidgetCaption != null) mWidgetCaption->setTextAlign(_align);
		else Widget::setTextAlign(_align);
	}

	Align Window::getTextAlign()
	{
		if (mWidgetCaption != null) return mWidgetCaption->getTextAlign();
		return Widget::getTextAlign();
	}

	void Window::setColour(const Ogre::ColourValue & _colour)
	{
		if (mWidgetCaption != null) mWidgetCaption->setColour(_colour);
		else Widget::setColour(_colour);
	}

	const Ogre::ColourValue & Window::getColour()
	{
		if (mWidgetCaption != null) return mWidgetCaption->getColour();
		return Widget::getColour();
	}

	void Window::setFontName(const Ogre::String & _font)
	{
		if (mWidgetCaption != null) mWidgetCaption->setFontName(_font);
		else Widget::setFontName(_font);
	}

	const Ogre::String & Window::getFontName()
	{
		if (mWidgetCaption != null) return mWidgetCaption->getFontName();
		return Widget::getFontName();
	}

	void Window::setFontHeight(uint16 _height)
	{
		if (mWidgetCaption != null) mWidgetCaption->setFontHeight(_height);
		else Widget::setFontHeight(_height);
	}

	uint16 Window::getFontHeight()
	{
		if (mWidgetCaption != null) return mWidgetCaption->getFontHeight();
		return Widget::getFontHeight();
	}

} // namespace MyGUI
