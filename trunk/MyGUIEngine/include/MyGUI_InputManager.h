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
#include "MyGUI_Instance.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_UnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_InputDefine.h"

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

		/** Load additional MyGUI *.lang file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		/** Inject MouseMove event*/
		bool injectMouseMove(int _absx, int _absy, int _absz);
		/** Inject MousePress event*/
		bool injectMousePress(int _absx, int _absy, MouseButton _id);
		/** Inject MouseRelease event*/
		bool injectMouseRelease(int _absx, int _absy, MouseButton _id);

		/** Inject KeyPress event*/
		bool injectKeyPress(KeyCode _key);
		/** Inject KeyReleas event*/
		bool injectKeyRelease(KeyCode _key);

		/** Is any widget have mouse focus */
		inline bool isFocusMouse() {return mWidgetMouseFocus != null;}
		/** Is any widget have key focus */
		inline bool isFocusKey() {return mWidgetKeyFocus != null;}
		/** Is any widget captured mouse */
		inline bool isCaptureMouse() {return mIsWidgetMouseCapture;}

		/** Set key focus for _widget */
		void setKeyFocusWidget(WidgetPtr _widget);
		/** Drop key focus for _widget */
		inline void resetKeyFocusWidget(WidgetPtr _widget) {if (mWidgetKeyFocus == _widget) setKeyFocusWidget(null);}
		/** Drop any key focus */
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		/** Get mouse focused widget */
		inline WidgetPtr getMouseFocusWidget() {return mWidgetMouseFocus;}
		/** Get key focused widget */
		inline WidgetPtr getKeyFocusWidget() {return mWidgetKeyFocus;}
		/** Get current language */
		inline const std::string & getCurrentLanguage() {return mCurrentLanguage->first;}
		/** Get position of last left mouse button press */
		inline const IntPoint & getLastLeftPressed() {return mLastLeftPressed;}
		/** Get current mouse position */
		inline const IntPoint & getMousePosition() {return mMousePosition;}

		// �������� �������, ������� ������ ����
		/** Drop any mouse focus */
		void resetMouseFocusWidget();

		// ������� ������ ������ �� ���� ��������� ����
		void _unlinkWidget(WidgetPtr _widget);

		// ������� ����� ������
		/** Event : Language has been changed.\n
			signature : void method(std::string _languageName)\n
			_languageName nema of current language
		*/
		EventInfo_String eventChangeLanguage;

		// ������ � ���������� ������
		/** Add modal widget - all other widgets inaccessible while modal widget exist */
		void addWidgetModal(WidgetPtr _widget);
		/** Remove modal widget */
		void removeWidgetModal(WidgetPtr _widget);

		/** Return true if any modal widget exist */
		bool isModalAny() {return !mVectorModalRootWidget.empty();}

		/** Is control button pressed */
		inline bool isControlPressed() {return mIsControlPressed;}
		/** Is shift button pressed */
		inline bool isShiftPressed() {return mIsShiftPressed;}

		// ���������� ������, ����������� � ��������� �����
		WidgetPtr getWidgetFromPoint(int _left, int _top);

		// ���������� ������ ����, ���� �� ���
		inline void resetMouseCaptureWidget() {mIsWidgetMouseCapture = false;}

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
		void resetKey(int _key);


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
		bool mIsShiftPressed;
		// ����� �� �������
		bool mIsControlPressed;
		// ������ ��� �������
		LangInfo mNums;
		// ��� ��� ���� ��������� ��� ������ ����� ������
		IntPoint mLastLeftPressed;
		IntPoint mMousePosition;
		// ������� ��� �������
		int mHoldKey;
		bool mFirstPressKey;
		float mTimerKey;
		int mOldAbsZ;
		
		// ������� ������������ ���������
		std::string mPointer;

		// ������ �������� � ��������� �������
		VectorWidgetPtr mVectorModalRootWidget;


	}; // class InputManager

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__
