#include "BehaviourTestSupport.h"
#include <limits>

namespace scrolling
{

	using unittest::require;

	// Skin parts are protected API. Expose only named fixture controls for driving mouse input.
	class ScrollBarFixture : public MyGUI::ScrollBar
	{
		MYGUI_RTTI_DERIVED(ScrollBarFixture)
	public:
		MyGUI::Widget* skinPart(std::string_view _name)
		{
			MyGUI::Widget* result = nullptr;
			assignWidget(result, _name);
			return result;
		}
	};

	class ScrollViewFixture : public MyGUI::ScrollView
	{
		MYGUI_RTTI_DERIVED(ScrollViewFixture)
	public:
		MyGUI::Widget* skinPart(std::string_view _name)
		{
			MyGUI::Widget* result = nullptr;
			assignWidget(result, _name);
			return result;
		}
	};

}

namespace
{

	using namespace scrolling;
	using unittest::require;

	void setup()
	{
		unittest::createInputLayer();
		unittest::loadResources("UnitTest_Scrolling/TestSkin.xml");
		auto& factories = MyGUI::FactoryManager::getInstance();
		const auto& category = MyGUI::WidgetManager::getInstance().getCategoryName();
		factories.registerFactory<ScrollBarFixture>(category);
		factories.registerFactory<ScrollViewFixture>(category);
	}

	template<typename Widget>
	MyGUI::Widget* part(Widget* _widget, std::string_view _name)
	{
		auto* result = _widget->skinPart(_name);
		require(result != nullptr, "Test skin must expose the requested part");
		return result;
	}

	MyGUI::IntPoint centre(MyGUI::Widget* _widget)
	{
		const auto coord = _widget->getAbsoluteCoord();
		return MyGUI::IntPoint(coord.left + coord.width / 2, coord.top + coord.height / 2);
	}

	void press(MyGUI::Widget* _widget)
	{
		const auto point = centre(_widget);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(point.left, point.top, 0);
		input.injectMousePress(point.left, point.top, MyGUI::MouseButton::Left);
	}

	ScrollBarFixture* createBar(MyGUI::Gui& _gui, bool _vertical)
	{
		auto* bar = _gui.createWidget<ScrollBarFixture>(
			_vertical ? "BehaviourScrollV" : "BehaviourScrollH",
			_vertical ? MyGUI::IntCoord(50, 50, 20, 200) : MyGUI::IntCoord(50, 50, 200, 20),
			MyGUI::Align::Default,
			"Main");
		bar->setScrollRange(101);
		bar->setScrollPage(3);
		bar->setScrollViewPage(20);
		bar->setScrollWheelPage(7);
		bar->setTrackSize(20);
		bar->setRepeatTriggerTime(0.1f);
		bar->setRepeatStepTime(0.05f);
		return bar;
	}

	void testRanges()
	{
		unittest::TestContext context;
		setup();
		for (bool vertical : {false, true})
		{
			auto* bar = createBar(context.getGui(), vertical);
			for (size_t range : {size_t(0), size_t(1), size_t(2), size_t(101)})
			{
				bar->setScrollRange(range);
				bar->setScrollPosition(range == 0 ? 0 : range - 1);
				require(
					bar->getScrollPosition() == (range == 0 ? 0 : range - 1),
					"Valid endpoint positions must round-trip");
				bar->setScrollPosition(std::numeric_limits<size_t>::max());
				// The API specifies the valid interval, but does not promise reset versus clamping for invalid input.
				require(
					range == 0 ? bar->getScrollPosition() == 0 : bar->getScrollPosition() < range,
					"An out-of-range assignment must leave a valid position");
			}
			bar->setScrollPosition(100);
			bar->setScrollRange(2);
			require(bar->getScrollPosition() < 2, "Shrinking the range must keep the position valid");
			bar->setMinTrackSize(15);
			bar->setTrackSize(1);
			require(bar->getTrackSize() == 15, "Track size must respect the documented minimum");
			context.getGui().destroyWidget(bar);
		}
	}

	void testButtonsAndRepeat()
	{
		unittest::TestContext context;
		setup();
		for (bool vertical : {false, true})
		{
			auto* bar = createBar(context.getGui(), vertical);
			auto& input = MyGUI::InputManager::getInstance();
			for (const char* name : {"Start", "End", "FirstPart", "SecondPart"})
			{
				bar->setScrollPosition(50);
				auto* button = part(bar, name);
				press(button);
				const bool backwards = std::string_view(name) == "Start" || std::string_view(name) == "FirstPart";
				const size_t step = std::string_view(name) == "Start" || std::string_view(name) == "End" ? 3 : 20;
				require(
					bar->getScrollPosition() == (backwards ? 50 - step : 50 + step),
					"Button press must use the configured line or page step");
				input.injectMouseMove(500, 500, 0);
				input.injectMouseRelease(500, 500, MyGUI::MouseButton::Left);
				const auto released = bar->getScrollPosition();
				context.getGui().eventFrameStart(0.3f);
				require(bar->getScrollPosition() == released, "Releasing outside must stop button repeat");
			}
			bar->setScrollPosition(0);
			press(part(bar, "End"));
			context.getGui().eventFrameStart(0.2f);
			context.getGui().eventFrameStart(0.1f);
			require(bar->getScrollPosition() > 3, "Holding a repeat-enabled button must advance more than one step");
			input.injectMouseRelease(500, 500, MyGUI::MouseButton::Left);
			bar->setRepeatEnabled(false);
			bar->setScrollPosition(0);
			press(part(bar, "End"));
			context.getGui().eventFrameStart(0.3f);
			require(bar->getScrollPosition() == 3, "Disabling repeat must limit a hold to one step");
			input.injectMouseRelease(500, 500, MyGUI::MouseButton::Left);
			context.getGui().destroyWidget(bar);
		}
	}

	void testDraggingAndWheel()
	{
		unittest::TestContext context;
		setup();
		for (bool vertical : {false, true})
		{
			auto* bar = createBar(context.getGui(), vertical);
			auto& input = MyGUI::InputManager::getInstance();
			bar->setScrollPosition(50);
			press(part(bar, "Track"));
			input.injectMouseMove(500, 500, 0);
			require(bar->getScrollPosition() == 100, "Dragging beyond the end must clamp to the last position");
			input.injectMouseRelease(500, 500, MyGUI::MouseButton::Left);
			input.injectMouseMove(0, 0, 0);
			require(
				bar->getScrollPosition() == 100 && !input.isCaptureMouse(),
				"Movement after release must not continue dragging");
			press(part(bar, "Track"));
			input.injectMouseMove(0, 0, 0);
			require(bar->getScrollPosition() == 0, "Dragging before the beginning must clamp to zero");
			input.injectMouseRelease(0, 0, MyGUI::MouseButton::Left);
			bar->setScrollPosition(50);
			const auto point = centre(part(bar, "Track"));
			input.injectMouseMove(point.left, point.top, 0);
			input.injectMouseMove(point.left, point.top, -1);
			require(bar->getScrollPosition() == 57, "Mouse wheel must use the configured wheel step");
			context.getGui().destroyWidget(bar);
		}
	}

	void testViewportBoundaries()
	{
		unittest::TestContext context;
		setup();
		auto* view = context.getGui().createWidget<ScrollViewFixture>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		view->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		auto* vertical = part(view, "VScroll");
		auto* horizontal = part(view, "HScroll");
		for (const auto size : {MyGUI::IntSize(0, 0), MyGUI::IntSize(100, 100), MyGUI::IntSize(200, 200)})
		{
			view->setCanvasSize(size);
			require(
				!vertical->getVisible() && !horizontal->getVisible(),
				"Empty, smaller, and exactly fitting content must not need scrollbars");
			require(
				view->getViewCoord().size() == MyGUI::IntSize(200, 200),
				"Hidden scrollbars must return their space to the viewport");
		}
		for (const auto size : {MyGUI::IntSize(200, 201), MyGUI::IntSize(201, 200)})
		{
			view->setCanvasSize(100, 100);
			view->setCanvasSize(size);
			require(
				vertical->getVisible() && horizontal->getVisible(),
				"One scrollbar must trigger the other when its space causes overflow");
			require(
				view->getViewCoord().size() == MyGUI::IntSize(180, 180),
				"Both scrollbars must reduce the viewport once");
		}
		view->setCanvasSize(400, 400);
		view->setViewOffset(MyGUI::IntPoint(-150, -160));
		require(
			view->getViewOffset() == MyGUI::IntPoint(-150, -160),
			"Valid offsets must scroll to the requested position");
		view->setCanvasSize(100, 100);
		require(
			view->getViewOffset() == MyGUI::IntPoint(0, 0),
			"Shrinking content to fit must reset obsolete scroll offsets");
		require(!vertical->getVisible() && !horizontal->getVisible(), "Shrinking content must hide both scrollbars");
		view->setCanvasSize(400, 400);
		view->setViewOffset(MyGUI::IntPoint(-150, -160));
		view->setSize(500, 500);
		require(view->getViewOffset() == MyGUI::IntPoint(0, 0), "Enlarging the viewport to fit must reset offsets");
	}

	void testScrollbarVisibility()
	{
		unittest::TestContext context;
		setup();
		for (bool verticalFirst : {false, true})
		{
			auto* view = context.getGui().createWidget<ScrollViewFixture>(
				"BehaviourScrollView",
				MyGUI::IntCoord(0, 0, 200, 200),
				MyGUI::Align::Default,
				"Main");
			auto* vertical = part(view, "VScroll");
			auto* horizontal = part(view, "HScroll");
			const auto setFirstVisible = [&](bool visible)
			{
				if (verticalFirst)
					view->setVisibleVScroll(visible);
				else
					view->setVisibleHScroll(visible);
			};
			const auto setSecondVisible = [&](bool visible)
			{
				if (verticalFirst)
					view->setVisibleHScroll(visible);
				else
					view->setVisibleVScroll(visible);
			};
			view->setCanvasSize(400, 400);
			for (int repeat = 0; repeat < 2; ++repeat)
			{
				setFirstVisible(false);
				setFirstVisible(false);
				require(
					vertical->getVisible() == !verticalFirst && horizontal->getVisible() == verticalFirst,
					"Disabling a visible scrollbar must hide it even while content overflows");
				require(
					view->getViewCoord().size() ==
						(verticalFirst ? MyGUI::IntSize(200, 180) : MyGUI::IntSize(180, 200)),
					"Disabling a scrollbar repeatedly must return its space exactly once");
				setSecondVisible(false);
				require(!vertical->getVisible() && !horizontal->getVisible(), "Both scrollbars must stay disabled");
				require(
					view->getViewCoord().size() == MyGUI::IntSize(200, 200),
					"Disabling both scrollbars must restore the full viewport");
				setFirstVisible(true);
				setSecondVisible(true);
				require(
					vertical->getVisible() && horizontal->getVisible(),
					"Re-enabling must restore needed scrollbars");
				require(
					view->getViewCoord().size() == MyGUI::IntSize(180, 180) && vertical->getHeight() == 180 &&
						horizontal->getWidth() == 180,
					"Repeated toggles must preserve viewport and scrollbar dimensions");
			}
			view->setCanvasSize(verticalFirst ? MyGUI::IntSize(200, 201) : MyGUI::IntSize(201, 200));
			setFirstVisible(false);
			require(
				!vertical->getVisible() && !horizontal->getVisible(),
				"Disabling one scrollbar must hide the other when the restored space removes its overflow");
			setFirstVisible(true);
			require(
				vertical->getVisible() && horizontal->getVisible(),
				"Re-enabling one scrollbar must show the other when its space causes overflow");
			context.getGui().destroyWidget(view);
		}
	}

	void testContentAlignmentAndClamping()
	{
		unittest::TestContext context;
		setup();
		auto* view = context.getGui().createWidget<ScrollViewFixture>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		view->setCanvasSize(101, 81);
		view->setCanvasAlign(MyGUI::Align::Center);
		require(
			view->getViewOffset() == MyGUI::IntPoint(49, 59),
			"Small content must remain centred with integer rounding");
		view->setCanvasAlign(MyGUI::Align::Right | MyGUI::Align::Bottom);
		require(view->getViewOffset() == MyGUI::IntPoint(99, 119), "Small content must align to the right and bottom");
		view->setCanvasSize(400, 400);
		require(
			view->getViewOffset() == MyGUI::IntPoint(0, 0),
			"Growing aligned content must clamp negative scroll offsets");
		view->setViewOffset(MyGUI::IntPoint(-200, -210));
		view->setCanvasSize(300, 320);
		require(
			view->getViewOffset() == MyGUI::IntPoint(-120, -140),
			"Shrinking overflowing content must clamp both offsets to the new limits");
		require(
			part(view, "VScroll")->castType<MyGUI::ScrollBar>()->getScrollPosition() == 140 &&
				part(view, "HScroll")->castType<MyGUI::ScrollBar>()->getScrollPosition() == 120,
			"Scrollbars must follow clamped content offsets");
	}

}

int main()
{
	return unittest::runTests({
		{"Scrollbar ranges and track limits", testRanges},
		{"Line/page buttons and repeat cancellation", testButtonsAndRepeat},
		{"Dragging and wheel input", testDraggingAndWheel},
		{"Viewport boundaries and scrollbar interaction", testViewportBoundaries},
		{"Scrollbar visibility toggles", testScrollbarVisibility},
		{"Content alignment and offset clamping", testContentAlignmentAndClamping},
	});
}
