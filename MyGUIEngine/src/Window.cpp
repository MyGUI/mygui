
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
		m_isDestroy(false)
	{

		// запомием размер скина
		mSkinSize = _info->getSize();

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
			if (iter != param.end()) mOffsetCaption = floatRect::parse(iter->second);
			iter = param.find("OffsetX");
			if (iter != param.end()) mOffsetX = floatRect::parse(iter->second);
			iter = param.find("OffsetResize");
			if (iter != param.end()) mOffsetResize = floatRect::parse(iter->second);
			iter = param.find("AlignCaption");
			if (iter != param.end()) mAlignCaption = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignX");
			if (iter != param.end()) mAlignX = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignResize");
			if (iter != param.end()) mAlignResize = SkinManager::getInstance().parseAlign(iter->second);
		}
	}

	void Window::showWindowCaption(bool _show)
	{
		if ( (_show == (mWidgetCaption != null)) || (mSkinCaption.empty()) ) return;
		if (mWidgetCaption != null) WidgetManager::getInstance().destroyWidget(mWidgetCaption);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetCaption, mAlignCaption, mSkinSize, m_cx, m_cy);
			mWidgetCaption = createWidget("Button", mSkinCaption, offset.left, offset.top, offset.right, offset.bottom, mAlignCaption);
			// делегаты для событий
			mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);
		}
	}

	void Window::showWindowX(bool _show)
	{
		if ( (_show == (mWidgetX != null)) || (mSkinX.empty()) ) return;
		if (mWidgetX != null) WidgetManager::getInstance().destroyWidget(mWidgetX);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetX, mAlignX, mSkinSize, m_cx, m_cy);
			mWidgetX = createWidget("Button", mSkinX, offset.left, offset.top, offset.right, offset.bottom, mAlignX);
			// делегаты для событий
			mWidgetX->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressedX);
		}
	}

	void Window::showWindowResize(bool _show)
	{
		if ( (_show == (mWidgetResize != null)) || (mSkinResize.empty()) ) return;
		if (mWidgetResize != null) WidgetManager::getInstance().destroyWidget(mWidgetResize);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetResize, mAlignResize, mSkinSize, m_cx, m_cy);
			mWidgetResize = createWidget("Button", mSkinResize, offset.left, offset.top, offset.right, offset.bottom, mAlignResize);
			// делегаты для событий
			mWidgetResize->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
			mWidgetResize->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedResize);
		}
	}

	void Window::OnMouseSetFocus(WidgetPtr _old)
	{
		Widget::OnMouseSetFocus(_old);
		setDoAlpha(WINDOW_ALPHA_FOCUS);
	}

	void Window::OnMouseLostFocus(WidgetPtr _new)
	{
		Widget::OnMouseLostFocus(_new);
		setDoAlpha(WINDOW_ALPHA_DEACTIVE);
	}

	void Window::OnMouseButtonPressed(bool _left)
	{
		Widget::OnMouseButtonPressed(_left);
		setDoAlpha(WINDOW_ALPHA_ACTIVE);
	}

	void Window::OnMouseButtonReleased(bool _left)
	{
		Widget::OnMouseButtonReleased(_left);
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
		m_enable = false;
		setDoAlpha(0.0);
		m_isDestroy = true;
		InputManager::getInstance().clearFocus(); // пока так
	}

	void Window::notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		const intPoint & point = InputManager::getInstance().getLastLeftPressed();
		move(m_preActionRect.left + (_x - point.left), m_preActionRect.top + (_y - point.top));
	}

	void Window::notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		const intPoint & point = InputManager::getInstance().getLastLeftPressed();
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

		if ((alpha == 0) && (m_isDestroy)) {
			WidgetPtr destroy = this;
			WidgetManager::getInstance().destroyWidget(destroy);
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

	void Window::show(bool _show, bool _smoot)
	{
		if (!_smoot) {Widget::show(_show);return;}
		if (_show) setDoAlpha(1.0f);
		else setDoAlpha(0.0f);
	}

} // namespace MyGUI