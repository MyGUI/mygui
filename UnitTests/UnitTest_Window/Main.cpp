#include "BehaviourTestSupport.h"
#include "TestRunner.h"

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
		{"Snapping and visibility", testSnappingAndVisibility},
	});
}
