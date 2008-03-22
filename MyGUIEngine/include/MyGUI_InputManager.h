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
//#include "MyGUI_WidgetOIS.h"
#include "MyGUI_Instance.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_UnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_InputDefine.h"

namespace MyGUI
{

	// делегат для смены оповещения смены языков
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

		bool injectMouseMove(int _absx, int _absy, int _absz);
		bool injectMousePress(int _absx, int _absy, MouseButton _id);
		bool injectMouseRelease(int _absx, int _absy, MouseButton _id);

		bool injectKeyPress(KeyCode _key);
		bool injectKeyRelease(KeyCode _key);

		// OIS
		/*inline bool injectMouseMove( const OIS::MouseEvent & _arg) {return injectMouseMove(_arg.state.X.abs, _arg.state.Y.abs, _arg.state.Z.abs);}
		inline bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return injectMousePress(_arg.state.X.abs, _arg.state.Y.abs, (MouseButton)_id);}
		inline bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return injectMouseRelease(_arg.state.X.abs, _arg.state.Y.abs, (MouseButton)_id);}
		inline bool injectKeyPress(const OIS::KeyEvent & _arg) {return injectKeyPress((KeyCode)_arg.key);}
		inline bool injectKeyRelease(const OIS::KeyEvent & _arg) {return injectKeyRelease((KeyCode)_arg.key);}*/

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

		//inline void setUseOISKeyLayout (bool b) {mUseOISKeyLayout = b;}
		//inline bool getUseOISKeyLayout () const {return mUseOISKeyLayout;}

		// тестовый вариант, очистка фокуса мыши
		void resetMouseFocusWidget();

		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(WidgetPtr _widget);

		// событие смены языков
		EventInfo_String eventChangeLanguage;

		// работа с модальными окнами
		void addWidgetModal(WidgetPtr _widget);
		void removeWidgetModal(WidgetPtr _widget);

		bool isModalAny() {return !mVectorModalRootWidget.empty();}

		inline bool isControlPressed() {return mIsControlPressed;}
		inline bool isShiftPressed() {return mIsShiftPressed;}

	protected:
		void _frameEntered(float _frame);

		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		Char getKeyChar(int keyEvent); // возвращает символ по его скан коду

		// создает латинскую раскладку
		void createDefaultCharSet();

		// сменяет язык на следующий
		inline void changeLanguage()
		{
			mCurrentLanguage++;
			if (mCurrentLanguage == mMapLanguages.end())
				mCurrentLanguage = mMapLanguages.begin();
			// послать событие
			eventChangeLanguage(mCurrentLanguage->first);
		}

		// запоминает клавишу для поддержки повторения
		void storeKey(int _key);

		// сбрасывает клавишу повторения
		void resetKey(int _key);


	protected:

		// виджеты которым принадлежит фокус
		WidgetPtr mWidgetMouseFocus;
		WidgetPtr mWidgetKeyFocus;
		// корневые виджеты, детям которых принадлежит фокус
		WidgetPtr mWidgetRootMouseFocus;
		WidgetPtr mWidgetRootKeyFocus;
		// захватил ли мышь активный виджет
		bool mIsWidgetMouseCapture;
		// таймер для двойного клика
	    Ogre::Timer mTime; //used for double click timing

		// карта языков
		MapLang mMapLanguages;
		// текущий язык
		MapLang::iterator mCurrentLanguage;
		// нажат ли шифт
		bool mIsShiftPressed;
		// нажат ли контрол
		bool mIsControlPressed;
		// массив для нумлока
		LangInfo mNums;
		// там где была последний раз нажата левая кнопка
		IntPoint mLastLeftPressed;
		IntPoint mMousePosition;
		// клавиша для повтора
		int mHoldKey;
		bool mFirstPressKey;
		float mTimerKey;
		
		//bool mUseOISKeyLayout;

		// текущий отображаемый указатель
		std::string mPointer;

		// список виджетов с модальным режимом
		VectorWidgetPtr mVectorModalRootWidget;


	}; // class InputManager

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__
