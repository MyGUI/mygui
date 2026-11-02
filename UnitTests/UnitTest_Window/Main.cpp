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
	});
}
