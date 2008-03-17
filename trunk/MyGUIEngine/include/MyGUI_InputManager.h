/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_INPUT_MANAGER_H__
#define __MYGUI_INPUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_Instance.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_UnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	// ������� ��� ����� ���������� ����� ������
	typedef delegates::CDelegate1<const std::string &> EventInfo_String;

	class _MyGUIExport InputManager : public FrameListener, public UnlinkWidget
	{
		INSTANCE_HEADER(InputManager);

		typedef std::vector<Char> LangInfo;
		typedef std::map<std::string, LangInfo> MapLang;

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		inline bool isFocusMouse() {return mWidgetMouseFocus != null;}
		inline bool isFocusKey() {return mWidgetKeyFocus != null;}
		inline bool isCaptureMouse() {return mIsWidgetMouseCapture;}

		void setKeyFocusWidget(WidgetPtr _widget);
		inline void resetKeyFocusWidget(WidgetPtr _widget) {if (mWidgetKeyFocus == _widget) setKeyFocusWidget(null);}
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		inline WidgetPtr getMouseFocusWidget() {return mWidgetMouseFocus;}
		inline WidgetPtr getKeyFocusWidget() {return mWidgetKeyFocus;}
		inline const std::string & getCurrentLanguage() {return mCurrentLanguage->first;}
		inline const IntPoint & getLastLeftPressed() {return mLastLeftPressed;}
		inline const IntPoint & getMousePosition() {return mMousePosition;}

		inline void setUseOISKeyLayout (bool b) {mUseOISKeyLayout = b;}
		inline bool getUseOISKeyLayout () const {return mUseOISKeyLayout;}

		// �������� �������, ������� ������ ����
		void resetMouseFocusWidget();

		// ������� ������ ������ �� ���� ��������� ����
		void _unlinkWidget(WidgetPtr _widget);

		// ������� ����� ������
		EventInfo_String eventChangeLanguage;

		// ������ � ���������� ������
		void addWidgetModal(WidgetPtr _widget);
		void removeWidgetModal(WidgetPtr _widget);

		bool isModalAny() {return !mVectorModalRootWidget.empty();}

	protected:
		void _frameEntered(float _frame);

		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		Char getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

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
		
		bool mUseOISKeyLayout;

		// ������� ������������ ���������
		std::string mPointer;

		// ������ �������� � ��������� �������
		VectorWidgetPtr mVectorModalRootWidget;


	}; // class InputManager

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__
