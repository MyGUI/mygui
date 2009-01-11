/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Precompiled.h"
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

	const float WINDOW_ALPHA_MAX = ALPHA_MAX;
	const float WINDOW_ALPHA_MIN = ALPHA_MIN;
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_SNAP_DISTANSE = 10;

	Window::Window(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mWidgetCaption(nullptr),
		mMouseRootFocus(false), mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mSnap(false)
	{
		initialiseWidgetSkin(_info);
	}

	Window::~Window()
	{
		shutdownWidgetSkin();
	}

	void Window::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void Window::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		// дефолтные размеры
		mMinmax.set(0, 0, 3000, 3000);

		// парсим свойства
		const MapString & properties = _info->getProperties();
		MapString::const_iterator iter = properties.find("Snap");
		if (iter != properties.end()) mSnap = utility::parseBool(iter->second);
		iter = properties.find("MainMove");
		if (iter != properties.end()) setUserString("Scale", "1 1 0 0");

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Caption") {
				MYGUI_DEBUG_ASSERT( ! mWidgetCaption, "widget already assigned");
				mWidgetCaption = (*iter);
				mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				mWidgetCaption->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Button") {
				(*iter)->eventMouseButtonClick = newDelegate(this, &Window::notifyPressedButtonEvent);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Action") {
				(*iter)->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
				(*iter)->eventMouseDrag = newDelegate(this, &Window::notifyMouseDrag);
			}
		}

	}

	void Window::shutdownWidgetSkin()
	{
		mWidgetClient = nullptr;
		mWidgetCaption = nullptr;
	}

	// переопределяем для присвоению клиенту
	WidgetPtr Window::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if (mWidgetClient != nullptr) return mWidgetClient->createWidgetT(_style, _type, _skin, _coord, _align, _layer, _name);
		return Widget::baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name);
	}

	void Window::onMouseChangeRootFocus(bool _focus)
	{
		mMouseRootFocus = _focus;
		updateAlpha();

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseChangeRootFocus(_focus);
	}

	void Window::onKeyChangeRootFocus(bool _focus)
	{
		mKeyRootFocus = _focus;
		updateAlpha();

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onKeyChangeRootFocus(_focus);
	}

	void Window::onMouseDrag(int _left, int _top)
	{
		// на тот случай, если двигать окно, можно за любое место виджета
		notifyMouseDrag(this, _left, _top);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseDrag(_left, _top);
	}

	void Window::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		notifyMousePressed(this, _left, _top, _id);
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left == _id) {
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

		setCoord(mPreActionCoord + coord);
		
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

	void Window::setPosition(const IntPoint & _point)
	{
		IntPoint pos = _point;
		// прилепляем к краям
		if (mSnap) {
			if (abs(pos.left) <= WINDOW_SNAP_DISTANSE) pos.left = 0;
			if (abs(pos.top) <= WINDOW_SNAP_DISTANSE) pos.top = 0;

			int width = (int)Gui::getInstance().getViewWidth();
			int height = (int)Gui::getInstance().getViewHeight();

			if ( abs(pos.left + mCoord.width - width) < WINDOW_SNAP_DISTANSE) pos.left = width - mCoord.width;
			if ( abs(pos.top + mCoord.height - height) < WINDOW_SNAP_DISTANSE) pos.top = height - mCoord.height;
		}
		Widget::setPosition(_point);
	}

	void Window::setSize(const IntSize& _size)
	{
		IntSize size = _size;
		// прилепляем к краям
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

	void Window::setCoord(const IntCoord & _coord)
	{
		IntPoint pos = _coord.point();
		IntSize size = _coord.size();
		// прилепляем к краям
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

		Widget::setCoord(coord);
	}

	// для мееедленного показа и скрытия
	void Window::showSmooth(bool _reset)
	{
		if (_reset) {
			setAlpha(ALPHA_MIN);
			show();
		}

		ControllerFadeAlpha * controller = new ControllerFadeAlpha((mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : WINDOW_ALPHA_MAX, WINDOW_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::actionWidgetHide(WidgetPtr _widget)
	{
		_widget->hide();
		_widget->setEnabled(true);
	}

	void Window::hideSmooth()
	{
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(this, &Window::actionWidgetHide);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::destroySmooth()
	{
		ControllerFadeAlpha * controller = new ControllerFadeAlpha(WINDOW_ALPHA_MIN, WINDOW_SPEED_COEF, false);
		controller->eventPostAction = newDelegate(action::actionWidgetDestroy);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::setCaption(const Ogre::UTFString & _caption)
	{
		if (mWidgetCaption != nullptr) mWidgetCaption->setCaption(_caption);
		else Widget::setCaption(_caption);
	}

	const Ogre::UTFString & Window::getCaption()
	{
		if (mWidgetCaption != nullptr) return mWidgetCaption->getCaption();
		return Widget::getCaption();
	}

	void Window::setTextAlign(Align _align)
	{
		if (mWidgetCaption != nullptr) mWidgetCaption->setTextAlign(_align);
		else Widget::setTextAlign(_align);
	}

	Align Window::getTextAlign()
	{
		if (mWidgetCaption != nullptr) return mWidgetCaption->getTextAlign();
		return Widget::getTextAlign();
	}

	void Window::setColour(const Colour& _colour)
	{
		if (mWidgetCaption != nullptr) mWidgetCaption->setColour(_colour);
		else Widget::setColour(_colour);
	}

	const Colour& Window::getColour()
	{
		if (mWidgetCaption != nullptr) return mWidgetCaption->getColour();
		return Widget::getColour();
	}

	void Window::setFontName(const Ogre::String & _font)
	{
		if (mWidgetCaption != nullptr) mWidgetCaption->setFontName(_font);
		else Widget::setFontName(_font);
	}

	const Ogre::String & Window::getFontName()
	{
		if (mWidgetCaption != nullptr) return mWidgetCaption->getFontName();
		return Widget::getFontName();
	}

	void Window::setFontHeight(uint16 _height)
	{
		if (mWidgetCaption != nullptr) mWidgetCaption->setFontHeight(_height);
		else Widget::setFontHeight(_height);
	}

	uint16 Window::getFontHeight()
	{
		if (mWidgetCaption != nullptr) return mWidgetCaption->getFontHeight();
		return Widget::getFontHeight();
	}

} // namespace MyGUI
