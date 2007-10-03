
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

	Window::Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetX(null), mWidgetResize(null),
		mAlignCaption(ALIGN_NONE), mAlignX(ALIGN_NONE), mAlignResize(ALIGN_NONE),
		m_bIsListenerAlpha(false),
		m_isDestroy(false),
		m_mouseRootFocus(false), m_keyRootFocus(false),
		m_bIsAutoAlpha(true)
	{
		m_minmax.left = 50;
		m_minmax.top = 50;
		m_minmax.right = 2050;
		m_minmax.bottom = 2050;

		// запомием размер скина
		mSkinSize = _info->getSize();

		// альфа в первоначальное положение
		setAlpha(0.0f);
		setDoAlpha(WINDOW_ALPHA_DEACTIVE);

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinCaption");
			if (iter != param.end()) mSkinCaption = iter->second;
			iter = param.find("SkinX");
			if (iter != param.end()) mSkinX = iter->second;
			iter = param.find("SkinResize");
			if (iter != param.end()) mSkinResize = iter->second;
			iter = param.find("OffsetCaption");
			if (iter != param.end()) mOffsetCaption = util::parseFloatRect(iter->second);
			iter = param.find("OffsetX");
			if (iter != param.end()) mOffsetX = util::parseFloatRect(iter->second);
			iter = param.find("OffsetResize");
			if (iter != param.end()) mOffsetResize = util::parseFloatRect(iter->second);
			iter = param.find("AlignCaption");
			if (iter != param.end()) mAlignCaption = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignX");
			if (iter != param.end()) mAlignX = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignResize");
			if (iter != param.end()) mAlignResize = SkinManager::getInstance().parseAlign(iter->second);
		}

		_showWindowCaption(true);
		_showWindowX(true);
		_showWindowResize(true);

	}

	void Window::_showWindowCaption(bool _show)
	{
		if ( (_show == (mWidgetCaption != null)) || (mSkinCaption.empty()) ) return;
		if (mWidgetCaption != null) WidgetManager::getInstance().destroyWidget(mWidgetCaption);
		else {
			FloatRect offset = WidgetManager::convertOffset(mOffsetCaption, mAlignCaption, mSkinSize, m_cx, m_cy);
			mWidgetCaption = createWidget("Button", mSkinCaption, offset.left, offset.top, offset.right, offset.bottom, mAlignCaption);
			// делегаты для событий
			mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);
		}
	}

	void Window::_showWindowX(bool _show)
	{
		if ( (_show == (mWidgetX != null)) || (mSkinX.empty()) ) return;
		if (mWidgetX != null) WidgetManager::getInstance().destroyWidget(mWidgetX);
		else {
			FloatRect offset = WidgetManager::convertOffset(mOffsetX, mAlignX, mSkinSize, m_cx, m_cy);
			mWidgetX = createWidget("Button", mSkinX, offset.left, offset.top, offset.right, offset.bottom, mAlignX);
			// делегаты для событий
			mWidgetX->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressedX);
		}
	}

	void Window::_showWindowResize(bool _show)
	{
		if ( (_show == (mWidgetResize != null)) || (mSkinResize.empty()) ) return;
		if (mWidgetResize != null) WidgetManager::getInstance().destroyWidget(mWidgetResize);
		else {
			FloatRect offset = WidgetManager::convertOffset(mOffsetResize, mAlignResize, mSkinSize, m_cx, m_cy);
			mWidgetResize = createWidget("Button", mSkinResize, offset.left, offset.top, offset.right, offset.bottom, mAlignResize);
			// делегаты для событий
			mWidgetResize->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetResize->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedResize);
		}
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
		m_preActionRect.left = m_x;
		m_preActionRect.top = m_y;
		m_preActionRect.right = m_cx;
		m_preActionRect.bottom = m_cy;
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

	void Window::size(int _cx, int _cy)
	{
		if (_cx < m_minmax.left) _cx = m_minmax.left;
		else if (_cx > m_minmax.right) _cx = m_minmax.right;
		if (_cy < m_minmax.top) _cy = m_minmax.top;
		else if (_cy > m_minmax.bottom) _cy = m_minmax.bottom;
		if ((_cx == m_cx) && (_cy == m_cy) ) return;
		Widget::size(_cx, _cy);
	}

} // namespace MyGUI