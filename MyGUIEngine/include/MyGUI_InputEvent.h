/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_INPUT_EVENT_H_
#define MYGUI_INPUT_EVENT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_Types.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"

namespace MyGUI
{

	/** State shared by all handlers during synchronous delivery of one input event.
		Do not retain the event or references to it after the callback.
	*/
	class InputEventContext
	{
	public:
		InputEventContext(const InputEventContext&) = delete;
		InputEventContext& operator=(const InputEventContext&) = delete;

		/** Original target; becomes nullptr if it is unlinked during a callback. */
		Widget* getTarget() const
		{
			return mTarget;
		}
		/** Widget currently receiving the event; becomes nullptr if it is unlinked. */
		Widget* getCurrentTarget() const
		{
			return mCurrentTarget;
		}
		/** Whether this input is consumed and further routed delivery will stop. */
		bool isHandled() const
		{
			return mHandled;
		}

		/** Mark input consumed. After this handler returns, later routed handlers on
			this widget and its ancestors are skipped. A handler may revise its decision
			with false before returning; a later handler cannot release consumed input.
			Does not cancel legacy behavior, capture, focus, or click generation.
		*/
		void setHandled(bool _value = true)
		{
			mHandled = _value;
		}

	protected:
		explicit InputEventContext(Widget* _target) :
			mTarget(_target),
			mCurrentTarget(_target)
		{
		}
		~InputEventContext() = default;

	private:
		friend class WidgetInput;
		Widget* mTarget;
		Widget* mCurrentTarget;
		bool mHandled{false};
	};

	/** Position for routed mouse movement. */
	class MouseMoveEvent final : public InputEventContext
	{
	public:
		const IntPoint& getPosition() const
		{
			return mPosition;
		}

	private:
		friend class WidgetInput;
		MouseMoveEvent(Widget* _target, int _left, int _top) :
			InputEventContext(_target),
			mPosition(_left, _top)
		{
		}

		IntPoint mPosition;
	};

	/** Position and button for routed press, release and captured drag. */
	class MouseButtonEvent final : public InputEventContext
	{
	public:
		const IntPoint& getPosition() const
		{
			return mPosition;
		}
		MouseButton getButton() const
		{
			return mButton;
		}

	private:
		friend class WidgetInput;
		MouseButtonEvent(Widget* _target, int _left, int _top, MouseButton _button) :
			InputEventContext(_target),
			mPosition(_left, _top),
			mButton(_button)
		{
		}

		IntPoint mPosition;
		MouseButton mButton;
	};

	/** Delta for routed mouse wheel input. */
	class MouseWheelEvent final : public InputEventContext
	{
	public:
		int getWheelDelta() const
		{
			return mWheelDelta;
		}

	private:
		friend class WidgetInput;
		MouseWheelEvent(Widget* _target, int _wheelDelta) :
			InputEventContext(_target),
			mWheelDelta(_wheelDelta)
		{
		}

		int mWheelDelta;
	};

	/** Context for routed click and double-click; these have no position payload. */
	class MouseClickEvent final : public InputEventContext
	{
	private:
		friend class WidgetInput;
		explicit MouseClickEvent(Widget* _target) :
			InputEventContext(_target)
		{
		}
	};

	/** Key code and Unicode character for routed key press. */
	class KeyPressEvent final : public InputEventContext
	{
	public:
		KeyCode getKey() const
		{
			return mKey;
		}
		Char getCharacter() const
		{
			return mCharacter;
		}

	private:
		friend class WidgetInput;
		KeyPressEvent(Widget* _target, KeyCode _key, Char _character) :
			InputEventContext(_target),
			mKey(_key),
			mCharacter(_character)
		{
		}

		KeyCode mKey;
		Char mCharacter;
	};

	/** Key code for routed key release. */
	class KeyReleaseEvent final : public InputEventContext
	{
	public:
		KeyCode getKey() const
		{
			return mKey;
		}

	private:
		friend class WidgetInput;
		KeyReleaseEvent(Widget* _target, KeyCode _key) :
			InputEventContext(_target),
			mKey(_key)
		{
		}

		KeyCode mKey;
	};

} // namespace MyGUI

#endif // MYGUI_INPUT_EVENT_H_
