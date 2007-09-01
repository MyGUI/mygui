
#include "InputManager.h"
#include "debugOut.h"

namespace widget
{

	InputManager::InputManager() :
		m_pointerManagerInstance(PointerManager::getInstance()),
		m_layerManagerInstance(LayerManager::getInstance()),
		m_widgetMouseFocus(0), m_widgetKeyFocus(0),
		m_activeMouse(false), m_activeKey(false),
		m_isWidgetMouseCapture(false)
	{
	}

	bool InputManager::injectMouseMove( const OIS::MouseEvent & _arg)
	{
		// двигаем курсор
		m_pointerManagerInstance.move(_arg.state.X.abs, _arg.state.Y.abs);

		// проверка на скролл
		if (_arg.state.Z.rel != 0) {
			if (m_widgetMouseFocus) m_widgetMouseFocus->OnMouseSheel(_arg.state.Z.rel);
			return m_activeMouse;
		}

		if (m_isWidgetMouseCapture) {
			m_widgetMouseFocus->OnMouseMove(_arg.state.X.abs, _arg.state.Y.abs);
			return m_activeMouse;
		}

		// ищем активное окно
		WidgetPtr item = dynamic_cast<WidgetPtr>(m_layerManagerInstance.findItem(_arg.state.X.abs, _arg.state.Y.abs));
		// ничего не изменилось
		if (m_widgetMouseFocus == item) return m_activeMouse;

		// смена фокуса
		if (m_widgetMouseFocus != null) m_widgetMouseFocus->OnMouseLostFocus(item);
		if (item != null) item->OnMouseSetFocus(m_widgetMouseFocus);

		// запоминаем текущее окно
		m_widgetMouseFocus = item;
		m_activeMouse = (m_widgetMouseFocus != null);

		return m_activeMouse;
	}

	bool InputManager::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (!m_activeMouse)
		{
			if (m_widgetKeyFocus) {
				// сброс окна ввода
				m_widgetKeyFocus->OnKeyLostFocus( null );
				m_widgetKeyFocus = null;
			}
			return m_activeMouse;
		}

//		if (_id != OIS::MB_Left) //Only responds to left clicks
//		    return m_activeMouse;

//		if (!m_currentWindow)
//		    return false;
		
		//Move windows to the top of the stack that have been clicked on
//		GUI::getSingleton()->upZOrder(m_currentWindow); // поднятие окна над другими
		
//		if (m_currentFocusWindow) {

		// захватываем только по левой клавише
		if (_id == OIS::MB_Left) m_isWidgetMouseCapture = true; // захват окна
			
//			const int Left = m_currentFocusWindow->m_overlayContainer->_getDerivedLeft() * GUI::getSingleton()->getWidth();
//			const int Top  = m_currentFocusWindow->m_overlayContainer->_getDerivedTop() *  GUI::getSingleton()->getHeight();

			// смещение клика внутри окна
//			m_iOffsetPressedX = arg.state.X.abs - Left - 1;
//			m_iOffsetPressedY = arg.state.Y.abs - Top  - 1;

		m_widgetMouseFocus->OnMouseButtonPressed(_id == OIS::MB_Left);
//		}

		if (m_widgetMouseFocus != m_widgetKeyFocus) { //changing input window

			if (m_widgetKeyFocus != null) m_widgetKeyFocus->OnKeyLostFocus(m_widgetMouseFocus);
			if (m_widgetMouseFocus != null) m_widgetMouseFocus->OnKeySetFocus(m_widgetKeyFocus);
			m_widgetKeyFocus = m_widgetMouseFocus;

		}

//		m_currentWindow->_OnUpZOrder(); // уведомление об поднятии

		return m_activeMouse;
	}

	bool InputManager::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{
		if (m_activeMouse)
		{
			// сбрасываем захват
			m_isWidgetMouseCapture = false;

			m_widgetMouseFocus->OnMouseButtonReleased(_id == OIS::MB_Left);

			if ((_id == OIS::MB_Left) && m_time.getMilliseconds() < GUI_TIME_DOUBLE_CLICK) {
				m_widgetMouseFocus->OnMouseButtonClick(true);
			} else {
			    m_time.reset();
	            m_widgetMouseFocus->OnMouseButtonClick(false);
			}
		}
		return m_activeMouse;
	}

	bool InputManager::injectKeyPress(const OIS::KeyEvent & _arg)
	{
		return false;
	}

	bool InputManager::injectKeyRelease(const OIS::KeyEvent & _arg)
	{
		return false;
	}

} // namespace widget
