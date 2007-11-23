#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "WidgetOIS.h"
#include "Instance.h"
#include "Common.h"
#include "WidgetDefines.h"
#include "delegate.h"

namespace MyGUI
{

	// делегат для смены оповещения смены языков
	typedef CDelegate1<const std::string &> EventChangeLanguage;

	class _MyGUIExport InputManager :  public Ogre::FrameListener
	{
		INSTANCE_HEADER(InputManager);

		typedef std::vector<wchar_t> LangInfo;
		typedef std::map<std::string, LangInfo> MapLang;

	public:
		void initialise();
		void shutdown();

	public:

		void load(const std::string & _file);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

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
		inline const IntPoint & getLastLeftPressed() {return m_lastLeftPressed;}
		inline const IntPoint & getMousePosition() {return mMousePosition;}

		// тестовый вариант, очистка фокуса мыши
		void resetMouseFocusWidget();

		// событие смены языков
		EventChangeLanguage eventChangeLanguage;

	protected:
		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду

		// создает латинскую раскладку
		void createDefaultCharSet();

		// сменяет язык на следующий
		inline void changeLanguage()
		{
			m_currentLanguage++;
			if (m_currentLanguage == m_mapLanguages.end())
				m_currentLanguage = m_mapLanguages.begin();
			// послать событие
			eventChangeLanguage(m_currentLanguage->first);
		}

		// запоминает клавишу для поддержки повторения
		inline void storeKey(int _key)
		{
			mHoldKey = OIS::KC_UNASSIGNED;

			if ( ! isCaptureKey() ) return;
			if ( ! mIsListener) {
				Ogre::Root::getSingleton().addFrameListener(this);
				mIsListener = true;
			}
			if ( (_key == OIS::KC_LSHIFT)
				|| (_key == OIS::KC_RSHIFT)
				|| (_key == OIS::KC_LCONTROL)
				|| (_key == OIS::KC_RCONTROL)
				|| (_key == OIS::KC_LMENU)
				|| (_key == OIS::KC_RMENU)
				) return;

			mHoldKey = _key;
			mFirstPressKey = true;
			mTimerKey = 0.0f;
		}

		// сбрасывает клавишу повторения
		inline void resetKey()
		{
			mHoldKey = OIS::KC_UNASSIGNED;
		}


	protected:

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
		IntPoint m_lastLeftPressed;
		IntPoint mMousePosition;
		// клавиша для повтора
		int mHoldKey;
		bool mFirstPressKey;
		float mTimerKey;
		bool mIsListener;

	}; // class InputManager

} // namespace MyGUI

#endif // __INPUT_MANAGER_H__
