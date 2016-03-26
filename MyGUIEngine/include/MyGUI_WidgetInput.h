/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WIDGET_INPUT_H_
#define MYGUI_WIDGET_INPUT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_WidgetToolTip.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"
#include "MyGUI_MaskPickInfo.h"
#include "MyGUI_Delegate.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	/**
	General information about creating delegate for event :
	@example "Delegate usage"
	@code
		void anyFunc(...) { } // global function

		class AnyClass
		{
		public:
			static void anyStaticMethod(...) { } // static class method
			void anyMethod(...) { } // class method
		};

		AnyClass anyObject; // class instance
	@endcode

	delegate creating:
	@code
		eventAny = MyGUI::newDelegate(anyFunc);
		eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
		eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
	@endcode
	*/

	// делегаты для событий виджета
	typedef delegates::CMultiDelegate1<Widget*> EventHandle_WidgetVoid;
	typedef delegates::CMultiDelegate2<Widget*, Widget*> EventHandle_WidgetWidget;
	typedef delegates::CMultiDelegate2<Widget*, bool> EventHandle_WidgetBool;
	typedef delegates::CMultiDelegate2<Widget*, int> EventHandle_WidgetInt;
	typedef delegates::CMultiDelegate2<Widget*, size_t> EventHandle_WidgetSizeT;
	typedef delegates::CMultiDelegate3<Widget*, int, int> EventHandle_WidgetIntInt;
	typedef delegates::CMultiDelegate4<Widget*, int, int, MouseButton> EventHandle_WidgetIntIntButton;
	typedef delegates::CMultiDelegate2<Widget*, KeyCode> EventHandle_WidgetKeyCode;
	typedef delegates::CMultiDelegate3<Widget*, KeyCode, Char> EventHandle_WidgetKeyCodeChar;
	typedef delegates::CMultiDelegate2<Widget*, const ToolTipInfo& > EventHandle_WidgetToolTip;

	class MYGUI_EXPORT WidgetInput
	{
	public:
		WidgetInput();
		virtual ~WidgetInput();

		/** Set need tool tip mode flag. Enable this if you need tool tip events for widget */
		void setNeedToolTip(bool _value);
		/** Get need tool tip mode flag */
		bool getNeedToolTip() const;

		/** Set mouse pointer for this widget */
		void setPointer(const std::string& _value);
		/** Get mouse pointer name for this widget */
		const std::string& getPointer() const;

		/** Set need key focus flag */
		void setNeedKeyFocus(bool _value);
		/** Is need key focus
			If disable this widget won't be reacting on keyboard at all.\n
			Enabled (true) by default.
		*/
		bool getNeedKeyFocus() const;

		/** Set need mouse focus flag */
		void setNeedMouseFocus(bool _value);
		/** Is need mouse focus
			If disable this widget won't be reacting on mouse at all.\n
			Enabled (true) by default.
		*/
		bool getNeedMouseFocus() const;

		/** Set inherits mode flag
			This mode makes all child widgets pickable even if widget don't
			need mouse focus (was set setNeedMouseFocus(false) ).\n
			Disabled (false) by default.
		*/
		void setInheritsPick(bool _value);
		/** Get inherits mode flag */
		bool getInheritsPick() const;

		/** Set picking mask for widget */
		void setMaskPick(const std::string& _filename);
		/** Set picking mask for widget */
		void setMaskPick(const MaskPickInfo& _info);

		bool isMaskPickInside(const IntPoint& _point, const IntCoord& _coord) const;

		bool getRootMouseFocus() const;
		bool getRootKeyFocus() const;

		/** Event : Widget lost mouse focus.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
			@param _sender widget that called this event
			@param _new widget with mouse focus or nullptr
		*/
		EventHandle_WidgetWidget eventMouseLostFocus;

		/** Event : Widget got mouse focus.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
			@param _sender widget that called this event
			@param _old widget with mouse focus or nullptr
		*/
		EventHandle_WidgetWidget eventMouseSetFocus;

		/** Event : Widget mouse move with captured widget.\n
			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
		*/
		EventPairAddParameter<EventHandle_WidgetIntInt, EventHandle_WidgetIntIntButton> eventMouseDrag;

		/** Event : Mouse move over widget.\n
			signature : void method(MyGUI::Widget* _sender, int _left, int _top)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
		*/
		EventHandle_WidgetIntInt eventMouseMove;

		/** Event : Mouse wheel over widget.\n
			signature : void method(MyGUI::Widget* _sender, int _rel)\n
			@param _sender widget that called this event
			@param _rel relative wheel position
		*/
		EventHandle_WidgetInt eventMouseWheel;

		/** Event : Mouse button pressed.\n
			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
			@param _id Mouse button id
		*/
		EventHandle_WidgetIntIntButton eventMouseButtonPressed;

		/** Event : Mouse button released.\n
			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
			@param _sender widget that called this event
			@param _left - pointer position
			@param _top - pointer position
			@param _id Mouse button id
		*/
		EventHandle_WidgetIntIntButton eventMouseButtonReleased;

		/** Event : Mouse button pressed and released.\n
			signature : void method(MyGUI::Widget* _sender)
			@param _sender widget that called this event
		*/
		EventHandle_WidgetVoid eventMouseButtonClick;

		/** Event : Mouse button double click.\n
			signature : void method(MyGUI::Widget* _sender)
			@param _sender widget that called this event
		*/
		EventHandle_WidgetVoid eventMouseButtonDoubleClick;

		/** Event : Widget lost keyboard focus.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
			@param _sender widget that called this event
			@param _new widget with keyboard focus or nullptr
		*/
		EventHandle_WidgetWidget eventKeyLostFocus;

		/** Event : Widget got keyboard focus.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
			@param _sender widget that called this event
			@param _old widget with keyboard focus or nullptr
		*/
		EventHandle_WidgetWidget eventKeySetFocus;

		/** Event : Key pressed.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
			@param _sender widget that called this event
			@param _key code
			@param _char of pressed symbol (for multilanguage applications)
		*/
		EventHandle_WidgetKeyCodeChar eventKeyButtonPressed;

		/** Event : Key released.\n
			signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key)\n
			@param _sender widget that called this event
			@param _key code
		*/
		EventHandle_WidgetKeyCode eventKeyButtonReleased;

		/** Event : Root widget changed mouse focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::Widget* _sender, bool _focus);
			@param _sender widget that called this event
			@param _focus Is widget got mouse focus.
		*/
		EventHandle_WidgetBool eventRootMouseChangeFocus;

		/** Event : Root widget changed keyboard focus.\n
			info : this event sends only to root widget\n
			signature : void method(MyGUI::Widget* _sender, bool _focus);
			@param _sender widget that called this event
			@param _focus Is widget got keyboard focus.
		*/
		EventHandle_WidgetBool eventRootKeyChangeFocus;

		/** Event : Event about changing tooltip state.\n
			signature : void method(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
			@param _sender widget that called this event
			@param _info about tooltip
		*/
		EventHandle_WidgetToolTip eventToolTip;

		/*internal:*/
		void _riseMouseLostFocus(Widget* _new);
		void _riseMouseSetFocus(Widget* _old);
		void _riseMouseDrag(int _left, int _top, MouseButton _id);
		void _riseMouseMove(int _left, int _top);
		void _riseMouseWheel(int _rel);
		void _riseMouseButtonPressed(int _left, int _top, MouseButton _id);
		void _riseMouseButtonReleased(int _left, int _top, MouseButton _id);
		void _riseMouseButtonClick();
		void _riseMouseButtonDoubleClick();
		void _riseKeyLostFocus(Widget* _new);
		void _riseKeySetFocus(Widget* _old);
		void _riseKeyButtonPressed(KeyCode _key, Char _char);
		void _riseKeyButtonReleased(KeyCode _key);
		void _riseMouseChangeRootFocus(bool _focus);
		void _riseKeyChangeRootFocus(bool _focus);

		void _setRootMouseFocus(bool _value);
		void _setRootKeyFocus(bool _value);

	protected:
		virtual void onMouseLostFocus(Widget* _new);
		virtual void onMouseSetFocus(Widget* _old);
		virtual void onMouseDrag(int _left, int _top, MouseButton _id);
		virtual void onMouseMove(int _left, int _top);
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonClick();
		virtual void onMouseButtonDoubleClick();
		virtual void onKeyLostFocus(Widget* _new);
		virtual void onKeySetFocus(Widget* _old);
		virtual void onKeyButtonPressed(KeyCode _key, Char _char);
		virtual void onKeyButtonReleased(KeyCode _key);
		virtual void onMouseChangeRootFocus(bool _focus);
		virtual void onKeyChangeRootFocus(bool _focus);

	private:
		std::string mPointer;
		MaskPickInfo mOwnMaskPickInfo;

		bool mNeedToolTip;
		bool mInheritsPick;
		bool mNeedKeyFocus;
		bool mNeedMouseFocus;

		bool mRootMouseFocus;
		bool mRootKeyFocus;
	};

} // namespace MyGUI

#endif // MYGUI_WIDGET_INPUT_H_
