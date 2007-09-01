#pragma once

#include "WidgetOIS.h"
#include "PointerManager.h"
#include "LayerManager.h"
#include "Widget.h"
#include "Types.h"

namespace widget
{

	class InputManager
	{
	public:
		InputManager();
	public:
		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		inline bool isCaptureMouse() {return m_activeMouse;}
		inline bool isCaptureKey() {return m_activeKey;}

	protected:
        static const int GUI_TIME_DOUBLE_CLICK = 250; //measured in milliseconds

		PointerManager & m_pointerManagerInstance;
		LayerManager & m_layerManagerInstance;

		// виджеты которым принадлежит фокус
		WidgetPtr m_widgetMouseFocus;
		WidgetPtr m_widgetKeyFocus;
		// принадлежат ли нам устройства
		bool m_activeMouse;
		bool m_activeKey;
		// захватил ли мышь активный виджет
		bool m_isWidgetMouseCapture;
		// таймер для двойного клика
	    Ogre::Timer m_time; //used for double click timing

	}; // class InputManager

} // namespace widget