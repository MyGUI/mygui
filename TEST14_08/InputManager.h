#pragma once

#include "WidgetOIS.h"
#include "PointerManager.h"
#include "LayerManager.h"
#include "Widget.h"
#include "Types.h"
#include "delegate.h"

namespace widget
{

	typedef std::vector<wchar_t> LangInfo;
	typedef std::map<std::string, LangInfo> MapLang;
	// ������� ��� ����� ���������� ����� ������
	typedef CDelegate1<const std::string &> EventChangeLanguage;

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

		inline bool isCaptureMouse() {return m_widgetMouseFocus != null;}
		inline bool isCaptureKey() {return m_widgetKeyFocus != null;}

		void setKeyFocusWidget(WidgetPtr _widget);
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		inline WidgetPtr getMouseFocusWidget() {return m_widgetMouseFocus;}
		inline WidgetPtr getKeyFocusWidget() {return m_widgetKeyFocus;}
		inline const std::string & getCurrentLanguage() {return m_currentLanguage->first;}

		// ������� ����� ������
		EventChangeLanguage eventChangeLanguage;

	protected:
		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

		// ������� ��������� ���������
		void createDefaultCharSet();
		void loadCharSet(const std::string & _file);

		// ������� ���� �� ���������
		inline void changeLanguage()
		{
			m_currentLanguage++;
			if (m_currentLanguage == m_mapLanguages.end())
				m_currentLanguage = m_mapLanguages.begin();
			// ������� �������
			eventChangeLanguage(m_currentLanguage->first);
		}

	protected:
        static const int GUI_TIME_DOUBLE_CLICK = 250; //measured in milliseconds

//		PointerManager & m_pointerManagerInstance;
		LayerManager & m_layerManagerInstance;

		// ������� ������� ����������� �����
		WidgetPtr m_widgetMouseFocus;
		WidgetPtr m_widgetKeyFocus;
		// ����������� �� ��� ����������
//		bool m_activeMouse;
//		bool m_activeKey;
		// �������� �� ���� �������� ������
		bool m_isWidgetMouseCapture;
		// ������ ��� �������� �����
	    Ogre::Timer m_time; //used for double click timing

		// ����� ������
		MapLang m_mapLanguages;
		// ������� ����
		MapLang::iterator m_currentLanguage;
		// ����� �� ����
		bool m_isCharShift;
		// ������ ��� �������
		LangInfo m_nums;

	}; // class InputManager

} // namespace widget