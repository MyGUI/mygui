/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_INPUT_MANAGER_H__
#define __MYGUI_INPUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Instance.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_InputDefine.h"
#include "MyGUI_Timer.h"

namespace MyGUI
{

	class MYGUI_EXPORT InputManager : public IUnlinkWidget
	{
		MYGUI_INSTANCE_HEADER(InputManager);

	public:
		void initialise();
		void shutdown();

		/** Inject MouseMove event
			@return true if event has been processed by GUI
		*/
		bool injectMouseMove(int _absx, int _absy, int _absz);
		/** Inject MousePress event
			@return true if event has been processed by GUI
		*/
		bool injectMousePress(int _absx, int _absy, MouseButton _id);
		/** Inject MouseRelease event
			@return true if event has been processed by GUI
		*/
		bool injectMouseRelease(int _absx, int _absy, MouseButton _id);

		/** Inject KeyPress event
			@return true if event has been processed by GUI
		*/
		bool injectKeyPress(KeyCode _key, Char _text = 0);
		/** Inject KeyRelease event
			@return true if event has been processed by GUI
		*/
		bool injectKeyRelease(KeyCode _key);

		/** Is any widget have mouse focus */
		bool isFocusMouse() { return mWidgetMouseFocus != nullptr; }
		/** Is any widget have key focus */
		bool isFocusKey() { return mWidgetKeyFocus != nullptr; }
		/** Is any widget captured mouse */
		bool isCaptureMouse() { return mIsWidgetMouseCapture; }

		/** Set key focus for _widget */
		void setKeyFocusWidget(WidgetPtr _widget);
		/** Drop key focus for _widget */
		void resetKeyFocusWidget(WidgetPtr _widget);
		/** Drop any key focus */
		void resetKeyFocusWidget() { setKeyFocusWidget(nullptr); }

		/** Get mouse focused widget */
		WidgetPtr getMouseFocusWidget() { return mWidgetMouseFocus; }
		/** Get key focused widget */
		WidgetPtr getKeyFocusWidget() { return mWidgetKeyFocus; }
		/** Get position of last left mouse button press */
		const IntPoint& getLastLeftPressed() { return mLastLeftPressed; }
		/** Get current mouse position */
		const IntPoint& getMousePosition() { return mMousePosition; }

		// тестовый вариант, очистка фокуса мыши
		/** Drop any mouse focus */
		void resetMouseFocusWidget();

		// работа с модальными окнами
		/** Add modal widget - all other widgets inaccessible while modal widget exist */
		void addWidgetModal(WidgetPtr _widget);
		/** Remove modal widget */
		void removeWidgetModal(WidgetPtr _widget);

		/** Return true if any modal widget exist */
		bool isModalAny() { return !mVectorModalRootWidget.empty(); }

		/** Is control button pressed */
		bool isControlPressed() { return mIsControlPressed; }
		/** Is shift button pressed */
		bool isShiftPressed() { return mIsShiftPressed; }

		/** Reset mouse capture (for example when we dragging and application
			lost focus you should call this)
		*/
		void resetMouseCaptureWidget() { mIsWidgetMouseCapture = false; }

		void unlinkWidget(WidgetPtr _widget) { _unlinkWidget(_widget); }

	/*event:*/
		/** Event : Mouse pointer has been changed.\n
			signature : void method(const std::string& _pointerName)\n
			@param _pointerName Name of current mouse pointer
		*/
		delegates::CDelegate1<const std::string &>
			eventChangeMousePointer;

	private:
		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(WidgetPtr _widget);

		void frameEntered(float _frame);

		void firstEncoding(KeyCode _key, bool bIsKeyPressed);

		// запоминает клавишу для поддержки повторения
		void storeKey(KeyCode _key, Char _text);

		// сбрасывает клавишу повторения
		void resetKey();

	public:
		void setShowFocus(bool _show);
		bool getShowFocus() { return m_showHelpers; }

	private:
		void updateFocusWidgetHelpers();

	private:
		// виджеты которым принадлежит фокус
		WidgetPtr mWidgetMouseFocus;
		WidgetPtr mWidgetKeyFocus;
		// захватил ли мышь активный виджет
		bool mIsWidgetMouseCapture;
		// таймер для двойного клика
	    Timer mTimer; //used for double click timing

		// нажат ли шифт
		bool mIsShiftPressed;
		// нажат ли контрол
		bool mIsControlPressed;
		// там где была последний раз нажата левая кнопка
		IntPoint mLastLeftPressed;
		IntPoint mMousePosition;
		// клавиша для повтора
		KeyCode mHoldKey;
		Char mHoldChar;
		bool mFirstPressKey;
		float mTimerKey;
		int mOldAbsZ;

		// текущий отображаемый указатель
		std::string mPointer;

		// список виджетов с модальным режимом
		VectorWidgetPtr mVectorModalRootWidget;

		bool m_showHelpers;
		WidgetPtr m_mouseHelper;
		WidgetPtr m_keyHelper;

	};

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__
