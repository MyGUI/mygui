#ifndef MYGUI_ROUTED_INPUT_TEST_SUPPORT_H_
#define MYGUI_ROUTED_INPUT_TEST_SUPPORT_H_

#include "MyGUI.h"
#include <functional>

namespace unittest
{

	// Identify which subscription fired when a test records several kinds of input.
	enum class InputKind
	{
		MouseMove,
		MouseDrag,
		MouseWheel,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseButtonClick,
		MouseButtonDoubleClick,
		KeyButtonPressed,
		KeyButtonReleased
	};

	inline void subscribeRouted(
		MyGUI::Widget* widget,
		const std::function<void(MyGUI::Widget*, MyGUI::InputEventContext&, InputKind)>& callback,
		int id)
	{
		widget->eventMouseMoveRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseMoveEvent& event)
			{ callback(sender, event, InputKind::MouseMove); },
			id);
		widget->eventMouseDragRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseButtonEvent& event)
			{ callback(sender, event, InputKind::MouseDrag); },
			id);
		widget->eventMouseWheelRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseWheelEvent& event)
			{ callback(sender, event, InputKind::MouseWheel); },
			id);
		widget->eventMouseButtonPressedRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseButtonEvent& event)
			{ callback(sender, event, InputKind::MouseButtonPressed); },
			id);
		widget->eventMouseButtonReleasedRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseButtonEvent& event)
			{ callback(sender, event, InputKind::MouseButtonReleased); },
			id);
		widget->eventMouseButtonClickRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseClickEvent& event)
			{ callback(sender, event, InputKind::MouseButtonClick); },
			id);
		widget->eventMouseButtonDoubleClickRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::MouseClickEvent& event)
			{ callback(sender, event, InputKind::MouseButtonDoubleClick); },
			id);
		widget->eventKeyButtonPressedRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::KeyPressEvent& event)
			{ callback(sender, event, InputKind::KeyButtonPressed); },
			id);
		widget->eventKeyButtonReleasedRouted += MyGUI::newDelegate(
			[callback](MyGUI::Widget* sender, MyGUI::KeyReleaseEvent& event)
			{ callback(sender, event, InputKind::KeyButtonReleased); },
			id);
	}

	inline void clearRouted(MyGUI::Widget* widget)
	{
		widget->eventMouseMoveRouted.clear();
		widget->eventMouseDragRouted.clear();
		widget->eventMouseWheelRouted.clear();
		widget->eventMouseButtonPressedRouted.clear();
		widget->eventMouseButtonReleasedRouted.clear();
		widget->eventMouseButtonClickRouted.clear();
		widget->eventMouseButtonDoubleClickRouted.clear();
		widget->eventKeyButtonPressedRouted.clear();
		widget->eventKeyButtonReleasedRouted.clear();
	}

}

#endif
