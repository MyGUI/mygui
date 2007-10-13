
#include "Window.h"
#include "WindowParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Window> WindowFactoryInstance("Window"); }
	// парсер команд
	namespace parser { WindowParser WindowParserInstance; }

	const float WINDOW_ALPHA_ACTIVE = 1.0;
	const float WINDOW_ALPHA_FOCUS = 0.7;
	const float WINDOW_ALPHA_DEACTIVE = 0.3;

	const int WINDOW_TO_STICK = 10;

	Window::Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetX(null), mWidgetResize(null), mWidgetClient(null),
		m_bIsListenerAlpha(false),
		m_isDestroy(false),
		m_mouseRootFocus(false), m_keyRootFocus(false),
		m_bIsAutoAlpha(true),
		mIsToStick(false)
	{
		// дефолтные размеры
		m_minmax.set(50, 50, 2050, 2050);

		// запоминаем размер скина
		IntSize size = _info->getSize();

		// альфа в первоначальное положение
		setAlpha(0.0f);
		setDoAlpha(WINDOW_ALPHA_DEACTIVE);

		// парсим свойства
		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("WindowToStick");
		if (iter != param.end()) mIsToStick = util::parseBool(iter->second);

		// парсим заголовок
		mWidgetCaption = parseSubWidget(param, "Button", "SkinCaption", "OffsetCaption", "AlignCaption", size);
		if (mWidgetCaption != null) {
			// делегаты для событий
			mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);
		}

		// парсим крестик
		mWidgetX = parseSubWidget(param, "Button", "SkinX", "OffsetX", "AlignX", size);
		if (mWidgetX != null) {
			// делегаты для событий
			mWidgetX->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressedX);
		}

		// парсим ресайзер
		mWidgetResize = parseSubWidget(param, "Button", "SkinResize", "OffsetResize", "AlignResize", size);
		if (mWidgetResize != null) {
			// делегаты для событий
			mWidgetResize->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetResize->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedResize);
		}

		// парсим клиент в последнюю очередь
		mWidgetClient = parseSubWidget(param, "Widget", "SkinClient", "OffsetClient", "AlignClient", size);

	}

	// переопределяем для присвоению клиенту
	WidgetPtr Window::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _name)
	{
		if (mWidgetClient != null) return mWidgetClient->createWidget(_type, _skin, _x, _y, _cx, _cy, _align, _name);
		return Widget::createWidget(_type, _skin, _x, _y, _cx, _cy, _align, _name);
	}

	void Window::_onMouseChangeRootFocus(bool _focus)
	{
		Widget::_onMouseChangeRootFocus(_focus);
		m_mouseRootFocus = _focus;
		updateAlpha();
	}

	void Window::_onKeyChangeRootFocus(bool _focus)
	{
		Widget::_onKeyChangeRootFocus(_focus);
		m_keyRootFocus = _focus;
		updateAlpha();
	}

	void Window::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (!_left) return;
		m_preActionRect.set(m_x, m_y, m_cx, m_cy);
	}

	void Window::notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left)
	{
		eventWindowXPressed(this);
	}

	void Window::notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		move(m_preActionRect.left + (_x - point.left), m_preActionRect.top + (_y - point.top));
	}

	void Window::notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
		size(m_preActionRect.right + (_x - point.left), m_preActionRect.bottom + (_y - point.top));
	}

	void Window::setDoAlpha(float _alpha)
	{
		if (m_isDestroy) return;

		m_doAlpha = _alpha;
		if (m_doAlpha == getAlpha()) {
			// если мы были подписанны, то отписываемся
			if (m_bIsListenerAlpha) {
				Ogre::Root::getSingleton().removeFrameListener(this);
				m_bIsListenerAlpha = false;
			}
			// выходим
			return;
		}

		// если надо, то подписываемся
		if (!m_bIsListenerAlpha) {
			Ogre::Root::getSingleton().addFrameListener(this);
			m_bIsListenerAlpha = true;
		}
	}

	bool Window::frameStarted(const Ogre::FrameEvent& evt)
	{
		const float COEF = 3.0f;
		// огр отписывает после прохода
		if (!m_bIsListenerAlpha) return true;

		float alpha = getAlpha();
		if (alpha == m_doAlpha) {
			Ogre::Root::getSingleton().removeFrameListener(this);
			m_bIsListenerAlpha = false;
			return true;

		} else if (alpha > m_doAlpha) {
			alpha -= evt.timeSinceLastFrame * COEF;
			if (alpha <= m_doAlpha) {
				alpha = m_doAlpha;
				Ogre::Root::getSingleton().removeFrameListener(this);
				m_bIsListenerAlpha = false;
			}
		} else {
			alpha += evt.timeSinceLastFrame * COEF;
			if (alpha >= m_doAlpha) {
				alpha = m_doAlpha;
				Ogre::Root::getSingleton().removeFrameListener(this);
				m_bIsListenerAlpha = false;
			}
		}

		if (alpha == 0.0) {
			if (m_isDestroy) {
				WidgetPtr destroy = this;
				WidgetManager::getInstance().destroyWidget(destroy);
			} else {
				Widget::show(false);
				m_enable = true;
			}
			return true;
		}

		// устанавливаем текущую альфу
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
		m_enable = true; // это если мы будем показывать, а оно еще и не скрылось
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
			m_enable = false;
			setDoAlpha(0.0);
			m_isDestroy = _destroy;
			InputManager::getInstance().resetKeyFocusWidget();
			InputManager::getInstance().resetMouseFocusWidget();
		}
	}

	void Window::updateAlpha()
	{
		if (!m_bIsAutoAlpha) return;

		if (m_keyRootFocus) setDoAlpha(WINDOW_ALPHA_ACTIVE);
		else if (m_mouseRootFocus) setDoAlpha(WINDOW_ALPHA_FOCUS);
		else setDoAlpha(WINDOW_ALPHA_DEACTIVE);
	}

	void Window::move(int _x, int _y)
	{
		// прилепляем к краям
		if (mIsToStick) {
			if (_x > 0) {if ( (_x - WINDOW_TO_STICK) <= 0) _x = 0;}
			else {if ( (_x + WINDOW_TO_STICK) >= 0) _x = 0;}
			if (_y > 0) {if ( (_y - WINDOW_TO_STICK) <= 0) _y = 0;}
			else {	if ( (_y + WINDOW_TO_STICK) >= 0) _y = 0;}

			int width = Gui::getInstance().getWidth();
			int height = Gui::getInstance().getHeight();

			if ( (_x + m_cx) < width) {if ( (_x + m_cx + WINDOW_TO_STICK) > width ) _x = width - m_cx;	}
			else {	if ( (_x + m_cx - WINDOW_TO_STICK) < width ) _x = width - m_cx;}
			if ( (_y + m_cy) < height) {if ( (_y + m_cy + WINDOW_TO_STICK) > height ) _y = height - m_cy;}
			else {	if ( (_y + m_cy - WINDOW_TO_STICK) < height ) _y = height - m_cy;}
		}
		Widget::move(_x, _y);
	}

	void Window::move(int _x, int _y, int _cx, int _cy)
	{
		Widget::move(_x, _y, _cx, _cy);
	}

	void Window::size(int _cx, int _cy)
	{
		// прилепляем к краям
		if (mIsToStick) {
			int width = Gui::getInstance().getWidth();
			int height = Gui::getInstance().getHeight();

			if ( (m_x + _cx) < width) {if ( (m_x + _cx + WINDOW_TO_STICK) > width ) _cx = width - m_x;	}
			else {	if ( (m_x + _cx - WINDOW_TO_STICK) < width ) _cx = width - m_x;}
			if ( (m_y + _cy) < height) {if ( (m_y + _cy + WINDOW_TO_STICK) > height ) _cy = height - m_y;}
			else {	if ( (m_y + _cy - WINDOW_TO_STICK) < height ) _cy = height - m_y;}
		}

		if (_cx < m_minmax.left) _cx = m_minmax.left;
		else if (_cx > m_minmax.right) _cx = m_minmax.right;
		if (_cy < m_minmax.top) _cy = m_minmax.top;
		else if (_cy > m_minmax.bottom) _cy = m_minmax.bottom;
		if ((_cx == m_cx) && (_cy == m_cy) ) return;

		Widget::size(_cx, _cy);
	}

} // namespace MyGUI