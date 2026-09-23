#include "ProgressTestSupport.h"
#include "TestRunner.h"

namespace
{

	using unittest::require;
	template<bool Fill>
	void testRangesAndDirections()
	{
		unittest::ProgressFixture fixture(Fill);
		auto* bar = fixture.bar;
		bar->setProgressRange(100);
		bar->setProgressPosition(150);
		require(bar->getProgressPosition() == 100, "Position must clamp to the range");
		bar->setProgressRange(40);
		require(bar->getProgressPosition() == 40, "Shrinking the range must clamp existing progress");
		bar->setProgressPosition(20);
		const MyGUI::FlowDirection directions[] = {
			MyGUI::FlowDirection::LeftToRight,
			MyGUI::FlowDirection::RightToLeft,
			MyGUI::FlowDirection::TopToBottom,
			MyGUI::FlowDirection::BottomToTop};
		for (auto direction : directions)
		{
			bar->setFlowDirection(direction);
			bar->setSize(143, 63);
			fixture.checkGeometry();
			if constexpr (Fill)
			{
				auto* place = bar->trackPlace();
				require(place->getChildCount() == 1, "Filled progress must use one track");
				auto* track = place->getChildAt(0);
				const auto rect = track->getCoord();
				if (direction == MyGUI::FlowDirection::RightToLeft)
					require(rect.right() == place->getWidth(), "Reverse horizontal fill must anchor to the right");
				if (direction == MyGUI::FlowDirection::BottomToTop)
					require(rect.bottom() == place->getHeight(), "Reverse vertical fill must anchor to the bottom");
			}
		}
		bar->setProgressRange(0);
		fixture.checkGeometry();
		for (size_t i = 0; i < bar->trackPlace()->getChildCount(); ++i)
			require(!bar->trackPlace()->getChildAt(i)->getVisible(), "An empty range must hide every track");
	}

	void testAutomaticLifecycle()
	{
		unittest::ProgressFixture fixture(false);
		auto* bar = fixture.bar;
		bar->setProgressAutoTrack(true);
		bar->setProgressAutoTrack(true);
		bar->setProgressRange(10);
		bar->setProgressPosition(5);
		require(
			bar->getProgressRange() == 1000 && bar->getProgressPosition() == 0,
			"Automatic mode must ignore manual progress changes");
		MyGUI::Gui::getInstance().eventFrameStart(0.25f);
		require(bar->getProgressPosition() == 100, "Automatic progress must advance once per frame");
		for (int i = 0; i < 40; ++i)
		{
			MyGUI::Gui::getInstance().eventFrameStart(0.1f);
			fixture.checkGeometry();
		}
		bar->setProgressAutoTrack(false);
		require(
			bar->getProgressRange() == 0 && bar->getProgressPosition() == 0,
			"Leaving automatic mode must reset progress");
		bar->setProgressRange(50);
		bar->setProgressPosition(25);
		MyGUI::Gui::getInstance().eventFrameStart(1.0f);
		require(bar->getProgressPosition() == 25, "Manual progress must no longer animate");
		bar->setProgressAutoTrack(true);
		MyGUI::Gui::getInstance().destroyWidget(bar);
		MyGUI::Gui::getInstance().eventFrameStart(1.0f);
		fixture.context.renderer().render();
		require(
			fixture.context.renderer().drawnVertices == 0,
			"Destroying an animated bar must release drawing and frame callbacks");
	}

	void testLongFrame()
	{
		unittest::ProgressFixture fixture(true);
		fixture.bar->setProgressAutoTrack(true);
		MyGUI::Gui::getInstance().eventFrameStart(1.0f);
		MyGUI::Gui::getInstance().eventFrameStart(2.5f);
		require(
			fixture.bar->getProgressPosition() == 0,
			"A frame crossing the animation endpoint must restart the track");
		fixture.checkGeometry();
		MyGUI::Gui::getInstance().eventFrameStart(0.25f);
		require(fixture.bar->getProgressPosition() == 100, "Animation must continue normally after wrapping");
		fixture.checkGeometry();
	}

}

int main()
{
	return unittest::runTests({
		{"Filled ranges and directions", testRangesAndDirections<true>},
		{"Segmented ranges and directions", testRangesAndDirections<false>},
		{"Automatic lifecycle", testAutomaticLifecycle},
		{"Long frame", testLongFrame},
	});
}
