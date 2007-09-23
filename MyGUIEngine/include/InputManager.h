#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "WidgetOIS.h"
#include "Instance.h"
#include "Common.h"
#include "WidgetDefines.h"
#include "delegate.h"

namespace MyGUI
{

	typedef std::vector<wchar_t> LangInfo;
	typedef std::map<std::string, LangInfo> MapLang;
	// делегат для смены оповещения смены языков
	typedef CDelegate1<const std::string &> EventChangeLanguage;

	class _MyGUIExport InputManager// : public Instance<InputManager>
	{
		INSTANCE_HEADER(InputManager);

	public:
		InputManager();

	public:
		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		inline bool isCaptureMouse() {return m_widgetMouseFocus != null;}
		inline bool isCaptureKey() {return m_widgetKeyFocus != null;}

		void setKeyFocusWidget(WidgetPtr _widget);
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		inline WidgetPtr getMouseFocusWidget() {return m_widgetMouseFocus;}
		inline WidgetPtr getKeyFocusWidget() {return m_widgetKeyFocus;}
		inline const std::string & getCurrentLanguage() {return m_currentLanguage->first;}
		inline const intPoint & getLastLeftPressed() {return m_lastLeftPressed;}

		// тестовый вариант, очистка фокуса мыши
		void resetMouseFocusWidget();

		// событие смены языков
		EventChangeLanguage eventChangeLanguage;

	protected:
		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду

		// создает латинскую раскладку
		void createDefaultCharSet();
		void loadCharSet(const std::string & _file);

		// сменяет язык на следующий
		inline void changeLanguage()
		{
			m_currentLanguage++;
			if (m_currentLanguage == m_mapLanguages.end())
				m_currentLanguage = m_mapLanguages.begin();
			// послать событие
			eventChangeLanguage(m_currentLanguage->first);
		}

	protected:
        static const int GUI_TIME_DOUBLE_CLICK = 250; //measured in milliseconds

		// виджеты которым принадлежит фокус
		WidgetPtr m_widgetMouseFocus;
		WidgetPtr m_widgetKeyFocus;
		// корневые виджеты, детям которых принадлежит фокус
		WidgetPtr m_widgetRootMouseFocus;
		WidgetPtr m_widgetRootKeyFocus;
		// захватил ли мышь активный виджет
		bool m_isWidgetMouseCapture;
		// таймер для двойного клика
	    Ogre::Timer m_time; //used for double click timing

		// карта языков
		MapLang m_mapLanguages;
		// текущий язык
		MapLang::iterator m_currentLanguage;
		// нажат ли шифт
		bool m_isCharShift;
		// массив для нумлока
		LangInfo m_nums;
		// там где была последний раз нажата левая кнопка
		intPoint m_lastLeftPressed;

	}; // class InputManager

} // namespace MyGUI

#endif
