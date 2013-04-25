/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_INPUT_MANAGER_H__
#define __MYGUI_INPUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"
#include "MyGUI_Timer.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT InputManager :
		public Singleton<InputManager>,
		public IUnlinkWidget,
		public MemberObsolete<InputManager>
	{
	public:
		InputManager();

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
		bool isFocusMouse() const;
		/** Is any widget have key focus (any widget might have it, not only EditBox or something similar) */
		bool isFocusKey() const;
		/** Is any widget captured mouse */
		bool isCaptureMouse() const;

		/** Set key focus for _widget */
		void setKeyFocusWidget(Widget* _widget);
		/** Drop key focus for _widget */
		void resetKeyFocusWidget(Widget* _widget);
		/** Drop any key focus */
		void resetKeyFocusWidget();

		/** Get mouse focused widget */
		Widget* getMouseFocusWidget() const;
		/** Get key focused widget */
		Widget* getKeyFocusWidget() const;

		/** Get position of last mouse button press.
			Position calculated on specific layer where mouse was pressed.
		*/
		const IntPoint& getLastPressedPosition(MouseButton _id) const;

		/** Get current mouse position on screen */
		const IntPoint& getMousePosition() const;

		/** Get mouse position on current layer.
			This position might different from getMousePosition() if mouse is over non-2d layer.
		*/
		IntPoint getMousePositionByLayer();

		// работа с модальными окнами
		/** Add modal widget - all other widgets inaccessible while modal widget exist */
		void addWidgetModal(Widget* _widget);
		/** Remove modal widget */
		void removeWidgetModal(Widget* _widget);

		/** Return true if any modal widget exist */
		bool isModalAny() const;

		/** Is control button pressed */
		bool isControlPressed() const;
		/** Is shift button pressed */
		bool isShiftPressed() const;

		/** Reset mouse capture.
			For example when we dragging and application
			lost focus you should call this.
		*/
		void resetMouseCaptureWidget();

		/** Unlink widget from input manager. */
		void unlinkWidget(Widget* _widget);

		/** Event : MultiDelegate. Mouse focus was changed.\n
			signature : void method(MyGUI::Widget* _widget)\n
			@param _widget
		*/
		delegates::CMultiDelegate1<Widget*> eventChangeMouseFocus;

		/** Event : MultiDelegate. Key focus was changed.\n
			signature : void method(MyGUI::Widget* _widget)\n
			@param _widget
		*/
		delegates::CMultiDelegate1<Widget*> eventChangeKeyFocus;

		/*internal:*/
		void _resetMouseFocusWidget();

	private:
		// удаляем данный виджет из всех возможных мест
		void _unlinkWidget(Widget* _widget);

		void frameEntered(float _frame);

		void firstEncoding(KeyCode _key, bool bIsKeyPressed);

		// запоминает клавишу для поддержки повторения
		void storeKey(KeyCode _key, Char _text);

		// сбрасывает клавишу повторения
		void resetKey();

	private:
		// виджеты которым принадлежит фокус
		Widget* mWidgetMouseFocus;
		Widget* mWidgetKeyFocus;
		ILayer* mLayerMouseFocus;

		// таймер для двойного клика
		Timer mTimer; //used for double click timing

		// нажат ли шифт
		bool mIsShiftPressed;
		// нажат ли контрол
		bool mIsControlPressed;

		IntPoint mMousePosition;

		// last mouse press position
		IntPoint mLastPressed[MouseButton::MAX];

		// is mouse button captured by active widget
		bool mMouseCapture[MouseButton::MAX];

		// клавиша для повтора
		KeyCode mHoldKey;
		Char mHoldChar;
		bool mFirstPressKey;
		float mTimerKey;
		int mOldAbsZ;

		// список виджетов с модальным режимом
		VectorWidgetPtr mVectorModalRootWidget;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_INPUT_MANAGER_H__
