#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include "TreeControlItem.h"
#include <limits>
#include <vector>

namespace scrolling
{

	using unittest::require;

	// Skin parts are protected API. Expose only named fixture controls for driving mouse input.
	class ScrollBarFixture : public MyGUI::ScrollBar
	{
		MYGUI_RTTI_DERIVED(ScrollBarFixture)
	public:
		int wheelCalls{0};

		MyGUI::Widget* skinPart(std::string_view _name)
		{
			MyGUI::Widget* result = nullptr;
			assignWidget(result, _name);
			return result;
		}

	protected:
		void onMouseWheelRouted(MyGUI::MouseWheelEvent& _event) override
		{
			++wheelCalls;
			MyGUI::ScrollBar::onMouseWheelRouted(_event);
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
			part(bar, "End")->eventMouseButtonReleasedRouted +=
				MyGUI::newDelegate([](MyGUI::MouseButtonEvent& event) { event.setHandled(); }, 1);
			input.injectMouseRelease(500, 500, MyGUI::MouseButton::Left);
			const auto stopped = bar->getScrollPosition();
			context.getGui().eventFrameStart(0.3f);
			require(bar->getScrollPosition() == stopped, "Intercepting release must still stop button repeat");
			bar->setMoveToClick(true);
			bar->setScrollPosition(0);
			press(part(bar, "SecondPart"));
			const auto jumped = bar->getScrollPosition();
			require(jumped > 20 && jumped < 100, "Move-to-click must jump to the press rather than step one page");
			context.getGui().eventFrameStart(0.3f);
			require(bar->getScrollPosition() == jumped, "Holding a move-to-click press must not repeat");
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

	void testRoutedPartPress()
	{
		unittest::TestContext context;
		setup();
		auto& gui = context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {0, 0, 300, 300}, MyGUI::Align::Default, "Main");
		auto* bar = createBar(gui, true);
		bar->attachToWidget(parent);
		bar->setScrollPosition(50);
		auto* end = part(bar, "End");
		const auto point = centre(end);
		const auto left = MyGUI::MouseButton::Left, right = MyGUI::MouseButton::Right;
		int notifications = 0, bubbled = 0;
		bar->eventMouseButtonPressed +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { ++notifications; }, 1);
		parent->eventMouseButtonPressedRouted += MyGUI::newDelegate([&](MyGUI::MouseButtonEvent&) { ++bubbled; }, 2);

		auto pressAtPoint = [&](MyGUI::Widget* target, MyGUI::MouseButton button)
		{
			target->_riseMouseButtonPressed(point.left, point.top, button);
			target->_riseMouseButtonReleased(point.left, point.top, button);
		};
		pressAtPoint(end, left);
		require(
			notifications == 0 && bar->getScrollPosition() == 53 && bubbled == 1,
			"A left part press must step and bubble without firing the scrollbar's legacy event");
		pressAtPoint(end, right);
		require(
			notifications == 0 && bar->getScrollPosition() == 53 && bubbled == 2,
			"A right part press must bubble without stepping or firing the scrollbar's legacy event");
		pressAtPoint(bar, left);
		require(notifications == 1, "A direct scrollbar press must fire its legacy event once");
		auto* decoration = end->createWidget<MyGUI::Widget>("Default", {0, 0, 5, 5}, MyGUI::Align::Default);
		pressAtPoint(decoration, left);
		require(
			notifications == 1 && bar->getScrollPosition() == 53 && bubbled == 4,
			"A descendant of a skin part must not activate the scrollbar");

		bar->eventMouseButtonPressedRouted +=
			MyGUI::newDelegate([](MyGUI::MouseButtonEvent& event) { event.setHandled(); }, 3);
		pressAtPoint(end, left);
		auto* track = part(bar, "Track");
		press(track);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(500, 500, 0);
		input.injectMouseRelease(500, 500, left);
		require(
			bar->getScrollPosition() == 53 && bubbled == 4,
			"Intercepted presses must not step, start a thumb drag or reach the parent");
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

	void testWheelBubbling()
	{
		unittest::TestContext context;
		setup();
		auto* view = context.getGui().createWidget<ScrollViewFixture>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		view->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		view->setCanvasSize(100, 600);
		auto* panel =
			view->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 100, 600), MyGUI::Align::Default);
		auto* button =
			panel->createWidget<MyGUI::Button>("Default", MyGUI::IntCoord(0, 0, 100, 600), MyGUI::Align::Default);
		int observerCalls = 0, typedCalls = 0;
		bool blockWheel = false;
		view->eventMouseWheelRouted += MyGUI::newDelegate(
			[&](MyGUI::MouseWheelEvent& event)
			{
				require(!event.isHandled(), "The typed observer must run before the scroll view's typed wheel hook");
				if (blockWheel)
					event.setHandled();
				++observerCalls;
			},
			1);
		view->eventMouseWheelRouted += MyGUI::newDelegate(
			[&](MyGUI::MouseWheelEvent& event)
			{
				require(!event.isHandled(), "Typed observers must only receive unconsumed input");
				++typedCalls;
			},
			2);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(10, 10, 0);
		require(input.getMouseFocusWidget() == button, "The nested button must receive the input");
		input.injectMouseMove(10, 10, -1);
		require(
			view->getViewOffset().top == -50 && observerCalls == 1 && typedCalls == 1,
			"Wheel over a nested button must scroll once with multiple typed observers");
		require(input.getMouseFocusWidget() == button, "Bubbling must not change mouse focus");

		int legacyCalls = 0;
		button->eventMouseWheel += MyGUI::newDelegate([&](MyGUI::Widget*, int) { ++legacyCalls; }, 1);
		input.injectMouseMove(10, 10, -2);
		require(
			legacyCalls == 1 && view->getViewOffset().top == -100,
			"Legacy observers must not prevent scrolling the parent");
		int buttonObserverCalls = 0;
		button->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { ++buttonObserverCalls; }, 2);
		input.injectMouseMove(10, 10, -3);
		require(
			legacyCalls == 2 && buttonObserverCalls == 1 && view->getViewOffset().top == -150,
			"Legacy and typed observers must both allow parent scrolling");
		button->eventMouseWheel.clear();
		button->eventMouseWheelRouted.clear();
		button->eventMouseWheelRouted +=
			MyGUI::newDelegate([](MyGUI::MouseWheelEvent& event) { event.setHandled(); }, 3);
		input.injectMouseMove(10, 10, -4);
		require(view->getViewOffset().top == -150, "A bubbling handler must be able to consume the wheel");
		button->eventMouseWheelRouted.clear();

		view->setCanvasSize(600, 100);
		input.injectMouseMove(10, 10, -5);
		require(view->getViewOffset().left == -50, "Bubbling must also reach horizontal scroll views");
		blockWheel = true;
		input.injectMouseMove(10, 10, -6);
		require(view->getViewOffset().left == -50, "Typed consumption must prevent the scroll view's typed action");
	}

	void testWheelCallbackOrder()
	{
		std::vector<int> stages;
		unittest::TestContext context;
		setup();
		auto* bar = createBar(context.getGui(), true);
		bar->setScrollPosition(50);
		bar->eventMouseWheel += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int)
			{
				require(bar->getScrollPosition() == 50, "Legacy delegates must precede routed scrolling");
				stages.push_back(1);
			},
			1);
		bar->eventMouseWheelRouted += MyGUI::newDelegate(
			[&](MyGUI::MouseWheelEvent& event)
			{
				require(!event.isHandled(), "Typed subscribers must only receive unconsumed input");
				stages.push_back(2);
			},
			2);
		bar->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { stages.push_back(3); }, 3);
		bar->_riseMouseWheel(-1);
		require(
			stages == std::vector<int>{2, 3, 1} && bar->wheelCalls == 1 && bar->getScrollPosition() == 57,
			"Legacy and typed observers must precede default scrolling without consuming input");
		bar->eventMouseWheel.clear();
		stages.clear();
		bar->_riseMouseWheel(-1);
		require(
			stages == std::vector<int>{2, 3} && bar->wheelCalls == 2 && bar->getScrollPosition() == 64,
			"Direct scrolling must run once after typed subscribers");
		stages.clear();
		auto* child = bar->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default);
		child->_riseMouseWheel(-1);
		require(
			stages == std::vector<int>{2, 3} && bar->wheelCalls == 3 && bar->getScrollPosition() == 71,
			"Descendant scrolling must use the virtual override once after typed subscribers");
		stages.clear();
		bar->eventMouseWheelRouted += MyGUI::newDelegate([](MyGUI::MouseWheelEvent& event) { event.setHandled(); }, 4);
		bar->_riseMouseWheel(-1);
		require(
			stages == std::vector<int>{2, 3} && bar->wheelCalls == 3 && bar->getScrollPosition() == 71,
			"Typed consumption must prevent direct default scrolling");
		stages.clear();
		child->_riseMouseWheel(-1);
		require(
			stages == std::vector<int>{2, 3} && bar->wheelCalls == 3 && bar->getScrollPosition() == 71,
			"Typed consumption must skip the default scrolling hook");
		bar->eventMouseWheelRouted.clear();
		bar->eventMouseWheelRouted += MyGUI::newDelegate([&](MyGUI::MouseWheelEvent&) { stages.push_back(3); }, 3);
		bar->setScrollPosition(100);
		stages.clear();
		child->_riseMouseWheel(-1);
		require(stages == std::vector<int>{3}, "A control at its boundary must leave wheel input unhandled");
		bar->setScrollPosition(50);
		bar->setReceiveHandledEvents(true);
		bar->eventMouseWheelRouted.clear();
		int observed = 0;
		bar->eventMouseWheelRouted += MyGUI::newDelegate(
			[&](MyGUI::MouseWheelEvent& event)
			{
				require(event.isHandled(), "An opted-in scrollbar must observe consumed wheel input");
				++observed;
			},
			5);
		child->eventMouseWheelRouted +=
			MyGUI::newDelegate([](MyGUI::MouseWheelEvent& event) { event.setHandled(); }, 6);
		const auto wheelCalls = bar->wheelCalls;
		child->_riseMouseWheel(-1);
		require(
			observed == 1 && bar->wheelCalls == wheelCalls && bar->getScrollPosition() == 50,
			"Observing handled input must never re-enable a scrollbar's default wheel action");
	}

	void testNestedWheelConsumers()
	{
		unittest::TestContext context;
		setup();
		auto* outer = context.getGui().createWidget<ScrollViewFixture>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		outer->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		outer->setCanvasSize(100, 600);
		auto* inner = outer->createWidget<MyGUI::ScrollView>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 150, 150),
			MyGUI::Align::Default);
		inner->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		inner->setCanvasSize(100, 400);
		inner->createWidget<MyGUI::Button>("Default", MyGUI::IntCoord(0, 0, 100, 400), MyGUI::Align::Default);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(10, 10, 0);
		input.injectMouseMove(10, 10, -1);
		require(
			inner->getViewOffset().top == -50 && outer->getViewOffset().top == 0,
			"Only the nearest scroll view must consume a wheel event");
		inner->setViewOffset(MyGUI::IntPoint(0, -10000));
		const auto limit = inner->getViewOffset();
		input.injectMouseMove(10, 10, -2);
		require(
			inner->getViewOffset() == limit && outer->getViewOffset().top == -50,
			"A scroll view at its limit must let its parent scroll");

		context.getGui().destroyWidget(inner);
		outer->setViewOffset(MyGUI::IntPoint());
		auto* bar =
			outer->createWidget<MyGUI::ScrollBar>("Default", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default);
		bar->setScrollRange(101);
		bar->setScrollWheelPage(7);
		input.injectMouseMove(10, 10, -2);
		input.injectMouseMove(10, 10, -3);
		require(
			bar->getScrollPosition() == 7 && outer->getViewOffset().top == 0,
			"A scrollbar's virtual wheel handler must consume input before its parent");
	}

	void testWheelMovementConsumption()
	{
		unittest::TestContext context;
		setup();
		unittest::loadResources("UnitTest_ListBehaviour/TestSkin.xml");
		unittest::loadResources("UnitTest_ItemBox/TestSkin.xml");
		auto* outer = context.getGui().createWidget<ScrollViewFixture>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		outer->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		outer->setCanvasSize(100, 1000);

		for (bool vertical : {false, true})
		{
			auto* inner = outer->createWidget<ScrollViewFixture>(
				"BehaviourScrollView",
				MyGUI::IntCoord(0, 0, 150, 150),
				MyGUI::Align::Default);
			inner->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
			for (auto* target : {static_cast<MyGUI::Widget*>(inner), part(inner, "Client"), inner->getClientWidget()})
			{
				inner->setCanvasSize(vertical ? MyGUI::IntSize(100, 400) : MyGUI::IntSize(400, 100));
				inner->setViewOffset(MyGUI::IntPoint(-10000, -10000));
				const auto limit = inner->getViewOffset();
				inner->setViewOffset(limit + (vertical ? MyGUI::IntPoint(0, 10) : MyGUI::IntPoint(10, 0)));
				outer->setViewOffset(MyGUI::IntPoint());
				target->_riseMouseWheel(-1);
				require(
					inner->getViewOffset() == limit && outer->getViewOffset().top == 0,
					"ScrollView and both client layers must consume the step reaching their boundary");
				target->_riseMouseWheel(-1);
				require(outer->getViewOffset().top == -50, "ScrollView client layers must allow boundary bubbling");
				inner->setCanvasSize(100, 100);
				target->_riseMouseWheel(-1);
				require(outer->getViewOffset().top == -100, "A ScrollView whose content fits must allow bubbling");
			}
			context.getGui().destroyWidget(inner);
		}

		for (bool descendant : {false, true})
		{
			auto* list = outer->createWidget<MyGUI::ListBox>(
				"BehaviourListBox",
				MyGUI::IntCoord(0, 0, 120, 64),
				MyGUI::Align::Default);
			for (int i = 0; i < 5; ++i)
				list->addItem("row");
			list->setScrollPosition(30);
			auto target = [&]() -> MyGUI::Widget*
			{
				return descendant ? list->getWidgetByIndex(2) : list;
			};
			outer->setViewOffset(MyGUI::IntPoint());
			target()->_riseMouseWheel(-1);
			require(
				list->_getScrollPosition() == 40 && outer->getViewOffset().top == 0,
				"The step reaching the list boundary must be consumed without scrolling the parent");
			target()->_riseMouseWheel(-1);
			require(
				list->_getScrollPosition() == 40 && outer->getViewOffset().top == -50,
				"Wheel at the list boundary must bubble from both the list and its rows");
			target()->_riseMouseWheel(1);
			require(
				list->_getScrollPosition() == 20 && outer->getViewOffset().top == -50,
				"Reversing wheel direction must scroll the list again");
			list->removeAllItems();
			list->addItem("only row");
			(descendant ? list->getWidgetByIndex(0) : list)->_riseMouseWheel(-1);
			require(outer->getViewOffset().top == -100, "A list whose rows fit must allow parent scrolling");
			list->removeAllItems();
			list->_riseMouseWheel(-1);
			require(outer->getViewOffset().top == -150, "An empty list must allow parent scrolling");
			context.getGui().destroyWidget(list);

			for (bool vertical : {false, true})
			{
				auto* box = outer->createWidget<MyGUI::ItemBox>(
					"TestItemBox",
					MyGUI::IntCoord(0, 0, 120, 80),
					MyGUI::Align::Default);
				box->requestCoordItem = MyGUI::newDelegate(
					[](MyGUI::ItemBox*, MyGUI::IntCoord& coord, bool) { coord = MyGUI::IntCoord(0, 0, 40, 20); },
					1);
				box->setVerticalAlignment(vertical);
				box->setSize(120, 80);
				for (int i = 0; i < 18; ++i)
					box->addItem();
				box->setViewOffset(MyGUI::IntPoint(10000, 10000));
				const auto limit = box->getViewOffset();
				require(
					vertical ? limit.top > 0 : limit.left > 0,
					"ItemBox fixture must overflow in its scroll direction");
				box->setViewOffset(limit - (vertical ? MyGUI::IntPoint(0, 10) : MyGUI::IntPoint(20, 0)));
				auto boxTarget = [&]() -> MyGUI::Widget*
				{
					return descendant ? box->getWidgetByIndex(17) : box;
				};
				outer->setViewOffset(MyGUI::IntPoint());
				boxTarget()->_riseMouseWheel(-1);
				require(
					box->getViewOffset() == limit && outer->getViewOffset().top == 0,
					"ItemBox must consume the step that reaches its boundary");
				boxTarget()->_riseMouseWheel(-1);
				require(outer->getViewOffset().top == -50, "ItemBox boundary input must reach the outer view");
				boxTarget()->_riseMouseWheel(0);
				require(
					box->getViewOffset() == limit && outer->getViewOffset().top == -50,
					"Zero wheel delta must not scroll either control");
				box->removeAllItems();
				box->addItem();
				(descendant ? box->getWidgetByIndex(0) : box)->_riseMouseWheel(-1);
				require(outer->getViewOffset().top == -100, "ItemBox content that fits must not consume wheel input");
				context.getGui().destroyWidget(box);
			}
		}

		for (bool vertical : {false, true})
		{
			auto* bar = outer->createWidget<ScrollBarFixture>(
				vertical ? "BehaviourScrollV" : "BehaviourScrollH",
				MyGUI::IntCoord(0, 0, 100, 100),
				MyGUI::Align::Default);
			bar->setScrollRange(101);
			bar->setScrollWheelPage(7);
			for (auto* target : {static_cast<MyGUI::Widget*>(bar), part(bar, "Track"), part(bar, "Start")})
			{
				outer->setViewOffset(MyGUI::IntPoint());
				bar->setScrollPosition(99);
				target->_riseMouseWheel(-1);
				require(
					bar->getScrollPosition() == 100 && outer->getViewOffset().top == 0,
					"Scrollbar and skin input must consume the final movement");
				target->_riseMouseWheel(-1);
				require(outer->getViewOffset().top == -50, "Scrollbar skin input must bubble at its boundary");
				bar->setScrollPosition(0);
				target->_riseMouseWheel(1);
				require(outer->getViewOffset().top == 0, "Wheel at the starting boundary must bubble upward too");
			}
			context.getGui().destroyWidget(bar);
		}
	}

	void testTreeExpandWheel()
	{
		unittest::TestContext context;
		setup();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::TreeControlItem>(
			MyGUI::WidgetManager::getInstance().getCategoryName());
		auto* view = context.getGui().createWidget<MyGUI::ScrollView>(
			"BehaviourScrollView",
			MyGUI::IntCoord(0, 0, 200, 200),
			MyGUI::Align::Default,
			"Main");
		view->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		view->setCanvasSize(200, 600);
		auto* item = view->createWidget<MyGUI::TreeControlItem>(
			"WheelTreeItem",
			MyGUI::IntCoord(0, 0, 100, 20),
			MyGUI::Align::Default);
		item->getButtonExpandCollapse()->_riseMouseWheel(-1);
		require(
			view->getViewOffset().top == -50,
			"The real tree expand button must let wheel input reach its scroll ancestor");
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
		{"Routed scrollbar part presses", testRoutedPartPress},
		{"Dragging and wheel input", testDraggingAndWheel},
		{"Wheel bubbling through scroll view children", testWheelBubbling},
		{"Nested wheel consumers", testNestedWheelConsumers},
		{"Tree expand-button wheel routing", testTreeExpandWheel},
		{"Wheel consumption follows movement", testWheelMovementConsumption},
		{"Wheel callback order and subclass compatibility", testWheelCallbackOrder},
		{"Viewport boundaries and scrollbar interaction", testViewportBoundaries},
		{"Scrollbar visibility toggles", testScrollbarVisibility},
		{"Content alignment and offset clamping", testContentAlignmentAndClamping},
	});
}
