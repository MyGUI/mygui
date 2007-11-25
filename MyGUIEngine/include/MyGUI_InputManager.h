/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_INPUT_MANAGER_H__
#define __MYGUI_INPUT_MANAGER_H__

#include "MyGUI_WidgetOIS.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	// ������� ��� ����� ���������� ����� ������
	typedef delegates::CDelegate1<const std::string &> EventChangeLanguage;

	class _MyGUIExport InputManager :  public Ogre::FrameListener
	{
		INSTANCE_HEADER(InputManager);

		typedef std::vector<wchar_t> LangInfo;
		typedef std::map<std::string, LangInfo> MapLang;

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, bool _resource = true);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		inline bool isCaptureMouse() {return mWidgetMouseFocus != null;}
		inline bool isCaptureKey() {return mWidgetKeyFocus != null;}

		void setKeyFocusWidget(WidgetPtr _widget);
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		inline WidgetPtr getMouseFocusWidget() {return mWidgetMouseFocus;}
		inline WidgetPtr getKeyFocusWidget() {return mWidgetKeyFocus;}
		inline const std::string & getCurrentLanguage() {return mCurrentLanguage->first;}
		inline const IntPoint & getLastLeftPressed() {return mLastLeftPressed;}
		inline const IntPoint & getMousePosition() {return mMousePosition;}

		// �������� �������, ������� ������ ����
		void resetMouseFocusWidget();

		// ������� ����� ������
		EventChangeLanguage eventChangeLanguage;

	protected:
		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

		// ������� ��������� ���������
		void createDefaultCharSet();

		// ������� ���� �� ���������
		inline void changeLanguage()
		{
			mCurrentLanguage++;
			if (mCurrentLanguage == mMapLanguages.end())
				mCurrentLanguage = mMapLanguages.begin();
			// ������� �������
			eventChangeLanguage(mCurrentLanguage->first);
		}

		// ���������� ������� ��� ��������� ����������
		void storeKey(int _key);

		// ���������� ������� ����������
		inline void resetKey()
		{
			mHoldKey = OIS::KC_UNASSIGNED;
		}


	protected:

		// ������� ������� ����������� �����
		WidgetPtr mWidgetMouseFocus;
		WidgetPtr mWidgetKeyFocus;
		// �������� �������, ����� ������� ����������� �����
		WidgetPtr mWidgetRootMouseFocus;
		WidgetPtr mWidgetRootKeyFocus;
		// �������� �� ���� �������� ������
		bool mIsWidgetMouseCapture;
		// ������ ��� �������� �����
	    Ogre::Timer mTime; //used for double click timing

		// ����� ������
		MapLang mMapLanguages;
		// ������� ����
		MapLang::iterator mCurrentLanguage;
		// ����� �� ����
		bool mIsCharShift;
		// ������ ��� �������
		LangInfo mNums;
		// ��� ��� ���� ��������� ��� ������ ����� ������
		IntPoint mLastLeftPressed;
		IntPoint mMousePosition;
		// ������� ��� �������
		int mHoldKey;
		bool mFirstPressKey;
		float mTimerKey;
		bool mIsListener;

	}; // class InputManager

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__