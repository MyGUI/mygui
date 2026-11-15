/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_WidgetInput.h"
#include "MyGUI_Widget.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	namespace
	{

		// Keep the original route stable across callbacks and invalidate destroyed widgets.
		class InputEventRoute final : public IUnlinkWidget
		{
		public:
			InputEventRoute(Widget*& _target, Widget*& _currentTarget) :
				mTarget(_target),
				mCurrentTarget(_currentTarget)
			{
				for (auto* widget = _target; widget != nullptr; widget = widget->getParent())
				{
					widgets.push_back(widget);
					if (widget->getWidgetStyle() == WidgetStyle::Popup)
						break;
				}
				WidgetManager::getInstance().registerUnlinker(this);
			}

			~InputEventRoute() override
			{
				WidgetManager::getInstance().unregisterUnlinker(this);
			}

			void _unlinkWidget(Widget* _widget) override
			{
				if (mTarget == _widget)
					mTarget = nullptr;
				if (mCurrentTarget == _widget)
					mCurrentTarget = nullptr;
				for (auto*& widget : widgets)
				{
					if (widget == _widget)
						widget = nullptr;
				}
			}

			bool valid(size_t _index) const
			{
				if (mTarget == nullptr)
					return false;
				for (size_t index = 0; index <= _index; ++index)
				{
					auto* widget = widgets[index];
					if (widget == nullptr || !widget->getInheritedEnabled() || !widget->getInheritedVisible())
						return false;
					if (index != 0 && widgets[index - 1]->getParent() != widget)
						return false;
				}
				return true;
			}

			VectorWidgetPtr widgets;

		private:
			Widget*& mTarget;
			Widget*& mCurrentTarget;
		};

	}

	WidgetInput::WidgetInput() = default;

	WidgetInput::~WidgetInput() = default;

	void WidgetInput::setMaskPick(const std::string& _filename)
	{
		if (_filename.empty())
			mOwnMaskPickInfo = MaskPickInfo();
		else if (!mOwnMaskPickInfo.load(_filename))
			MYGUI_LOG(Error, "mask not load '" << _filename << "'");
	}

	void WidgetInput::setMaskPick(const MaskPickInfo& _info)
	{
		mOwnMaskPickInfo = _info;
	}

	bool WidgetInput::isMaskPickInside(const IntPoint& _point, const IntCoord& _coord) const
	{
		return mOwnMaskPickInfo.empty() || mOwnMaskPickInfo.pick(_point, _coord);
	}

	template<typename Event, typename LegacyDelegate, typename... Args>
	void WidgetInput::riseInputEvent(
		void (WidgetInput::*_legacyHook)(Args...),
		LegacyDelegate& _legacyDelegate,
		void (WidgetInput::*_routedHook)(Event&),
		delegates::MultiDelegate<Widget*, Event&> WidgetInput::* _routedDelegate,
		Args... _args)
	{
		auto* target = static_cast<Widget*>(this);
		Event event(target, _args...);
		InputEventRoute route(event.mTarget, event.mCurrentTarget);
		for (size_t index = 0; index < route.widgets.size(); ++index)
		{
			auto canContinue = [&]
			{
				return !event.isHandled() && route.valid(index);
			};
			if (!canContinue())
				break;
			auto* widget = route.widgets[index];
			event.mCurrentTarget = widget;
			(widget->*_routedDelegate).invokeWhile(canContinue, widget, event);
			if (!canContinue())
				break;
			if (index == 0)
			{
				(widget->*_legacyHook)(_args...);
				if (!canContinue())
					break;
				_legacyDelegate(widget, _args...);
				if (!canContinue())
					break;
			}
			(widget->*_routedHook)(event);
		}
	}

	void WidgetInput::_riseMouseLostFocus(Widget* _new)
	{
		onMouseLostFocus(_new);
		eventMouseLostFocus(static_cast<Widget*>(this), _new);
	}

	void WidgetInput::_riseMouseSetFocus(Widget* _old)
	{
		onMouseSetFocus(_old);
		eventMouseSetFocus(static_cast<Widget*>(this), _old);
	}

	void WidgetInput::_riseMouseDrag(int _left, int _top, MouseButton _id)
	{
		riseInputEvent(
			&WidgetInput::onMouseDrag,
			eventMouseDrag,
			&WidgetInput::onMouseDragRouted,
			&WidgetInput::eventMouseDragRouted,
			_left,
			_top,
			_id);
	}

	void WidgetInput::_riseMouseMove(int _left, int _top)
	{
		riseInputEvent(
			&WidgetInput::onMouseMove,
			eventMouseMove,
			&WidgetInput::onMouseMoveRouted,
			&WidgetInput::eventMouseMoveRouted,
			_left,
			_top);
	}

	void WidgetInput::_riseMouseWheel(int _rel)
	{
		riseInputEvent(
			&WidgetInput::onMouseWheel,
			eventMouseWheel,
			&WidgetInput::onMouseWheelRouted,
			&WidgetInput::eventMouseWheelRouted,
			_rel);
	}

	void WidgetInput::_riseMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		InputManager::getInstance().endMouseInteraction(_id);
		riseInputEvent(
			&WidgetInput::onMouseButtonPressed,
			eventMouseButtonPressed,
			&WidgetInput::onMouseButtonPressedRouted,
			&WidgetInput::eventMouseButtonPressedRouted,
			_left,
			_top,
			_id);
	}

	void WidgetInput::_riseMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		InputManager::MouseInteractionRelease release(InputManager::getInstance(), _id);
		riseInputEvent(
			&WidgetInput::onMouseButtonReleased,
			eventMouseButtonReleased,
			&WidgetInput::onMouseButtonReleasedRouted,
			&WidgetInput::eventMouseButtonReleasedRouted,
			_left,
			_top,
			_id);
	}

	void WidgetInput::_riseMouseButtonClick()
	{
		riseInputEvent(
			&WidgetInput::onMouseButtonClick,
			eventMouseButtonClick,
			&WidgetInput::onMouseButtonClickRouted,
			&WidgetInput::eventMouseButtonClickRouted);
	}

	void WidgetInput::_riseMouseButtonDoubleClick()
	{
		riseInputEvent(
			&WidgetInput::onMouseButtonDoubleClick,
			eventMouseButtonDoubleClick,
			&WidgetInput::onMouseButtonDoubleClickRouted,
			&WidgetInput::eventMouseButtonDoubleClickRouted);
	}

	void WidgetInput::_riseKeyLostFocus(Widget* _new)
	{
		onKeyLostFocus(_new);
		eventKeyLostFocus(static_cast<Widget*>(this), _new);
	}

	void WidgetInput::_riseKeySetFocus(Widget* _old)
	{
		onKeySetFocus(_old);
		eventKeySetFocus(static_cast<Widget*>(this), _old);
	}

	void WidgetInput::_riseKeyButtonPressed(KeyCode _key, Char _char)
	{
		riseInputEvent(
			&WidgetInput::onKeyButtonPressed,
			eventKeyButtonPressed,
			&WidgetInput::onKeyButtonPressedRouted,
			&WidgetInput::eventKeyButtonPressedRouted,
			_key,
			_char);
	}

	void WidgetInput::_riseKeyButtonReleased(KeyCode _key)
	{
		riseInputEvent(
			&WidgetInput::onKeyButtonReleased,
			eventKeyButtonReleased,
			&WidgetInput::onKeyButtonReleasedRouted,
			&WidgetInput::eventKeyButtonReleasedRouted,
			_key);
	}

	void WidgetInput::_riseMouseChangeRootFocus(bool _focus)
	{
		onMouseChangeRootFocus(_focus);
		eventRootMouseChangeFocus(static_cast<Widget*>(this), _focus);
	}

	void WidgetInput::_riseKeyChangeRootFocus(bool _focus)
	{
		onKeyChangeRootFocus(_focus);
		eventRootKeyChangeFocus(static_cast<Widget*>(this), _focus);
	}

	void WidgetInput::setNeedToolTip(bool _value)
	{
		mNeedToolTip = _value;
	}

	bool WidgetInput::getNeedToolTip() const
	{
		return mNeedToolTip;
	}

	void WidgetInput::setPointer(std::string_view _value)
	{
		mPointer = _value;
	}

	const std::string& WidgetInput::getPointer() const
	{
		return mPointer;
	}

	void WidgetInput::setNeedKeyFocus(bool _value)
	{
		mNeedKeyFocus = _value;
	}

	bool WidgetInput::getNeedKeyFocus() const
	{
		return mNeedKeyFocus;
	}

	void WidgetInput::setNeedMouseFocus(bool _value)
	{
		mNeedMouseFocus = _value;
	}

	void WidgetInput::setInheritsPick(bool _value)
	{
		mInheritsPick = _value;
	}

	bool WidgetInput::getRootMouseFocus() const
	{
		return mRootMouseFocus;
	}

	bool WidgetInput::getRootKeyFocus() const
	{
		return mRootKeyFocus;
	}

	void WidgetInput::_setRootMouseFocus(bool _value)
	{
		mRootMouseFocus = _value;
	}

	void WidgetInput::_setRootKeyFocus(bool _value)
	{
		mRootKeyFocus = _value;
	}

	void WidgetInput::onMouseLostFocus(Widget* _new)
	{
	}

	void WidgetInput::onMouseSetFocus(Widget* _old)
	{
	}

	void WidgetInput::onMouseDrag(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseMove(int _left, int _top)
	{
	}

	void WidgetInput::onMouseInteractionEnd(MouseButton _button) noexcept
	{
	}

	void WidgetInput::onMouseWheel(int _rel)
	{
	}

	void WidgetInput::onMouseMoveRouted(MouseMoveEvent& _event)
	{
	}

	void WidgetInput::onMouseDragRouted(MouseButtonEvent& _event)
	{
	}

	void WidgetInput::onMouseWheelRouted(MouseWheelEvent& _event)
	{
	}

	void WidgetInput::onMouseButtonPressedRouted(MouseButtonEvent& _event)
	{
	}

	void WidgetInput::onMouseButtonReleasedRouted(MouseButtonEvent& _event)
	{
	}

	void WidgetInput::onMouseButtonClickRouted(MouseClickEvent& _event)
	{
	}

	void WidgetInput::onMouseButtonDoubleClickRouted(MouseClickEvent& _event)
	{
	}

	void WidgetInput::onKeyButtonPressedRouted(KeyPressEvent& _event)
	{
	}

	void WidgetInput::onKeyButtonReleasedRouted(KeyReleaseEvent& _event)
	{
	}

	void WidgetInput::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
	}

	void WidgetInput::onMouseButtonClick()
	{
	}

	void WidgetInput::onMouseButtonDoubleClick()
	{
	}

	void WidgetInput::onKeyLostFocus(Widget* _new)
	{
	}

	void WidgetInput::onKeySetFocus(Widget* _old)
	{
	}

	void WidgetInput::onKeyButtonPressed(KeyCode _key, Char _char)
	{
	}

	void WidgetInput::onKeyButtonReleased(KeyCode _key)
	{
	}

	void WidgetInput::onMouseChangeRootFocus(bool _focus)
	{
	}

	void WidgetInput::onKeyChangeRootFocus(bool _focus)
	{
	}

} // namespace MyGUI
