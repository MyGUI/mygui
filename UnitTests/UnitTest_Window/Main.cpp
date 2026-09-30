#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include <cmath>

namespace
{

	using unittest::require;
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
		{"Snapping and visibility", testSnappingAndVisibility},
	});
}
