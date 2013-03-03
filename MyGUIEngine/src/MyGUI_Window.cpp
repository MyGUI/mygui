/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Window.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{
	const float WINDOW_ALPHA_ACTIVE = ALPHA_MAX;
	const float WINDOW_ALPHA_FOCUS = 0.7f;
	const float WINDOW_ALPHA_DEACTIVE = 0.3f;
	const float WINDOW_SPEED_COEF = 3.0f;

	const int WINDOW_SNAP_DISTANSE = 10;

	Window::Window() :
		mWidgetCaption(nullptr),
		mMouseRootFocus(false),
		mKeyRootFocus(false),
		mIsAutoAlpha(false),
		mSnap(false),
		mAnimateSmooth(false),
		mClient(nullptr),
		mMovable(true)
	{
	}

	void Window::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		// дефолтные размеры
		mMinmax.set(
			(std::numeric_limits<int>::min)(),
			(std::numeric_limits<int>::min)(),
			(std::numeric_limits<int>::max)(),
			(std::numeric_limits<int>::max)());

		bool main_move = false;
		if (isUserString("MainMove"))
		{
			setUserString("Scale", "1 1 0 0");
			main_move = true;
		}

		///@wskin_child{Window, Widget, Client} Клиентская зона.
		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			if (main_move)
			{
				mClient->setUserString("Scale", "1 1 0 0");
				mClient->eventMouseButtonPressed += newDelegate(this, &Window::notifyMousePressed);
				mClient->eventMouseButtonReleased += newDelegate(this, &Window::notifyMouseReleased);
				mClient->eventMouseDrag += newDelegate(this, &Window::notifyMouseDrag);
			}
			setWidgetClient(mClient);
		}

		///@wskin_child{Window, TextBox, Caption} Caption for window.
		assignWidget(mWidgetCaption, "Caption");
		if (mWidgetCaption != nullptr)
		{
			mWidgetCaption->setUserString("Scale", "1 1 0 0");
			mWidgetCaption->eventMouseButtonPressed += newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseButtonReleased += newDelegate(this, &Window::notifyMouseReleased);
			mWidgetCaption->eventMouseDrag += newDelegate(this, &Window::notifyMouseDrag);
		}

		VectorWidgetPtr buttons = getSkinWidgetsByName("Button");
		for (VectorWidgetPtr::iterator iter = buttons.begin(); iter != buttons.end(); ++iter)
		{
			(*iter)->eventMouseButtonClick += newDelegate(this, &Window::notifyPressedButtonEvent);
		}

		VectorWidgetPtr actions = getSkinWidgetsByName("Action");
		for (VectorWidgetPtr::iterator iter = actions.begin(); iter != actions.end(); ++iter)
		{
			(*iter)->eventMouseButtonPressed += newDelegate(this, &Window::notifyMousePressed);
			(*iter)->eventMouseButtonReleased += newDelegate(this, &Window::notifyMouseReleased);
			(*iter)->eventMouseDrag += newDelegate(this, &Window::notifyMouseDrag);
			(*iter)->eventMouseWheel += newDelegate(this, &Window::notifyMouseWheel);
		}

		const size_t countNames = 8;
		const char* resizers[2][countNames] =
		{
			{"ResizeLeftTop", "ResizeTop", "ResizeRightTop", "ResizeRight", "ResizeRightBottom", "ResizeBottom", "ResizeLeftBottom", "ResizeLeft"},
			{"Left Top", "Top", "Right Top", "Right", "Right Bottom", "Bottom", "Left Bottom", "Left"}
		};

		for (size_t index = 0; index < countNames; ++ index)
		{
			Widget* widget = nullptr;
			assignWidget(widget, resizers[0][index]);
			if (widget != nullptr)
			{
				widget->eventMouseButtonPressed += newDelegate(this, &Window::notifyMousePressed);
				widget->eventMouseButtonReleased += newDelegate(this, &Window::notifyMouseReleased);
				widget->eventMouseDrag += newDelegate(this, &Window::notifyMouseDrag);
				widget->eventMouseWheel += newDelegate(this, &Window::notifyMouseWheel);
				widget->setUserString("Action", resizers[1][index]);
			}
		}
	}

	void Window::shutdownOverride()
	{
		mClient = nullptr;
		mWidgetCaption = nullptr;

		Base::shutdownOverride();
	}

	void Window::onMouseChangeRootFocus(bool _focus)
	{
		mMouseRootFocus = _focus;
		updateAlpha();

		Base::onMouseChangeRootFocus(_focus);
	}

	void Window::onKeyChangeRootFocus(bool _focus)
	{
		mKeyRootFocus = _focus;
		updateAlpha();

		Base::onKeyChangeRootFocus(_focus);
	}

	void Window::onMouseDrag(int _left, int _top, MouseButton _id)
	{
		// на тот случай, если двигать окно, можно за любое место виджета
		notifyMouseDrag(this, _left, _top, _id);

		Base::onMouseDrag(_left, _top, _id);
	}

	void Window::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		notifyMousePressed(this, _left, _top, _id);

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void Window::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		notifyMouseReleased(this, _left, _top, _id);

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void Window::notifyMousePressed(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left == _id)
		{
			mPreActionCoord = mCoord;
			mCurrentActionScale = _getActionScale(_sender);
		}
	}

	void Window::notifyPressedButtonEvent(MyGUI::Widget* _sender)
	{
		eventWindowButtonPressed(this, _sender->getUserString("Event"));
	}

	void Window::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (_id != MouseButton::Left)
			return;

		const IntPoint& point = InputManager::getInstance().getLastPressedPosition(MouseButton::Left);

		IntCoord coord = mCurrentActionScale;
		coord.left *= (_left - point.left);
		coord.top *= (_top - point.top);
		coord.width *= (_left - point.left);
		coord.height *= (_top - point.top);

		if (coord.empty())
			return;

		if (coord.left == 0 && coord.top == 0)
			setSize((mPreActionCoord + coord).size());
		else if (coord.width == 0 && coord.height == 0)
			setPosition((mPreActionCoord + coord).point());
		else
			setCoord(mPreActionCoord + coord);

		// посылаем событие о изменении позиции и размере
		eventWindowChangeCoord(this);
	}

	void Window::updateAlpha()
	{
		if (!mIsAutoAlpha)
			return;

		float alpha;
		if (mKeyRootFocus)
			alpha = WINDOW_ALPHA_ACTIVE;
		else if (mMouseRootFocus)
			alpha = WINDOW_ALPHA_FOCUS;
		else
			alpha = WINDOW_ALPHA_DEACTIVE;

		ControllerFadeAlpha* controller = createControllerFadeAlpha(alpha, WINDOW_SPEED_COEF, true);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::setAutoAlpha(bool _auto)
	{
		mIsAutoAlpha = _auto;
		if (!_auto)
			setAlpha(ALPHA_MAX);
		else
		{
			if (mKeyRootFocus)
				setAlpha(WINDOW_ALPHA_ACTIVE);
			else if (mMouseRootFocus)
				setAlpha(WINDOW_ALPHA_FOCUS);
			else
				setAlpha(WINDOW_ALPHA_DEACTIVE);
		}
	}

	void Window::setPosition(const IntPoint& _point)
	{
		IntPoint point = _point;
		// прилепляем к краям
		if (mSnap)
		{
			IntCoord coord(point, mCoord.size());
			getSnappedCoord(coord);
			point = coord.point();
		}

		Base::setPosition(point);
	}

	void Window::setSize(const IntSize& _size)
	{
		IntSize size = _size;
		// прилепляем к краям

		if (size.width < mMinmax.left)
			size.width = mMinmax.left;
		else if (size.width > mMinmax.right)
			size.width = mMinmax.right;
		if (size.height < mMinmax.top)
			size.height = mMinmax.top;
		else if (size.height > mMinmax.bottom)
			size.height = mMinmax.bottom;
		if ((size.width == mCoord.width) && (size.height == mCoord.height))
			return;

		if (mSnap)
		{
			IntCoord coord(mCoord.point(), size);
			getSnappedCoord(coord);
			size = coord.size();
		}

		Base::setSize(size);
	}

	void Window::setCoord(const IntCoord& _coord)
	{
		IntPoint pos = _coord.point();
		IntSize size = _coord.size();

		if (size.width < mMinmax.left)
		{
			int offset = mMinmax.left - size.width;
			size.width = mMinmax.left;
			if ((pos.left - mCoord.left) > offset)
				pos.left -= offset;
			else
				pos.left = mCoord.left;
		}
		else if (size.width > mMinmax.right)
		{
			int offset = mMinmax.right - size.width;
			size.width = mMinmax.right;
			if ((pos.left - mCoord.left) < offset)
				pos.left -= offset;
			else
				pos.left = mCoord.left;
		}
		if (size.height < mMinmax.top)
		{
			int offset = mMinmax.top - size.height;
			size.height = mMinmax.top;
			if ((pos.top - mCoord.top) > offset)
				pos.top -= offset;
			else
				pos.top = mCoord.top;
		}
		else if (size.height > mMinmax.bottom)
		{
			int offset = mMinmax.bottom - size.height;
			size.height = mMinmax.bottom;
			if ((pos.top - mCoord.top) < offset)
				pos.top -= offset;
			else
				pos.top = mCoord.top;
		}

		// прилепляем к краям
		if (mSnap)
		{
			IntCoord coord(pos, size);
			getSnappedCoord(coord);
			size = coord.size();
		}

		IntCoord coord(pos, size);
		if (coord == mCoord)
			return;

		Base::setCoord(coord);
	}

	void Window::setCaption(const UString& _caption)
	{
		if (mWidgetCaption != nullptr)
			mWidgetCaption->setCaption(_caption);
		else
			Base::setCaption(_caption);
	}

	const UString& Window::getCaption()
	{
		if (mWidgetCaption != nullptr)
			return mWidgetCaption->getCaption();
		return Base::getCaption();
	}

	void Window::destroySmooth()
	{
		ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MIN, WINDOW_SPEED_COEF, false);
		controller->eventPostAction += newDelegate(action::actionWidgetDestroy);
		ControllerManager::getInstance().addItem(this, controller);
	}

	void Window::animateStop(Widget* _widget, ControllerItem* _controller)
	{
		if (mAnimateSmooth)
		{
			ControllerManager::getInstance().removeItem(this);
			mAnimateSmooth = false;
		}
	}

	void Window::setVisible(bool _visible)
	{
		if (mAnimateSmooth)
		{
			ControllerManager::getInstance().removeItem(this);
			setAlpha(getAlphaVisible());
			setEnabledSilent(true);
			mAnimateSmooth = false;
		}

		Base::setVisible(_visible);
	}

	float Window::getAlphaVisible() const
	{
		return (mIsAutoAlpha && !mKeyRootFocus) ? WINDOW_ALPHA_DEACTIVE : ALPHA_MAX;
	}

	void Window::getSnappedCoord(IntCoord& _coord)
	{
		if (abs(_coord.left) <= WINDOW_SNAP_DISTANSE) _coord.left = 0;
		if (abs(_coord.top) <= WINDOW_SNAP_DISTANSE) _coord.top = 0;

		const IntSize view_size = getParentSize();

		if ( abs(_coord.left + _coord.width - view_size.width) < WINDOW_SNAP_DISTANSE)
			_coord.left = view_size.width - _coord.width;
		if ( abs(_coord.top + _coord.height - view_size.height) < WINDOW_SNAP_DISTANSE)
			_coord.top = view_size.height - _coord.height;
	}

	void Window::setVisibleSmooth(bool _visible)
	{
		mAnimateSmooth = true;
		ControllerManager::getInstance().removeItem(this);

		if (_visible)
		{
			setEnabledSilent(true);
			if (!getVisible())
			{
				setAlpha(ALPHA_MIN);
				Base::setVisible(true);
			}
			ControllerFadeAlpha* controller = createControllerFadeAlpha(getAlphaVisible(), WINDOW_SPEED_COEF, true);
			controller->eventPostAction += newDelegate(this, &Window::animateStop);
			ControllerManager::getInstance().addItem(this, controller);
		}
		else
		{
			setEnabledSilent(false);
			ControllerFadeAlpha* controller = createControllerFadeAlpha(ALPHA_MIN, WINDOW_SPEED_COEF, false);
			controller->eventPostAction += newDelegate(action::actionWidgetHide);
			ControllerManager::getInstance().addItem(this, controller);
		}
	}

	ControllerFadeAlpha* Window::createControllerFadeAlpha(float _alpha, float _coef, bool _enable)
	{
		ControllerItem* item = ControllerManager::getInstance().createItem(ControllerFadeAlpha::getClassTypeName());
		ControllerFadeAlpha* controller = item->castType<ControllerFadeAlpha>();

		controller->setAlpha(_alpha);
		controller->setCoef(_coef);
		controller->setEnabled(_enable);

		return controller;
	}

	void Window::setMinSize(const IntSize& _value)
	{
		mMinmax.left = _value.width;
		mMinmax.top = _value.height;
	}

	IntSize Window::getMinSize()
	{
		return IntSize(mMinmax.left, mMinmax.top);
	}

	void Window::setMaxSize(const IntSize& _value)
	{
		mMinmax.right = _value.width;
		mMinmax.bottom = _value.height;
	}

	IntSize Window::getMaxSize()
	{
		return IntSize(mMinmax.right, mMinmax.bottom);
	}

	void Window::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{Window, AutoAlpha, bool} Режим регулировки прозрачности опираясь на фокус ввода.
		if (_key == "AutoAlpha")
			setAutoAlpha(utility::parseValue<bool>(_value));

		/// @wproperty{Window, Snap, bool} Режим прилипания к краям экрана.
		else if (_key == "Snap")
			setSnap(utility::parseValue<bool>(_value));

		/// @wproperty{Window, MinSize, int int} Минимальный размер окна.
		else if (_key == "MinSize")
			setMinSize(utility::parseValue<IntSize>(_value));

		/// @wproperty{Window, MaxSize, int int} Максимальный размер окна.
		else if (_key == "MaxSize")
			setMaxSize(utility::parseValue<IntSize>(_value));

		/// @wproperty{Window, Movable, bool} Режим движения окна мышью за любой участок.
		else if (_key == "Movable")
			setMovable(utility::parseValue<bool>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	const IntCoord& Window::getActionScale() const
	{
		return mCurrentActionScale;
	}

	bool Window::getAutoAlpha() const
	{
		return mIsAutoAlpha;
	}

	TextBox* Window::getCaptionWidget()
	{
		return mWidgetCaption;
	}

	void Window::setMinSize(int _width, int _height)
	{
		setMinSize(IntSize(_width, _height));
	}

	void Window::setMaxSize(int _width, int _height)
	{
		setMaxSize(IntSize(_width, _height));
	}

	void Window::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void Window::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void Window::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	bool Window::getSnap() const
	{
		return mSnap;
	}

	void Window::setSnap(bool _value)
	{
		mSnap = _value;
	}

	void Window::notifyMouseReleased(MyGUI::Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left == _id)
		{
			mCurrentActionScale.clear();
		}
	}

	IntCoord Window::_getActionScale(Widget* _widget)
	{
		if (_widget->isUserString("Scale"))
		{
			IntCoord result = IntCoord::parse(_widget->getUserString("Scale"));

			if (result == IntCoord(1, 1, 0, 0) && !mMovable)
				result.clear();

			return result;
		}
		else if (_widget->isUserString("Action"))
		{
			const std::string& action = _widget->getUserString("Action");
			if (action == "Move")
			{
				if (mMovable)
					return IntCoord(1, 1, 0, 0);
				else
					return IntCoord();
			}

			IntCoord coord;
			Align align = Align::parse(action);

			if (align.isLeft())
			{
				coord.left = 1;
				coord.width = -1;
			}
			else if (align.isRight())
			{
				coord.width = 1;
			}

			if (align.isTop())
			{
				coord.top = 1;
				coord.height = -1;
			}
			else if (align.isBottom())
			{
				coord.height = 1;
			}

			return coord;
		}

		return IntCoord();
	}

	void Window::setMovable(bool _value)
	{
		mMovable = _value;
	}

	bool Window::getMovable() const
	{
		return mMovable;
	}

	void Window::notifyMouseWheel(MyGUI::Widget* _sender, int _rel)
	{
		onMouseWheel(_rel);
		eventMouseWheel(_sender, _rel);
	}

} // namespace MyGUI
