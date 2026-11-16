#include "RoutedInputTestSupport.h"
#include "RoutedInput.h"
#include "BehaviourTestSupport.h"
#include <algorithm>
#include <array>
#include <functional>
#include <stdexcept>
#include <vector>

namespace
{

	using unittest::require;
	using Type = unittest::InputKind;

	[[noreturn]] void throwInputFailure()
	{
		throw std::runtime_error("nested input failure");
	}

	constexpr std::array types{
		Type::MouseMove,
		Type::MouseDrag,
		Type::MouseWheel,
		Type::MouseButtonPressed,
		Type::MouseButtonReleased,
		Type::MouseButtonClick,
		Type::MouseButtonDoubleClick,
		Type::KeyButtonPressed,
		Type::KeyButtonReleased};

	class InputWidget : public MyGUI::Widget
	{
		MYGUI_RTTI_DERIVED(InputWidget)
	public:
		using MyGUI::WidgetInput::beginMouseInteraction;
		std::function<void()> interactionEnd;
		std::vector<Type> legacy;
		std::function<void()> keyAction;
		std::function<void()> wheelAction;
		std::function<void(MyGUI::InputEventContext&, Type)> typedAction;

	protected:
		void onMouseInteractionEnd(MyGUI::Widget*, MyGUI::MouseButton) override
		{
			if (interactionEnd)
				interactionEnd();
		}
		void onMouseMoveRouted(MyGUI::MouseMoveEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseMove);
		}
		void onMouseDragRouted(MyGUI::MouseButtonEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseDrag);
		}
		void onMouseWheelRouted(MyGUI::MouseWheelEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseWheel);
		}
		void onMouseButtonPressedRouted(MyGUI::MouseButtonEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseButtonPressed);
		}
		void onMouseButtonReleasedRouted(MyGUI::MouseButtonEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseButtonReleased);
		}
		void onMouseButtonClickRouted(MyGUI::MouseClickEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseButtonClick);
		}
		void onMouseButtonDoubleClickRouted(MyGUI::MouseClickEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::MouseButtonDoubleClick);
		}
		void onKeyButtonPressedRouted(MyGUI::KeyPressEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::KeyButtonPressed);
		}
		void onKeyButtonReleasedRouted(MyGUI::KeyReleaseEvent& _event) override
		{
			if (typedAction)
				typedAction(_event, Type::KeyButtonReleased);
		}
		void onMouseMove(int, int) override
		{
			legacy.push_back(Type::MouseMove);
		}
		void onMouseDrag(int, int, MyGUI::MouseButton) override
		{
			legacy.push_back(Type::MouseDrag);
		}
		void onMouseWheel(int) override
		{
			legacy.push_back(Type::MouseWheel);
			if (wheelAction)
				wheelAction();
		}
		void onMouseButtonPressed(int, int, MyGUI::MouseButton) override
		{
			legacy.push_back(Type::MouseButtonPressed);
		}
		void onMouseButtonReleased(int, int, MyGUI::MouseButton) override
		{
			legacy.push_back(Type::MouseButtonReleased);
		}
		void onMouseButtonClick() override
		{
			legacy.push_back(Type::MouseButtonClick);
		}
		void onMouseButtonDoubleClick() override
		{
			legacy.push_back(Type::MouseButtonDoubleClick);
		}
		void onKeyButtonPressed(MyGUI::KeyCode, MyGUI::Char) override
		{
			legacy.push_back(Type::KeyButtonPressed);
			if (keyAction)
				keyAction();
		}
		void onKeyButtonReleased(MyGUI::KeyCode) override
		{
			legacy.push_back(Type::KeyButtonReleased);
		}
	};

	struct Fixture
	{
		unittest::TestContext context;
		InputWidget* parent;
		InputWidget* child;

		Fixture()
		{
			unittest::createInputLayer();
			MyGUI::FactoryManager::getInstance().registerFactory<InputWidget>(
				MyGUI::WidgetManager::getInstance().getCategoryName());
			parent = context.getGui().createWidget<InputWidget>(
				"Default",
				MyGUI::IntCoord(0, 0, 200, 200),
				MyGUI::Align::Default,
				"Main");
			child =
				parent->createWidget<InputWidget>("Default", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default);
			require(child->isType<InputWidget>(), "The fixture must use the custom input widget factory");
			child->setNeedKeyFocus(true);
		}
	};

	void raise(MyGUI::Widget* _widget, Type _type)
	{
		switch (_type)
		{
		case Type::MouseMove: _widget->_riseMouseMove(27, 39); break;
		case Type::MouseDrag: _widget->_riseMouseDrag(27, 39, MyGUI::MouseButton::Right); break;
		case Type::MouseWheel: _widget->_riseMouseWheel(-3); break;
		case Type::MouseButtonPressed: _widget->_riseMouseButtonPressed(27, 39, MyGUI::MouseButton::Right); break;
		case Type::MouseButtonReleased: _widget->_riseMouseButtonReleased(27, 39, MyGUI::MouseButton::Right); break;
		case Type::MouseButtonClick: _widget->_riseMouseButtonClick(); break;
		case Type::MouseButtonDoubleClick: _widget->_riseMouseButtonDoubleClick(); break;
		case Type::KeyButtonPressed: _widget->_riseKeyButtonPressed(MyGUI::KeyCode::A, U'\u03b1'); break;
		case Type::KeyButtonReleased: _widget->_riseKeyButtonReleased(MyGUI::KeyCode::A); break;
		}
	}

	void subscribeLegacy(MyGUI::Widget* _widget, std::vector<Type>& _events)
	{
		_widget->eventMouseMove +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int, int) { _events.push_back(Type::MouseMove); }, 1);
		_widget->eventMouseDrag += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { _events.push_back(Type::MouseDrag); },
			2);
		_widget->eventMouseWheel +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int) { _events.push_back(Type::MouseWheel); }, 3);
		_widget->eventMouseButtonPressed += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { _events.push_back(Type::MouseButtonPressed); },
			4);
		_widget->eventMouseButtonReleased += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { _events.push_back(Type::MouseButtonReleased); },
			5);
		_widget->eventMouseButtonClick +=
			MyGUI::newDelegate([&](MyGUI::Widget*) { _events.push_back(Type::MouseButtonClick); }, 6);
		_widget->eventMouseButtonDoubleClick +=
			MyGUI::newDelegate([&](MyGUI::Widget*) { _events.push_back(Type::MouseButtonDoubleClick); }, 7);
		_widget->eventKeyButtonPressed += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::KeyCode, MyGUI::Char) { _events.push_back(Type::KeyButtonPressed); },
			8);
		_widget->eventKeyButtonReleased +=
			MyGUI::newDelegate([&](MyGUI::Widget*, MyGUI::KeyCode) { _events.push_back(Type::KeyButtonReleased); }, 9);
	}

}


void testRoutedInputCaptureAndKeys()
{
	std::vector<Type> events;
	Fixture fixture;
	unittest::subscribeRouted(
		fixture.parent,
		[&](MyGUI::InputEventContext& event, Type kind)
		{
			require(
				event.getTarget() == fixture.child,
				"Capture and keyboard focus must determine the original target");
			events.push_back(kind);
		},
		1);
	auto& input = MyGUI::InputManager::getInstance();
	input.injectMousePress(20, 20, MyGUI::MouseButton::Left);
	input.injectMouseMove(350, 350, 0);
	require(
		input.getMouseFocusWidget() == fixture.child && input.isCaptureMouse(),
		"Bubbling must not transfer mouse capture to an ancestor");
	input.injectMouseRelease(20, 20, MyGUI::MouseButton::Left);
	require(!input.isCaptureMouse(), "Release must end capture normally");
	input.injectMousePress(20, 20, MyGUI::MouseButton::Left);
	input.injectMouseRelease(20, 20, MyGUI::MouseButton::Left);
	input.injectKeyPress(MyGUI::KeyCode::A, 'a');
	input.injectKeyRelease(MyGUI::KeyCode::A);
	require(input.getKeyFocusWidget() == fixture.child, "Bubbling must preserve keyboard focus");
	for (auto type : {Type::MouseDrag, Type::MouseButtonDoubleClick, Type::KeyButtonPressed, Type::KeyButtonReleased})
		require(std::count(events.begin(), events.end(), type) == 1, "Each generated input must route exactly once");
	require(
		std::count(events.begin(), events.end(), Type::MouseButtonClick) == 2,
		"Two clicks must not activate ancestors again");
	require(fixture.parent->legacy.empty(), "Routed capture and keyboard input must not execute parent defaults");

	events.clear();
	fixture.child->_riseMouseSetFocus(nullptr);
	fixture.child->_riseMouseLostFocus(nullptr);
	fixture.child->_riseKeySetFocus(nullptr);
	fixture.child->_riseKeyLostFocus(nullptr);
	fixture.child->_riseMouseChangeRootFocus(true);
	fixture.child->_riseKeyChangeRootFocus(true);
	fixture.child->setCoord(10, 10, 60, 60);
	require(events.empty(), "Focus and geometry notifications must not enter the input route");
}

void testRoutedInputBoundaries()
{
	Fixture fixture;
	int parentCalls = 0, popupCalls = 0;
	unittest::subscribeRouted(fixture.parent, [&](MyGUI::InputEventContext&, Type) { ++parentCalls; }, 1);
	auto* popup = fixture.parent->createWidget<MyGUI::Widget>(
		MyGUI::WidgetStyle::Popup,
		"Default",
		MyGUI::IntCoord(20, 20, 100, 100),
		MyGUI::Align::Default,
		"Main");
	auto* popupChild =
		popup->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 20, 20), MyGUI::Align::Default);
	unittest::subscribeRouted(popup, [&](MyGUI::InputEventContext&, Type) { ++popupCalls; }, 2);
	for (auto type : types)
		raise(popupChild, type);
	require(popupCalls == 9 && parentCalls == 0, "Routes must stop at popup roots");

	unittest::subscribeRouted(
		fixture.child,
		[&](MyGUI::InputEventContext&, Type) { fixture.parent->setEnabled(false); },
		3);
	raise(fixture.child, Type::MouseWheel);
	require(parentCalls == 0, "Disabling an ancestor during input must stop propagation");
	fixture.parent->setEnabled(true);
	unittest::clearRouted(fixture.child);
	unittest::subscribeRouted(
		fixture.child,
		[&](MyGUI::InputEventContext&, Type) { fixture.parent->setVisible(false); },
		4);
	raise(fixture.child, Type::KeyButtonPressed);
	require(parentCalls == 0, "Hiding an ancestor during input must stop propagation");
}

void testRoutedInputLifetime()
{
	{
		Fixture fixture;
		int gainedFocus = 0;
		fixture.child->eventMouseSetFocus +=
			MyGUI::newDelegate([&](MyGUI::Widget*, MyGUI::Widget*) { ++gainedFocus; }, 1);
		unittest::subscribeRouted(
			fixture.child,
			[&](MyGUI::InputEventContext& event, Type kind)
			{
				if (kind == Type::MouseMove)
					fixture.context.getGui().destroyWidget(event.getCurrentTarget());
			},
			2);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(20, 20, 0);
		require(
			gainedFocus == 0 && input.getMouseFocusWidget() == nullptr && !fixture.parent->getRootMouseFocus(),
			"Destroying a new hover target must not assign focus to an unlinked widget");
	}
	{
		Fixture fixture;
		int typedCalls = 0, legacyCalls = 0;
		fixture.child->keyAction = [&]
		{
			fixture.context.getGui().destroyWidget(fixture.child);
		};
		fixture.child->eventKeyButtonPressed +=
			MyGUI::newDelegate([&](MyGUI::Widget*, MyGUI::KeyCode, MyGUI::Char) { ++legacyCalls; }, 1);
		unittest::subscribeRouted(fixture.child, [&](MyGUI::InputEventContext&, Type) { ++typedCalls; }, 2);
		raise(fixture.child, Type::KeyButtonPressed);
		require(
			typedCalls == 1 && legacyCalls == 0,
			"Destruction in a legacy hook must skip its delegates after the target typed subscriber ran");
	}
	{
		Fixture fixture;
		bool sawUnlinkedTarget = false;
		unittest::subscribeRouted(
			fixture.parent,
			[&](MyGUI::InputEventContext&, Type kind)
			{
				if (kind == Type::MouseDrag)
					fixture.context.getGui().destroyWidget(fixture.child);
			},
			1);
		unittest::subscribeRouted(
			fixture.parent,
			[&](MyGUI::InputEventContext& event, Type kind)
			{
				if (kind == Type::MouseDrag)
					sawUnlinkedTarget = event.getTarget() == nullptr && event.getCurrentTarget() == fixture.parent;
			},
			2);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMousePress(20, 20, MyGUI::MouseButton::Left);
		input.injectMouseMove(350, 350, 0);
		require(
			!sawUnlinkedTarget && input.getMouseFocusWidget() == nullptr && !input.isCaptureMouse(),
			"Destroying a captured target must skip later listeners and safely release capture during nested "
			"dispatch");
	}
}

void testRoutedInputReentrancy()
{
	std::vector<Type> parentEvents;
	Fixture fixture;
	fixture.child->eventKeyButtonPressedRouted +=
		MyGUI::newDelegate([](MyGUI::KeyPressEvent& event) { event.setHandled(); }, 1);
	fixture.child->wheelAction = [&]
	{
		raise(fixture.child, Type::KeyButtonPressed);
	};
	unittest::subscribeRouted(
		fixture.parent,
		[&](MyGUI::InputEventContext&, Type kind) { parentEvents.push_back(kind); },
		2);
	raise(fixture.child, Type::MouseWheel);
	require(
		parentEvents == std::vector<Type>{Type::MouseWheel} && fixture.child->legacy == parentEvents,
		"Consuming nested key input inside a legacy wheel hook must leave the outer wheel unhandled");

	parentEvents.clear();
	fixture.child->legacy.clear();
	fixture.child->wheelAction = {};
	fixture.child->eventKeyButtonPressedRouted.clear();
	fixture.child->keyAction = throwInputFailure;
	fixture.child->eventMouseWheelRouted += MyGUI::newDelegate(
		[&](MyGUI::MouseWheelEvent& event)
		{
			unittest::requireThrows<std::runtime_error>(
				[&] { raise(fixture.child, Type::KeyButtonPressed); },
				"Nested input exceptions must propagate to the calling handler");
			event.setHandled();
		},
		3);
	raise(fixture.child, Type::MouseWheel);
	require(
		parentEvents.empty() && fixture.child->legacy == std::vector<Type>{Type::KeyButtonPressed},
		"The outer typed handler must still be able to intercept its own input after a nested exception");

	fixture.child->eventMouseWheelRouted.clear();
	raise(fixture.child, Type::MouseWheel);
	require(parentEvents == std::vector<Type>{Type::MouseWheel}, "Later input must start unhandled");
}

void testTypedRoutedInputTypes()
{
	std::vector<int> stages;
	Fixture fixture;
	int typedCalls = 0;
	Type raised{};
	for (auto* widget : {fixture.child, fixture.parent})
	{
		widget->typedAction = [&](MyGUI::InputEventContext& event, Type kind)
		{
			require(
				kind == raised && !event.isHandled(),
				"Only the matching default hook must receive unconsumed input");
			stages.push_back(2);
		};
		auto receive = [&, widget](MyGUI::InputEventContext& event, Type expected)
		{
			require(
				raised == expected && event.getTarget() == fixture.child && event.getCurrentTarget() == widget &&
					!event.isHandled(),
				"Only the matching typed event must receive the original target and shared context");
			++typedCalls;
			stages.push_back(1);
		};
		widget->eventMouseMoveRouted += MyGUI::newDelegate(
			[receive](MyGUI::MouseMoveEvent& event)
			{
				require(event.getPosition() == MyGUI::IntPoint(27, 39), "Typed move must expose its position");
				receive(event, Type::MouseMove);
			},
			2);
		auto receiveButton = [receive](MyGUI::MouseButtonEvent& event, Type expected)
		{
			require(
				event.getPosition() == MyGUI::IntPoint(27, 39) && event.getButton() == MyGUI::MouseButton::Right,
				"Typed button input must expose position and button");
			receive(event, expected);
		};
		widget->eventMouseDragRouted += MyGUI::newDelegate(
			[receiveButton](MyGUI::MouseButtonEvent& event) { receiveButton(event, Type::MouseDrag); },
			3);
		widget->eventMouseButtonPressedRouted += MyGUI::newDelegate(
			[receiveButton](MyGUI::MouseButtonEvent& event) { receiveButton(event, Type::MouseButtonPressed); },
			4);
		widget->eventMouseButtonReleasedRouted += MyGUI::newDelegate(
			[receiveButton](MyGUI::MouseButtonEvent& event) { receiveButton(event, Type::MouseButtonReleased); },
			5);
		widget->eventMouseWheelRouted += MyGUI::newDelegate(
			[receive](MyGUI::MouseWheelEvent& event)
			{
				require(event.getWheelDelta() == -3, "Typed wheel input must expose its delta");
				receive(event, Type::MouseWheel);
			},
			6);
		widget->eventMouseButtonClickRouted +=
			MyGUI::newDelegate([receive](MyGUI::MouseClickEvent& event) { receive(event, Type::MouseButtonClick); }, 7);
		widget->eventMouseButtonDoubleClickRouted += MyGUI::newDelegate(
			[receive](MyGUI::MouseClickEvent& event) { receive(event, Type::MouseButtonDoubleClick); },
			8);
		widget->eventKeyButtonPressedRouted += MyGUI::newDelegate(
			[receive](MyGUI::KeyPressEvent& event)
			{
				require(
					event.getKey() == MyGUI::KeyCode::A && event.getCharacter() == U'\u03b1',
					"Typed key press must expose the key and Unicode character");
				receive(event, Type::KeyButtonPressed);
			},
			9);
		widget->eventKeyButtonReleasedRouted += MyGUI::newDelegate(
			[receive](MyGUI::KeyReleaseEvent& event)
			{
				require(event.getKey() == MyGUI::KeyCode::A, "Typed key release must expose its key");
				receive(event, Type::KeyButtonReleased);
			},
			10);
	}
	std::vector<Type> legacyNotifications;
	subscribeLegacy(fixture.child, legacyNotifications);
	for (auto type : types)
	{
		stages.clear();
		raised = type;
		raise(fixture.child, type);
		require(
			stages == std::vector<int>{1, 2, 1, 2},
			"Typed subscribers must run before default hooks at each widget");
	}
	require(
		legacyNotifications == std::vector<Type>(types.begin(), types.end()) && typedCalls == 18 &&
			fixture.child->legacy == std::vector<Type>(types.begin(), types.end()) && fixture.parent->legacy.empty(),
		"Typed routing must not duplicate legacy/default actions");
}

void testTypedRoutedInputConsumption()
{
	Fixture fixture;
	std::vector<Type> legacyNotifications;
	subscribeLegacy(fixture.child, legacyNotifications);
	int parentCalls = 0, firstCalls = 0, laterCalls = 0, defaultCalls = 0;
	unittest::subscribeRouted(fixture.parent, [&](MyGUI::InputEventContext&, Type) { ++parentCalls; }, 1);
	unittest::subscribeRouted(
		fixture.child,
		[&](MyGUI::InputEventContext& event, Type)
		{
			++firstCalls;
			event.setHandled();
		},
		2);
	unittest::subscribeRouted(
		fixture.child,
		[&](MyGUI::InputEventContext& event, Type)
		{
			++laterCalls;
			event.setHandled(false);
		},
		3);
	fixture.child->typedAction = [&](MyGUI::InputEventContext&, Type)
	{
		++defaultCalls;
	};
	for (auto type : types)
		raise(fixture.child, type);
	require(
		firstCalls == 9 && laterCalls == 0 && fixture.child->legacy.empty() && legacyNotifications.empty() &&
			defaultCalls == 0 && parentCalls == 0,
		"Subscribers must be able to skip later subscribers, legacy callbacks, defaults and ancestors for every input "
		"kind");
	unittest::clearRouted(fixture.child);
	unittest::subscribeRouted(fixture.child, [&](MyGUI::InputEventContext&, Type) { ++laterCalls; }, 4);
	fixture.child->typedAction = [&](MyGUI::InputEventContext& event, Type)
	{
		++defaultCalls;
		event.setHandled();
	};
	for (auto type : types)
		raise(fixture.child, type);
	require(
		laterCalls == 9 && legacyNotifications == std::vector<Type>(types.begin(), types.end()) &&
			fixture.child->legacy == legacyNotifications && defaultCalls == 9 && parentCalls == 0,
		"Default hooks must run after subscribers and may consume input before ancestors");
	fixture.child->typedAction = {};
	unittest::clearRouted(fixture.child);
	fixture.child->eventMouseWheelRouted += MyGUI::newDelegate(
		[](MyGUI::MouseWheelEvent& event)
		{
			event.setHandled();
			event.setHandled(false);
		},
		5);
	raise(fixture.child, Type::MouseWheel);
	require(parentCalls == 1, "A handler can revise its consumption before returning");
}


void testTypedRoutedInputLifetime()
{
	{
		Fixture fixture;
		int typedCalls = 0;
		fixture.child->typedAction = [&](MyGUI::InputEventContext&, Type)
		{
			fixture.context.getGui().destroyWidget(fixture.child);
		};
		fixture.parent->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { ++typedCalls; }, 1);
		raise(fixture.child, Type::MouseWheel);
		require(typedCalls == 0, "Destruction in a typed virtual hook must skip ancestors");
	}
	{
		Fixture fixture;
		bool targetInvalidated = false;
		int laterCalls = 0;
		fixture.parent->eventMouseWheelRouted += MyGUI::newDelegate(
			[&](MyGUI::MouseWheelEvent& event)
			{
				fixture.context.getGui().destroyWidget(fixture.child);
				targetInvalidated = event.getTarget() == nullptr && event.getCurrentTarget() == fixture.parent;
			},
			1);
		fixture.parent->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { ++laterCalls; }, 2);
		raise(fixture.child, Type::MouseWheel);
		require(
			targetInvalidated && laterCalls == 0,
			"Target invalidation must update the typed event and skip later subscribers");
	}
	{
		Fixture fixture;
		int typedCalls = 0;
		unittest::subscribeRouted(
			fixture.child,
			[&](MyGUI::InputEventContext& event, Type)
			{ fixture.context.getGui().destroyWidget(event.getCurrentTarget()); },
			1);
		fixture.child->typedAction = [&](MyGUI::InputEventContext&, Type)
		{
			++typedCalls;
		};
		fixture.child->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { ++typedCalls; }, 2);
		raise(fixture.child, Type::MouseWheel);
		require(typedCalls == 0, "Destruction in the typed event must skip typed delivery on the invalid route");
	}
}

void testTypedRoutedInputReentrancy()
{
	std::vector<Type> received;
	Fixture fixture;
	fixture.child->eventMouseWheelRouted += MyGUI::newDelegate(
		[&](MyGUI::MouseWheelEvent& event)
		{
			raise(fixture.child, Type::KeyButtonPressed);
			require(
				!event.isHandled() && event.getWheelDelta() == -3 && event.getTarget() == fixture.child &&
					event.getCurrentTarget() == fixture.child,
				"Nested typed delivery must leave the outer event and its context intact");
		},
		1);
	fixture.parent->eventKeyButtonPressedRouted += MyGUI::newDelegate(
		[&](MyGUI::KeyPressEvent& event)
		{
			received.push_back(Type::KeyButtonPressed);
			event.setHandled();
		},
		2);
	fixture.parent->eventMouseWheelRouted +=
		MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { received.push_back(Type::MouseWheel); }, 3);
	raise(fixture.child, Type::MouseWheel);
	require(
		received == std::vector<Type>{Type::KeyButtonPressed, Type::MouseWheel},
		"Nested typed events must complete independently before the outer route resumes");
}

void testSharedMouseInteraction()
{
	Fixture fixture;
	auto* child = fixture.child;
	auto* parent = fixture.parent;
	const auto left = MyGUI::MouseButton::Left;
	int childEnds = 0, parentEnds = 0;
	child->interactionEnd = [&]
	{
		++childEnds;
	};
	parent->interactionEnd = [&]
	{
		++parentEnds;
	};
	child->beginMouseInteraction(child, left);
	parent->beginMouseInteraction(child, left);
	child->_riseMouseButtonReleased(0, 0, MyGUI::MouseButton::Right);
	require(childEnds == 0 && parentEnds == 0, "Another button must leave both interactions active");
	child->_riseMouseButtonReleased(0, 0, left);
	require(childEnds == 1 && parentEnds == 1, "Releasing the registered button must finish both participants");

	// The first callback unlinks the other participant while capture cleanup is in progress.
	parent->interactionEnd = [&]
	{
		++parentEnds;
		fixture.context.getGui().destroyWidget(child);
	};
	MyGUI::InputManager::getInstance().setKeyFocusWidget(child);
	parent->beginMouseInteraction(child, left);
	child->beginMouseInteraction(child, left);
	MyGUI::InputManager::getInstance().resetMouseCaptureWidget();
	require(
		childEnds == 2 && parentEnds == 2 && MyGUI::InputManager::getInstance().getKeyFocusWidget() == nullptr,
		"Unlink must finish each participant once and clear focus");
}
