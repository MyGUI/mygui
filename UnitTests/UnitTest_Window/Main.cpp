#include "RoutedInputTestSupport.h"
#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include <cmath>
#include <vector>

namespace
{

	using unittest::require;

	void testCoordinateConstraints()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setSnap(false);
		window->setMinSize(100, 60);
		window->setMaxSize(250, 160);
		struct ResizeCase
		{
			MyGUI::IntCoord requested;
			MyGUI::IntCoord expected;
		};
		const ResizeCase cases[] = {
			{{250, 190, 50, 30}, {200, 160, 100, 60}}, // Top/left resize keeps the opposite corner.
			{{-50, 10, 350, 210}, {50, 60, 250, 160}},
			{{100, 100, 50, 30}, {100, 100, 100, 60}}, // Bottom/right resize keeps the origin.
			{{100, 100, 350, 210}, {100, 100, 250, 160}},
			{{100, 100, 200, 120}, {100, 100, 200, 120}},
		};
		for (const auto& entry : cases)
		{
			window->setCoord(100, 100, 200, 120);
			window->setCoord(entry.requested);
			require(window->getCoord() == entry.expected, "Clamping a resize must preserve its anchored corner");
		}
		window->setMaxSize(800, 600);
		window->setSnap(true);
		window->setCoord(5, 7, 200, 120);
		require(
			window->getCoord() == MyGUI::IntCoord(0, 0, 205, 127),
			"Top/left snapping must extend size while preserving the opposite corner");
		window->setCoord(100, 100, 695, 493);
		require(
			window->getCoord() == MyGUI::IntCoord(100, 100, 700, 500),
			"Bottom/right snapping must extend size to the view edges");
		window->setSnap(false);
		window->setCoord(5, 7, 200, 120);
		require(
			window->getCoord() == MyGUI::IntCoord(5, 7, 200, 120),
			"Disabling snapping must preserve the requested rectangle");
	}

	void testAutomaticAlpha()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto& gui = context.getGui();
		auto* window = gui.createWidget<MyGUI::Window>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		auto& input = MyGUI::InputManager::getInstance();
		auto checkAlpha = [&](float expected)
		{
			gui.eventFrameStart(1.0f);
			require(
				std::abs(window->getAlpha() - expected) < 0.001f,
				"Automatic opacity must follow the current focus state");
		};
		window->setAutoAlpha(true);
		checkAlpha(0.3f);
		input.injectMouseMove(110, 110, 0);
		checkAlpha(0.7f);
		window->setAutoAlpha(true);
		checkAlpha(0.7f);
		input.setKeyFocusWidget(window);
		checkAlpha(1.0f);
		window->setAutoAlpha(true);
		checkAlpha(1.0f);
		input.injectMouseMove(500, 500, 0);
		checkAlpha(1.0f);
		input.setKeyFocusWidget(nullptr);
		checkAlpha(0.3f);
		window->setAutoAlpha(false);
		checkAlpha(1.0f);
		input.injectMouseMove(110, 110, 0);
		checkAlpha(1.0f);
		input.injectMouseMove(500, 500, 0);
		checkAlpha(1.0f);
	}

	void testMovementAndResize()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setSnap(false);
		int changes = 0;
		window->eventWindowChangeCoord += MyGUI::newDelegate([&changes](MyGUI::Window*) { ++changes; }, 1);
		unittest::dragFromTo({120, 110}, {150, 150});
		require(window->getCoord() == MyGUI::IntCoord(130, 140, 200, 120), "Caption drag must translate the window");
		require(changes == 1, "One drag movement must emit one coordinate event");
		window->setMovable(false);
		unittest::dragFromTo({150, 150}, {200, 200});
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140) && changes == 1,
			"Nonmovable window must ignore caption drags");
		window->setMinSize(100, 60);
		window->setMaxSize(250, 160);
		unittest::dragFromTo({325, 255}, {425, 355});
		require(window->getSize() == MyGUI::IntSize(250, 160), "Resize must clamp to maximum dimensions");
		unittest::dragFromTo({375, 295}, {175, 195});
		require(window->getSize() == MyGUI::IntSize(100, 60), "Resize must clamp to minimum dimensions");
		window->setSize(200, 120);
		unittest::dragFromTo({135, 255}, {155, 265});
		require(
			window->getCoord() == MyGUI::IntCoord(150, 140, 180, 130),
			"Left resize must preserve the opposite edge");
	}

	void testRotatedWindowDrag()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setRotation(0.7853981634f);
		const MyGUI::FloatPoint start = window->rotatePoint(MyGUI::FloatPoint(140.0f, 110.0f));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(press.left, press.top, 0);
		input.injectMousePress(press.left, press.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(press.left + 30, press.top + 40, 0);
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140),
			"Rotated window must follow the first mouse displacement");
		input.injectMouseMove(press.left + 60, press.top + 30, 0);
		require(
			window->getPosition() == MyGUI::IntPoint(160, 130),
			"Further dragging must not drift as the window moves");
		input.injectMouseMove(press.left, press.top, 0);
		require(
			window->getCoord() == MyGUI::IntCoord(100, 100, 200, 120),
			"Dragging back to the press position must restore the original window coordinates");
		input.injectMouseRelease(press.left, press.top, MyGUI::MouseButton::Left);
	}

	void testDragWithinRotatedParent()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* parent = context.getGui().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(50, 50, 400, 400),
			MyGUI::Align::Default,
			"Main");
		auto* window = parent->createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default);
		parent->setRotation(1.5707963268f);
		window->setRotation(0.7853981634f);
		const MyGUI::FloatPoint start = window->rotatePoint(MyGUI::FloatPoint(190.0f, 160.0f));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(press.left, press.top, 0);
		input.injectMousePress(press.left, press.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(press.left + 40, press.top, 0);
		require(
			window->getPosition() == MyGUI::IntPoint(100, 60),
			"Window movement must use the parent's axes, regardless of its own rotation");
		input.injectMouseMove(press.left, press.top, 0);
		require(
			window->getCoord() == MyGUI::IntCoord(100, 100, 200, 120),
			"Dragging back within a rotated parent must restore the original window coordinates");
		input.injectMouseRelease(press.left, press.top, MyGUI::MouseButton::Left);
	}

	void testRotatedWindowResize()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setRotation(1.5707963268f);
		const MyGUI::FloatPoint oppositeBefore = window->rotatePoint(MyGUI::FloatPoint(100.0f, 100.0f));
		const MyGUI::FloatPoint start = window->rotatePoint(MyGUI::FloatPoint(295.0f, 215.0f));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(press.left, press.top, 0);
		input.injectMousePress(press.left, press.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(press.left, press.top + 20, 0);
		require(
			window->getSize() == MyGUI::IntSize(220, 120),
			"Rotated resize handle must use the window's layout axes");
		require(
			window->getPosition() == MyGUI::IntPoint(90, 110),
			"Resizing around the default pivot must keep the opposite displayed corner fixed");
		input.injectMouseMove(press.left, press.top + 40, 0);
		require(window->getCoord() == MyGUI::IntCoord(80, 120, 240, 120), "Further resizing must not drift");
		const MyGUI::FloatPoint oppositeAfter = window->rotatePoint(MyGUI::FloatPoint(80.0f, 120.0f));
		require(
			std::abs(oppositeAfter.left - oppositeBefore.left) < 0.01f &&
				std::abs(oppositeAfter.top - oppositeBefore.top) < 0.01f,
			"The opposite displayed corner must stay anchored across drag updates");
		input.injectMouseMove(press.left, press.top, 0);
		require(
			window->getCoord() == MyGUI::IntCoord(100, 100, 200, 120),
			"Resizing back to the press position must restore the original size and anchored position");
		input.injectMouseRelease(press.left, press.top, MyGUI::MouseButton::Left);

		window->setCoord(MyGUI::IntCoord(100, 100, 200, 120));
		const MyGUI::FloatPoint otherOpposite = window->rotatePoint(MyGUI::FloatPoint(300.0f, 100.0f));
		const MyGUI::FloatPoint otherStart = window->rotatePoint(MyGUI::FloatPoint(105.0f, 215.0f));
		const MyGUI::IntPoint otherPress((int)std::lround(otherStart.left), (int)std::lround(otherStart.top));
		input.injectMouseMove(otherPress.left, otherPress.top, 0);
		input.injectMousePress(otherPress.left, otherPress.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(otherPress.left, otherPress.top + 20, 0);
		require(
			window->getCoord() == MyGUI::IntCoord(110, 110, 180, 120),
			"Left-bottom resize must keep the opposite displayed corner fixed");
		input.injectMouseMove(otherPress.left, otherPress.top + 40, 0);
		require(window->getCoord() == MyGUI::IntCoord(120, 120, 160, 120), "Left-bottom resize must not drift");
		const MyGUI::FloatPoint otherAfter = window->rotatePoint(MyGUI::FloatPoint(280.0f, 120.0f));
		require(
			std::abs(otherAfter.left - otherOpposite.left) < 0.01f &&
				std::abs(otherAfter.top - otherOpposite.top) < 0.01f,
			"The opposite displayed corner must stay anchored when position and size both change");
		input.injectMouseMove(otherPress.left, otherPress.top, 0);
		require(
			window->getCoord() == MyGUI::IntCoord(100, 100, 200, 120),
			"Left-bottom resizing back to the press position must restore the original window coordinates");
		input.injectMouseRelease(otherPress.left, otherPress.top, MyGUI::MouseButton::Left);
	}

	void testResizeNotifications()
	{
		for (bool leftEdge : {false, true})
		{
			unittest::TestContext context;
			unittest::createInputLayer();
			unittest::loadResources("UnitTest_Window/TestSkin.xml");
			auto* window = context.getGui().createWidget<MyGUI::Window>(
				"InteractionWindow",
				MyGUI::IntCoord(100, 100, 200, 120),
				MyGUI::Align::Default,
				"Main");
			window->setRotation(1.5707963268f);
			window->setMinSize(160, 60);
			window->setMaxSize(240, 160);
			std::vector<MyGUI::IntCoord> widgetChanges;
			std::vector<MyGUI::IntCoord> absoluteChanges;
			std::vector<MyGUI::IntCoord> windowChanges;
			window->eventChangeCoord +=
				MyGUI::newDelegate([&](MyGUI::Widget* sender) { widgetChanges.push_back(sender->getCoord()); }, 1);
			window->eventWindowChangeCoord +=
				MyGUI::newDelegate([&](MyGUI::Window* sender) { windowChanges.push_back(sender->getCoord()); }, 2);
			window->eventChangeAbsoluteCoord +=
				MyGUI::newDelegate([&](MyGUI::Widget* sender) { absoluteChanges.push_back(sender->getCoord()); }, 3);
			const auto start = window->localToLayer(MyGUI::FloatPoint(leftEdge ? 5.0f : 195.0f, 115.0f));
			const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
			auto& input = MyGUI::InputManager::getInstance();
			input.injectMouseMove(press.left, press.top, 0);
			input.injectMousePress(press.left, press.top, MyGUI::MouseButton::Left);
			auto drag = [&](int delta, const MyGUI::IntCoord& expected, bool changed)
			{
				widgetChanges.clear();
				absoluteChanges.clear();
				windowChanges.clear();
				input.injectMouseMove(press.left, press.top + delta, 0);
				require(window->getCoord() == expected, "Resize must preserve the final anchored geometry");
				require(
					widgetChanges.size() == (changed ? 1u : 0u),
					"Resize must emit one widget event per actual change");
				if (changed)
					require(
						widgetChanges.front() == expected,
						"Widget callback must observe the final anchored geometry");
				require(
					absoluteChanges == widgetChanges,
					"Absolute coordinate notifications must also observe only final geometry");
				require(
					windowChanges.size() == 1 && windowChanges.front() == expected,
					"Window event must observe the completed drag update");
			};
			drag(20, leftEdge ? MyGUI::IntCoord(110, 110, 180, 120) : MyGUI::IntCoord(90, 110, 220, 120), true);
			const MyGUI::IntCoord limit =
				leftEdge ? MyGUI::IntCoord(120, 120, 160, 120) : MyGUI::IntCoord(80, 120, 240, 120);
			drag(60, limit, true);
			drag(80, limit, false);
			drag(0, MyGUI::IntCoord(100, 100, 200, 120), true);
			widgetChanges.clear();
			windowChanges.clear();
			input.injectMouseRelease(press.left, press.top, MyGUI::MouseButton::Left);
			require(widgetChanges.empty() && windowChanges.empty(), "Release must not emit coordinate events");
		}
	}

	void testResizeWithinRotatedParent()
	{
		for (auto style : {MyGUI::WidgetStyle::Child, MyGUI::WidgetStyle::Popup})
			for (bool customCenter : {false, true})
				for (bool leftEdge : {false, true})
					for (float radians : {0.7853981634f, 1.5707963268f})
					{
						unittest::TestContext context;
						unittest::createInputLayer();
						unittest::loadResources("UnitTest_Window/TestSkin.xml");
						auto* parent = context.getGui().createWidget<MyGUI::Widget>(
							"Default",
							MyGUI::IntCoord(50, 50, 400, 400),
							MyGUI::Align::Default,
							"Main");
						parent->setRotation(1.5707963268f);
						auto* window = parent->createWidget<MyGUI::Window>(
							style,
							"InteractionWindow",
							MyGUI::IntCoord(100, 100, 200, 120),
							MyGUI::Align::Default,
							style == MyGUI::WidgetStyle::Popup ? "Main" : "");
						window->setRotation(radians);
						if (customCenter)
							window->setRotationCenter(MyGUI::FloatPoint(30, 40));
						const auto anchor = window->localToLayer(MyGUI::FloatPoint(leftEdge ? 200.0f : 0.0f, 0));
						const auto start = window->localToLayer(MyGUI::FloatPoint(leftEdge ? 5.0f : 195.0f, 115));
						int localEvents = 0;
						int absoluteEvents = 0;
						auto checkAnchor = [&](MyGUI::Widget* sender)
						{
							const auto actual =
								sender->localToLayer(MyGUI::FloatPoint(leftEdge ? (float)sender->getWidth() : 0.0f, 0));
							require(
								std::abs(actual.left - anchor.left) < 0.51f &&
									std::abs(actual.top - anchor.top) < 0.51f,
								"Resize callbacks must observe the preserved anchor with inherited rotation and custom "
								"pivots");
						};
						window->eventChangeCoord += MyGUI::newDelegate(
							[&](MyGUI::Widget* sender)
							{
								++localEvents;
								checkAnchor(sender);
							},
							1);
						window->eventChangeAbsoluteCoord += MyGUI::newDelegate(
							[&](MyGUI::Widget* sender)
							{
								++absoluteEvents;
								checkAnchor(sender);
							},
							2);
						const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
						const auto target = window->localToLayer(MyGUI::FloatPoint(leftEdge ? 25.0f : 215.0f, 115));
						unittest::dragFromTo(
							press,
							MyGUI::IntPoint((int)std::lround(target.left), (int)std::lround(target.top)));
						require(
							window->getSize() == MyGUI::IntSize(leftEdge ? 180 : 220, 120),
							"Nested resize must use the window's axes");
						require(
							localEvents == 1 && absoluteEvents == 1,
							"Nested resize must notify final local and absolute coordinates once");
					}
	}

	void testResizeCallbackCanSetCoordinates()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setRotation(1.5707963268f);
		std::vector<MyGUI::IntCoord> changes;
		window->eventChangeCoord += MyGUI::newDelegate(
			[&](MyGUI::Widget* sender)
			{
				changes.push_back(sender->getCoord());
				if (changes.size() == 1)
					window->setCoord(MyGUI::IntCoord(50, 60, 230, 130));
			},
			1);
		const auto start = window->localToLayer(MyGUI::FloatPoint(195, 115));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		unittest::dragFromTo(press, MyGUI::IntPoint(press.left, press.top + 20));
		require(
			changes.size() == 2 && changes[0] == MyGUI::IntCoord(90, 110, 220, 120) &&
				changes[1] == MyGUI::IntCoord(50, 60, 230, 130),
			"A callback's explicit coordinate update must be notified immediately");
		require(window->getCoord() == changes.back(), "The resize must not overwrite callback coordinate changes");
	}

	void testRotatedResizeSnapping()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(550, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setRotation(1.5707963268f);
		window->setSnap(true);
		std::vector<MyGUI::IntCoord> changes;
		window->eventChangeCoord +=
			MyGUI::newDelegate([&](MyGUI::Widget* sender) { changes.push_back(sender->getCoord()); }, 1);
		const auto start = window->localToLayer(MyGUI::FloatPoint(195, 115));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		unittest::dragFromTo(press, MyGUI::IntPoint(press.left, press.top + 45));
		require(
			changes.size() == 1 && changes.front() == MyGUI::IntCoord(525, 125, 250, 120),
			"Resize must snap size before applying the anchor correction and notify the final rectangle once");
	}

	void testSnappingAndVisibility()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setSnap(true);
		window->setPosition(7, -5);
		require(window->getPosition() == MyGUI::IntPoint(0, 0), "Near top/left edges must snap");
		window->setPosition(595, 475);
		require(window->getPosition() == MyGUI::IntPoint(600, 480), "Near bottom/right edges must snap");
		window->setSnap(false);
		window->setPosition(7, 5);
		require(window->getPosition() == MyGUI::IntPoint(7, 5), "Disabled snapping must preserve coordinates");
		window->setVisibleSmooth(false);
		context.getGui().eventFrameStart(1.0f);
		require(!window->getVisible(), "Fade-out must finish by hiding the window");
		window->setVisibleSmooth(true);
		context.getGui().eventFrameStart(1.0f);
		require(window->getVisible() && window->getAlpha() == 1.0f, "Fade-in must restore visibility and alpha");
		window->destroySmooth();
		context.getGui().eventFrameStart(1.0f);
		context.getGui().eventFrameStart(1.0f);
	}

	void testDirectRoutedWindowActions()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setSnap(false);
		window->setUserString("Scale", "1 1 0 0");
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(120, 110, 0);
		require(input.getMouseFocusWidget() == window, "Fixture must target the window itself");

		for (auto* subscription : {&window->eventMouseButtonPressedRouted, &window->eventMouseDragRouted})
		{
			*subscription +=
				MyGUI::newDelegate([](MyGUI::Widget*, MyGUI::MouseButtonEvent& event) { event.setHandled(); }, 1);
			unittest::dragFromTo({120, 110}, {150, 150});
			require(
				window->getPosition() == MyGUI::IntPoint(100, 100),
				"Typed handlers must intercept direct press and drag before Window's default action");
			subscription->clear();
		}

		int legacyPresses = 0;
		window->eventMouseButtonPressed +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { ++legacyPresses; }, 2);
		unittest::dragFromTo({120, 110}, {150, 150});
		require(
			legacyPresses == 1 && window->getPosition() == MyGUI::IntPoint(130, 140),
			"Legacy press observers must not prevent direct movement");
		window->eventMouseButtonPressed.clear();
		window->setPosition(100, 100);

		int changes = 0;
		window->eventWindowChangeCoord += MyGUI::newDelegate([&](MyGUI::Window*) { ++changes; }, 3);
		unittest::dragFromTo({120, 110}, {150, 150});
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140) && changes == 1,
			"Unconsumed direct movement must run once");
		input.injectMouseMove(160, 160, 0);
		window->_riseMouseDrag(160, 160, MyGUI::MouseButton::Left);
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140) && changes == 1,
			"Direct release must clear the active move action");

		window->setUserString("Scale", "0 0 1 1");
		unittest::dragFromTo({150, 150}, {180, 190});
		require(
			window->getCoord() == MyGUI::IntCoord(130, 140, 230, 160) && changes == 2,
			"Unconsumed direct resizing must run once");
	}

	void testWindowActionConsumption()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* parent = context.getGui().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(0, 0, 600, 500),
			MyGUI::Align::Default,
			"Main");
		auto* window = parent->createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default);
		window->setSnap(false);
		window->setUserString("Scale", "1 1 0 0");
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(120, 110, 0);
		auto* caption = input.getMouseFocusWidget();
		require(caption != nullptr && caption != window, "Fixture must pick the caption");
		int bubbled = 0;
		for (auto* subscription :
			 {&parent->eventMouseButtonPressedRouted,
			  &parent->eventMouseButtonReleasedRouted,
			  &parent->eventMouseDragRouted})
			*subscription += MyGUI::newDelegate([&](MyGUI::Widget*, MyGUI::MouseButtonEvent&) { ++bubbled; }, 1);
		for (MyGUI::Widget* target : {static_cast<MyGUI::Widget*>(window), caption})
		{
			bubbled = 0;
			target->_riseMouseButtonPressed(120, 110, MyGUI::MouseButton::Right);
			target->_riseMouseDrag(150, 150, MyGUI::MouseButton::Right);
			target->_riseMouseButtonReleased(150, 150, MyGUI::MouseButton::Right);
			require(
				bubbled == 3 && window->getActionScale().empty(),
				"Right-button gestures must bubble without starting an action");

			target->_riseMouseDrag(150, 150, MyGUI::MouseButton::Left);
			target->_riseMouseButtonReleased(150, 150, MyGUI::MouseButton::Left);
			require(bubbled == 5, "Left-button drag and release without an active action must bubble");
			window->setMovable(false);
			target->_riseMouseButtonPressed(120, 110, MyGUI::MouseButton::Left);
			target->_riseMouseDrag(150, 150, MyGUI::MouseButton::Left);
			target->_riseMouseButtonReleased(150, 150, MyGUI::MouseButton::Left);
			require(
				bubbled == 8 && window->getActionScale().empty(),
				"Disabled movement must leave the entire gesture unhandled");
			window->setMovable(true);

			bubbled = 0;
			target->_riseMouseButtonPressed(120, 110, MyGUI::MouseButton::Left);
			require(
				bubbled == 0 && !window->getActionScale().empty(),
				"An actionable left press must start and consume the gesture");
			target->_riseMouseButtonReleased(150, 150, MyGUI::MouseButton::Right);
			require(
				bubbled == 1 && !window->getActionScale().empty(),
				"Right release must bubble without ending the left-button action");
			target->_riseMouseButtonReleased(150, 150, MyGUI::MouseButton::Left);
			require(
				bubbled == 1 && window->getActionScale().empty(),
				"Left release must end and consume the active action");
		}
		window->setUserString("Scale", "0 0 0 0");
		bubbled = 0;
		window->_riseMouseButtonPressed(120, 110, MyGUI::MouseButton::Left);
		require(
			bubbled == 1 && window->getActionScale().empty(),
			"A window surface without an action must let presses bubble");

		window->setMaxSize(200, 120);
		bubbled = 0;
		unittest::dragFromTo({295, 215}, {325, 245});
		require(
			bubbled == 0 && window->getSize() == MyGUI::IntSize(200, 120) && window->getActionScale().empty(),
			"An active resize at its size limit must remain handled through release");
	}

	void testWindowActionCleanup()
	{
		enum class EndMode
		{
			ConsumedRelease,
			Hide,
			Disable,
			Skin,
			CaptureReset
		};
		for (bool direct : {false, true})
		{
			for (auto mode :
				 {EndMode::ConsumedRelease, EndMode::Hide, EndMode::Disable, EndMode::Skin, EndMode::CaptureReset})
			{
				unittest::TestContext context;
				unittest::createInputLayer();
				unittest::loadResources("UnitTest_Window/TestSkin.xml");
				auto* window = context.getGui().createWidget<MyGUI::Window>(
					direct ? "Default" : "InteractionWindow",
					MyGUI::IntCoord(100, 100, 200, 120),
					MyGUI::Align::Default,
					"Main");
				window->setSnap(false);
				window->setUserString("Scale", "1 1 0 0");
				auto& input = MyGUI::InputManager::getInstance();
				input.injectMousePress(120, 110, MyGUI::MouseButton::Left);
				require(!window->getActionScale().empty(), "Press must start the interaction before cleanup");
				auto* target = input.getMouseFocusWidget();
				switch (mode)
				{
				case EndMode::ConsumedRelease:
					target->eventMouseButtonReleasedRouted += MyGUI::newDelegate(
						[](MyGUI::Widget*, MyGUI::MouseButtonEvent& event) { event.setHandled(); },
						1);
					input.injectMouseRelease(120, 110, MyGUI::MouseButton::Left);
					break;
				case EndMode::Hide:
					window->setVisible(false);
					window->setVisible(true);
					break;
				case EndMode::Disable:
					window->setEnabled(false);
					window->setEnabled(true);
					break;
				case EndMode::Skin: window->changeWidgetSkin(direct ? "Default" : "InteractionWindow"); break;
				case EndMode::CaptureReset: input.resetMouseCaptureWidget(); break;
				}
				require(
					window->getActionScale().empty(),
					"Release and cancellation must clear state independently of input propagation");
				input.resetMouseCaptureWidget();
				input.injectMouseMove(120, 110, 0);
				target = input.getMouseFocusWidget();
				require(target != nullptr, "The restored window must be pickable");
				target->eventMouseButtonReleasedRouted.clear();
				target->eventMouseButtonPressedRouted +=
					MyGUI::newDelegate([](MyGUI::Widget*, MyGUI::MouseButtonEvent& event) { event.setHandled(); }, 2);
				unittest::dragFromTo({120, 110}, {150, 150});
				require(
					window->getPosition() == MyGUI::IntPoint(100, 100),
					"A blocked new press must not reuse the old action");
			}
		}
	}

	void testNestedWindowInteraction()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setUserString("Scale", "1 1 0 0");
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMousePress(120, 110, MyGUI::MouseButton::Left);
		bool nested = false;
		window->eventMouseButtonReleasedRouted += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::MouseButtonEvent& event)
			{
				if (!nested)
				{
					nested = true;
					input.injectMousePress(120, 110, MyGUI::MouseButton::Left);
				}
				event.setHandled();
			},
			1);
		input.injectMouseRelease(120, 110, MyGUI::MouseButton::Left);
		require(
			!window->getActionScale().empty() && input.isCaptureMouse(),
			"Finishing an old release must preserve a nested new interaction");
		input.injectMouseRelease(120, 110, MyGUI::MouseButton::Left);
		require(
			window->getActionScale().empty() && !input.isCaptureMouse(),
			"The nested interaction must end on its own release");
	}

	void testWindowWheelBubbling()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		unittest::loadResources("UnitTest_Scrolling/TestSkin.xml");
		auto* outer = context.getGui().createWidget<MyGUI::ScrollView>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 400, 300),
			MyGUI::Align::Default,
			"Main");
		outer->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		outer->setCanvasSize(350, 800);
		auto* window = outer->createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(10, 10, 200, 120),
			MyGUI::Align::Default);
		int legacyCalls = 0;
		window->eventMouseWheel += MyGUI::newDelegate([&](MyGUI::Widget*, int) { ++legacyCalls; }, 1);
		auto& input = MyGUI::InputManager::getInstance();
		for (auto point : {MyGUI::IntPoint(195, 15), MyGUI::IntPoint(205, 125)})
		{
			outer->setViewOffset(MyGUI::IntPoint());
			input.injectMouseMove(point.left, point.top, 0);
			auto* part = input.getMouseFocusWidget();
			require(part != nullptr && part->getParent() == window, "Fixture must target a window skin part");
			int routedCalls = 0;
			bool blockWheel = false;
			window->eventMouseWheelRouted += MyGUI::newDelegate(
				[&](MyGUI::Widget* sender, MyGUI::MouseWheelEvent& event)
				{
					++routedCalls;
					require(
						sender == window && event.getTarget() == part && event.getWheelDelta() == -1,
						"Routed wheel input must preserve the original skin part and delta");
					event.setHandled(blockWheel);
				},
				2);
			part->_riseMouseWheel(-1);
			require(
				legacyCalls == 0 && routedCalls == 1 && outer->getViewOffset().top == -50,
				"Wheel input on action and resize parts must bypass Window legacy callbacks and scroll the parent");
			blockWheel = true;
			part->_riseMouseWheel(-1);
			require(
				routedCalls == 2 && outer->getViewOffset().top == -50,
				"Window routed handlers must be able to prevent ancestor scrolling");
			window->eventMouseWheelRouted.clear();
		}
		window->_riseMouseWheel(-1);
		require(
			legacyCalls == 1 && outer->getViewOffset().top == -100,
			"Direct wheel input must notify legacy observers and continue to the scrolling parent");
	}

	void testRoutedWindowParts()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Window/TestSkin.xml");
		auto* window = context.getGui().createWidget<MyGUI::Window>(
			"InteractionWindow",
			MyGUI::IntCoord(100, 100, 200, 120),
			MyGUI::Align::Default,
			"Main");
		window->setSnap(false);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(120, 110, 0);
		auto* caption = input.getMouseFocusWidget();
		require(caption != nullptr && caption != window, "Fixture must pick the caption skin part");
		caption->eventMouseButtonPressedRouted +=
			MyGUI::newDelegate([](MyGUI::Widget*, MyGUI::MouseButtonEvent& event) { event.setHandled(); }, 1);
		unittest::dragFromTo({120, 110}, {150, 150});
		require(
			window->getPosition() == MyGUI::IntPoint(100, 100),
			"Caption input can be consumed before moving the window");
		caption->eventMouseButtonPressedRouted.clear();
		bool blockPress = true;
		int presses = 0;
		int releases = 0;
		int drags = 0;
		window->eventMouseButtonPressedRouted += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::MouseButtonEvent& event)
			{
				if (event.getTarget() == caption)
				{
					++presses;
					if (blockPress)
						event.setHandled();
				}
			},
			3);
		window->eventMouseButtonReleasedRouted += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::MouseButtonEvent& event)
			{
				if (event.getTarget() == caption)
					++releases;
			},
			3);
		window->eventMouseDragRouted += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::MouseButtonEvent& event)
			{
				if (event.getTarget() == caption)
					++drags;
			},
			3);
		unittest::dragFromTo({120, 110}, {150, 150});
		require(
			window->getPosition() == MyGUI::IntPoint(100, 100),
			"Window typed subscribers must be able to prevent the caption's press action");
		require(
			presses > 0 && releases > 0 && drags > 0,
			"Window typed subscribers must run before all three typed action hooks");
		blockPress = false;
		unittest::dragFromTo({120, 110}, {150, 150});
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140),
			"Unconsumed caption input must move the window once");
		unittest::clearRouted(window);
		auto* content =
			window->createWidget<MyGUI::Button>("Default", MyGUI::IntCoord(10, 30, 80, 30), MyGUI::Align::Default);
		content->setUserString("Action", "Move");
		unittest::dragFromTo({150, 180}, {180, 210});
		require(
			window->getPosition() == MyGUI::IntPoint(130, 140),
			"Ordinary content must not become a move handle even if it has an Action user string");
		window->changeWidgetSkin("InteractionWindow");
		unittest::dragFromTo({150, 150}, {160, 160});
		require(
			window->getPosition() == MyGUI::IntPoint(140, 150),
			"Replacing the skin must rebuild eligible action parts");
	}

}

int main()
{
	return unittest::runTests({
		{"Moving and resizing", testMovementAndResize},
		{"Rotated window drag", testRotatedWindowDrag},
		{"Drag within rotated parent", testDragWithinRotatedParent},
		{"Rotated window resize", testRotatedWindowResize},
		{"Resize notifications", testResizeNotifications},
		{"Resize within rotated parent", testResizeWithinRotatedParent},
		{"Resize callback coordinate changes", testResizeCallbackCanSetCoordinates},
		{"Rotated resize snapping", testRotatedResizeSnapping},
		{"Snapping and visibility", testSnappingAndVisibility},
		{"Coordinate constraints and size snapping", testCoordinateConstraints},
		{"Automatic opacity and focus transitions", testAutomaticAlpha},
		{"Direct routed movement and resizing", testDirectRoutedWindowActions},
		{"Move and resize action consumption", testWindowActionConsumption},
		{"Routed action parts", testRoutedWindowParts},
		{"Wheel bubbling through window parts", testWindowWheelBubbling},
		{"Interaction cleanup independent of routing", testWindowActionCleanup},
		{"Nested interaction cleanup", testNestedWindowInteraction},
	});
}
