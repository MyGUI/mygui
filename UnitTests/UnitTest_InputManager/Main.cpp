#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include <vector>

namespace
{

	using unittest::require;

	struct FocusObserver
	{
		std::vector<MyGUI::Widget*> previous;
		std::vector<MyGUI::Widget*> next;
		std::vector<bool> root;

		void gained(MyGUI::Widget*, MyGUI::Widget* _old)
		{
			previous.push_back(_old);
		}
		void lost(MyGUI::Widget*, MyGUI::Widget* _new)
		{
			next.push_back(_new);
		}
		void rootChanged(MyGUI::Widget*, bool _focus)
		{
			root.push_back(_focus);
		}
	};

	MyGUI::Widget* root(MyGUI::Gui& _gui, int _left)
	{
		return _gui
			.createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(_left, 0, 100, 100), MyGUI::Align::Default, "Main");
	}

	void click(int _left, int _top, MyGUI::MouseButton _button = MyGUI::MouseButton::Left)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMousePress(_left, _top, _button);
		input.injectMouseRelease(_left, _top, _button);
	}

	void requirePick(int _left, int _top, MyGUI::Widget* _expected, std::string_view _message)
	{
		require(MyGUI::LayerManager::getInstance().getWidgetFromPoint(_left, _top) == _expected, _message);
		auto& input = MyGUI::InputManager::getInstance();
		require(input.injectMouseMove(_left, _top, 0) == (_expected != nullptr), _message);
		require(input.getMouseFocusWidget() == _expected, _message);
	}

	void testOverlappingRootsAndLayers()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* first = root(context.getGui(), 0);
		auto* second = root(context.getGui(), 50);
		requirePick(75, 10, second, "The last created overlapping root must receive mouse focus");
		requirePick(10, 10, first, "An uncovered part of the lower root must remain pickable");
		click(10, 10);
		requirePick(75, 10, first, "Clicking an uncovered root must raise it above its sibling");
		click(125, 10);
		requirePick(75, 10, second, "Clicking the other root must reverse their picking order");

		auto& layers = MyGUI::LayerManager::getInstance();
		auto* overlay = layers.createLayerAt("Overlay", "OverlappedLayer", 1)->castType<MyGUI::OverlappedLayer>();
		overlay->setPick(true);
		auto* top = context.getGui().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(0, 0, 100, 100),
			MyGUI::Align::Default,
			"Overlay");
		requirePick(75, 10, top, "A higher layer must take precedence over lower-layer roots");
		layers.upLayerItem(first);
		requirePick(75, 10, top, "Raising a root must not move it above a higher layer");
		overlay->setPick(false);
		requirePick(75, 10, first, "A non-pickable layer must let mouse input reach the layer below");
		overlay->setPick(true);
		requirePick(75, 10, top, "Re-enabling layer picking must restore its priority");
		requirePick(700, 500, nullptr, "Empty space must not pick any widget");
	}

	void testChildDepth()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* parent = root(context.getGui(), 100);
		auto* first =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(10, 10, 60, 60), MyGUI::Align::Default);
		auto* second =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(30, 30, 60, 60), MyGUI::Align::Default);
		requirePick(140, 40, second, "The last created sibling at equal depth must win in the overlap");
		requirePick(115, 15, first, "Child picking must account for its parent's position");
		second->setDepth(1);
		requirePick(140, 40, first, "Higher depth must move a child behind its sibling");
		second->setDepth(-1);
		requirePick(140, 40, second, "Lower depth must move a child in front of its sibling");
		second->setDepth(-1);
		requirePick(140, 40, second, "Setting the same depth must preserve picking order");
		second->setDepth(0);
		requirePick(140, 40, second, "Returning to equal depth must place the changed child last");
		first->setDepth(-2);
		requirePick(140, 40, first, "Changing either sibling's depth must update picking immediately");
		requirePick(195, 95, parent, "Uncovered parent space must pick the parent");
		requirePick(200, 100, nullptr, "Picking must exclude the parent's right and bottom edges");
		requirePick(201, 101, nullptr, "Points beyond the parent's bounds must not pick it");
	}

	void testHitTestEdges()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* widget = root(context.getGui(), 0);
		widget->setCoord(10, 20, 100, 100);
		requirePick(10, 20, widget, "The top-left pixel must be pickable");
		requirePick(10, 70, widget, "The left edge must be pickable");
		requirePick(60, 20, widget, "The top edge must be pickable");
		requirePick(109, 119, widget, "The last pixel inside the bottom-right corner must be pickable");
		requirePick(110, 70, nullptr, "The right edge must not be pickable");
		requirePick(60, 120, nullptr, "The bottom edge must not be pickable");
		requirePick(110, 120, nullptr, "The bottom-right corner must not be pickable");
		requirePick(9, 70, nullptr, "Points before the left edge must not be pickable");
		requirePick(60, 19, nullptr, "Points above the top edge must not be pickable");

		widget->setSize(1, 1);
		requirePick(10, 20, widget, "A one-pixel widget must remain pickable at its origin");
		requirePick(11, 20, nullptr, "A one-pixel widget must not pick the next column");
		requirePick(10, 21, nullptr, "A one-pixel widget must not pick the next row");
		widget->setSize(0, 10);
		requirePick(10, 20, nullptr, "A zero-width widget must not be pickable");
		widget->setSize(10, 0);
		requirePick(10, 20, nullptr, "A zero-height widget must not be pickable");
	}

	void testAdjacentWidgetEdges()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* right = root(context.getGui(), 110);
		right->setCoord(110, 20, 100, 100);
		auto* below = root(context.getGui(), 10);
		below->setCoord(10, 120, 100, 100);
		// Create this root last so an inclusive edge would incorrectly steal its neighbours' input.
		auto* first = root(context.getGui(), 10);
		first->setCoord(10, 20, 100, 100);
		requirePick(109, 70, first, "The pixel before the vertical boundary must pick the left widget");
		requirePick(110, 70, right, "The vertical boundary must pick the right widget despite stacking order");
		requirePick(60, 119, first, "The pixel before the horizontal boundary must pick the upper widget");
		requirePick(60, 120, below, "The horizontal boundary must pick the lower widget despite stacking order");
	}

	void testClippedChildEdges()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* background = root(context.getGui(), 100);
		background->setCoord(100, 50, 200, 200);
		auto* parent = root(context.getGui(), 100);
		parent->setCoord(100, 50, 100, 100);
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(80, 80, 50, 50), MyGUI::Align::Default);
		requirePick(180, 130, child, "The child's top-left pixel must account for the parent's position");
		requirePick(199, 149, child, "The child's last visible pixel must remain pickable");
		requirePick(200, 140, background, "The parent's right edge must exclude its clipped child");
		requirePick(190, 150, background, "The parent's bottom edge must exclude its clipped child");
	}

	void testAncestorAvailability()
	{
		for (bool hide : {true, false})
		{
			unittest::TestContext context;
			unittest::createInputLayer();
			auto* background = root(context.getGui(), 0);
			auto* parent = root(context.getGui(), 0);
			auto* child =
				parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 80, 80), MyGUI::Align::Default);
			auto* grandchild =
				child->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 30, 30), MyGUI::Align::Default);
			auto& input = MyGUI::InputManager::getInstance();
			input.injectMousePress(10, 10, MyGUI::MouseButton::Left);
			input.setKeyFocusWidget(grandchild);
			require(input.getMouseFocusWidget() == grandchild && input.isCaptureMouse(), "Grandchild must be captured");
			if (hide)
				parent->setVisible(false);
			else
				parent->setEnabled(false);
			require(!input.isFocusMouse() && !input.isFocusKey(), "Unavailable ancestors must clear descendant focus");
			require(!input.isCaptureMouse(), "Unavailable ancestors must release descendant capture");
			require(!parent->getRootKeyFocus() && !child->getRootMouseFocus(), "Ancestor root focus must be cleared");
			require(grandchild->getVisible() && grandchild->getEnabled(), "Inherited state must preserve local flags");
			requirePick(10, 10, background, "An unavailable ancestor must exclude its entire subtree from picking");
			parent->setVisible(true);
			parent->setEnabled(true);
			requirePick(10, 10, grandchild, "Restoring the ancestor must make descendants pickable again");
			require(!input.isFocusKey(), "Restoring availability must not restore keyboard focus implicitly");
		}
	}

	void testReparentInheritedAvailability()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* available = root(context.getGui(), 0);
		auto* hidden = root(context.getGui(), 200);
		auto* disabled = root(context.getGui(), 400);
		hidden->setVisible(false);
		disabled->setEnabled(false);
		auto* child =
			available->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 80, 80), MyGUI::Align::Default);
		auto* grandchild =
			child->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 30, 30), MyGUI::Align::Default);

		child->attachToWidget(hidden);
		require(
			!child->getInheritedVisible() && !grandchild->getInheritedVisible(),
			"Reparenting under a hidden widget must update inherited visibility for the entire subtree");
		require(
			child->getInheritedEnabled() && grandchild->getInheritedEnabled(),
			"A hidden but enabled parent must not change inherited enabled state");

		child->attachToWidget(disabled);
		require(
			child->getInheritedVisible() && grandchild->getInheritedVisible(),
			"A visible parent must restore inherited visibility after reparenting");
		require(
			!child->getInheritedEnabled() && !grandchild->getInheritedEnabled(),
			"Reparenting under a disabled widget must update inherited enabled state for the entire subtree");

		child->detachFromWidget("Main");
		require(
			child->getInheritedVisible() && grandchild->getInheritedVisible() && child->getInheritedEnabled() &&
				grandchild->getInheritedEnabled(),
			"Detaching from an unavailable parent must restore inherited availability");
	}

	void testDisabledAncestorBlocksPopupPicking()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* layer = MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
		auto* popupLayer = layer->castType<MyGUI::OverlappedLayer>();
		popupLayer->setPick(true);
		auto* background = root(context.getGui(), 0);
		auto* parent = root(context.getGui(), 200);
		auto* popup = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Popup,
			"Default",
			MyGUI::IntCoord(0, 0, 80, 80),
			MyGUI::Align::Default,
			"Popup");

		parent->setEnabled(false);
		require(
			popup->getEnabled() && !popup->getInheritedEnabled(),
			"The popup must retain only its local enabled flag");
		requirePick(10, 10, background, "A popup with a disabled ancestor must not intercept mouse input");
		parent->setEnabled(true);
		requirePick(10, 10, popup, "Enabling the ancestor must make its popup pickable");
		parent->setEnabled(false);
		requirePick(10, 10, background, "Disabling the ancestor again must immediately block popup input");
	}

	void testOverlappedChildren()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* parent = root(context.getGui(), 100);
		auto* first = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Overlapped,
			"Default",
			MyGUI::IntCoord(0, 0, 60, 60),
			MyGUI::Align::Default);
		auto* second = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Overlapped,
			"Default",
			MyGUI::IntCoord(30, 0, 60, 60),
			MyGUI::Align::Default);
		requirePick(140, 10, second, "The newer overlapped child must initially receive input");
		click(110, 10);
		requirePick(140, 10, first, "Pressing an overlapped child must raise its picking order");
		click(180, 10);
		requirePick(140, 10, second, "Pressing its sibling must restore that sibling's picking priority");
	}

	void testCaptureOutsideBounds()
	{
		std::vector<MyGUI::IntPoint> drags;
		std::vector<MyGUI::IntPoint> releases;
		int clicks = 0;
		int otherReleases = 0;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* first = root(context.getGui(), 0);
		auto* second = root(context.getGui(), 200);
		first->eventMouseDrag += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int _left, int _top, MyGUI::MouseButton _button)
			{
				require(_button == MyGUI::MouseButton::Left, "Drag must report the captured button");
				drags.emplace_back(_left, _top);
			},
			1);
		first->eventMouseButtonReleased += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int _left, int _top, MyGUI::MouseButton _button)
			{
				require(_button == MyGUI::MouseButton::Left, "Release must report the captured button");
				releases.emplace_back(_left, _top);
			},
			1);
		first->eventMouseButtonClick += MyGUI::newDelegate([&](MyGUI::Widget*) { ++clicks; }, 1);
		second->eventMouseButtonReleased +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { ++otherReleases; }, 1);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMousePress(10, 20, MyGUI::MouseButton::Left);
		require(input.isCaptureMouse(), "Press must start capture");
		input.injectMouseMove(700, 500, 0);
		input.injectMouseMove(210, 30, 0);
		require(input.getMouseFocusWidget() == first, "Capture must retain focus outside the widget and over siblings");
		require(drags == std::vector<MyGUI::IntPoint>({{700, 500}, {210, 30}}), "Drag must retain layer coordinates");
		require(
			input.getLastPressedPosition(MyGUI::MouseButton::Left) == MyGUI::IntPoint(10, 20),
			"Drag must preserve the press position");
		input.injectMouseRelease(210, 30, MyGUI::MouseButton::Left);
		require(releases == std::vector<MyGUI::IntPoint>{{210, 30}}, "Release outside must reach the captured widget");
		require(clicks == 0 && otherReleases == 0, "An outside release must neither click nor release the sibling");
		require(!input.isCaptureMouse(), "Release must end capture");
		requirePick(210, 30, second, "After release, normal hit testing must resume");
	}

	void testDoubleClick()
	{
		std::vector<std::string> events;
		int otherDoubleClicks = 0;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* first = root(context.getGui(), 0);
		auto* second = root(context.getGui(), 200);
		first->eventMouseButtonClick += MyGUI::newDelegate([&](MyGUI::Widget*) { events.emplace_back("click"); }, 1);
		first->eventMouseButtonDoubleClick +=
			MyGUI::newDelegate([&](MyGUI::Widget*) { events.emplace_back("double"); }, 1);
		second->eventMouseButtonDoubleClick += MyGUI::newDelegate([&](MyGUI::Widget*) { ++otherDoubleClicks; }, 1);
		auto& input = MyGUI::InputManager::getInstance();
		input.setDoubleClickTime(0.25f);
		click(10, 10);
		require(events == std::vector<std::string>{"click"}, "The first click must not emit a double click");
		context.getGui().eventFrameStart(0.125f);
		click(10, 10);
		require(
			events == std::vector<std::string>({"click", "click", "double"}),
			"A timely second click must emit click before double click");
		events.clear();
		context.getGui().eventFrameStart(0.5f);
		click(10, 10);
		context.getGui().eventFrameStart(0.25f);
		click(10, 10);
		require(
			events == std::vector<std::string>({"click", "click"}),
			"Clicks at the time limit must remain single clicks");
		click(210, 10);
		require(otherDoubleClicks == 0, "Clicking a different widget must reset double-click detection");
		events.clear();
		click(10, 10);
		require(events == std::vector<std::string>{"click"}, "Returning to a widget must start a new click sequence");
		events.clear();
		click(10, 10, MyGUI::MouseButton::Right);
		click(10, 10, MyGUI::MouseButton::Right);
		require(events.empty(), "Right clicks must not emit left-click or double-click events");
	}

	struct KeyObserver
	{
		std::vector<MyGUI::KeyCode> pressed;
		std::vector<MyGUI::Char> text;
		std::vector<MyGUI::KeyCode> released;

		void observe(MyGUI::Widget* _widget)
		{
			_widget->eventKeyButtonPressed += MyGUI::newDelegate(
				[this](MyGUI::Widget*, MyGUI::KeyCode _key, MyGUI::Char _text)
				{
					pressed.push_back(_key);
					text.push_back(_text);
				},
				1);
			_widget->eventKeyButtonReleased +=
				MyGUI::newDelegate([this](MyGUI::Widget*, MyGUI::KeyCode _key) { released.push_back(_key); }, 1);
		}
	};

	void testKeyRepeat()
	{
		KeyObserver observer;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* widget = root(context.getGui(), 0);
		observer.observe(widget);
		auto& input = MyGUI::InputManager::getInstance();
		input.setKeyFocusWidget(widget);
		input.injectKeyPress(MyGUI::KeyCode::A, 'a');
		require(
			observer.pressed.size() == 1 && observer.released.empty(),
			"Initial press must be delivered immediately");
		context.getGui().eventFrameStart(0.3f);
		require(observer.pressed.size() == 1, "Holding a key must respect the initial repeat delay");
		context.getGui().eventFrameStart(0.2f);
		context.getGui().eventFrameStart(0.02f);
		require(observer.pressed.size() == 1, "Repeat must wait for its interval after the initial delay");
		context.getGui().eventFrameStart(0.04f);
		require(observer.pressed.size() == 2 && observer.released.size() == 1, "Repeat must emit a press/release pair");
		context.getGui().eventFrameStart(0.06f);
		require(
			observer.pressed == std::vector<MyGUI::KeyCode>(3, MyGUI::KeyCode::A),
			"Subsequent repeats must retain the held key");
		require(observer.text == std::vector<MyGUI::Char>(3, 'a'), "Repeats must retain the original character");
		require(
			observer.released == std::vector<MyGUI::KeyCode>(2, MyGUI::KeyCode::A),
			"Every repeat must release the held key");
		input.injectKeyRelease(MyGUI::KeyCode::A);
		context.getGui().eventFrameStart(1.0f);
		require(observer.pressed.size() == 3 && observer.released.size() == 3, "Physical release must stop repeats");
		for (auto modifier :
			 {MyGUI::KeyCode::LeftShift,
			  MyGUI::KeyCode::RightShift,
			  MyGUI::KeyCode::LeftControl,
			  MyGUI::KeyCode::RightControl,
			  MyGUI::KeyCode::LeftAlt,
			  MyGUI::KeyCode::RightAlt})
		{
			const auto count = observer.pressed.size();
			input.injectKeyPress(modifier);
			context.getGui().eventFrameStart(1.0f);
			context.getGui().eventFrameStart(0.1f);
			require(observer.pressed.size() == count + 1, "Modifier keys must not repeat");
			input.injectKeyRelease(modifier);
		}
	}

	void testCallbacksChangeFocus()
	{
		KeyObserver firstKeys;
		KeyObserver secondKeys;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* first = root(context.getGui(), 0);
		auto* second = root(context.getGui(), 200);
		firstKeys.observe(first);
		secondKeys.observe(second);
		auto& input = MyGUI::InputManager::getInstance();
		first->eventMouseButtonPressed += MyGUI::newDelegate(
			[&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { input.setKeyFocusWidget(second); },
			1);
		click(10, 10);
		require(input.getKeyFocusWidget() == second, "A press callback must be able to redirect keyboard focus");
		unittest::keyStroke(MyGUI::KeyCode::B, 'b');
		require(
			firstKeys.pressed.empty() && secondKeys.text == std::vector<MyGUI::Char>{'b'},
			"Keys must reach the focus selected by the callback");

		input.setKeyFocusWidget(first);
		input.injectKeyPress(MyGUI::KeyCode::A, 'a');
		first->eventKeyButtonPressed +=
			MyGUI::newDelegate([&](MyGUI::Widget*, MyGUI::KeyCode, MyGUI::Char) { input.resetKeyFocusWidget(); }, 2);
		context.getGui().eventFrameStart(0.5f);
		context.getGui().eventFrameStart(0.06f);
		require(!input.isFocusKey(), "A repeat callback must be able to drop focus");
		require(
			firstKeys.pressed.size() == 2 && firstKeys.released.empty(),
			"Dropping focus during repeat must suppress its synthetic release");
		context.getGui().eventFrameStart(0.1f);
		input.setKeyFocusWidget(second);
		context.getGui().eventFrameStart(1.0f);
		require(secondKeys.pressed.size() == 1, "A canceled repeat must not restart when another widget gains focus");
		input.injectKeyRelease(MyGUI::KeyCode::A);
	}

	void testKeyRepeatReplacement()
	{
		KeyObserver observer;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* widget = root(context.getGui(), 0);
		observer.observe(widget);
		auto& input = MyGUI::InputManager::getInstance();
		require(!input.injectKeyPress(MyGUI::KeyCode::A, 'a'), "A key press without focus must be unhandled");
		input.setKeyFocusWidget(widget);
		context.getGui().eventFrameStart(0.5f);
		context.getGui().eventFrameStart(0.1f);
		require(observer.pressed.empty(), "A key pressed without focus must not start repeating after focus is set");
		input.injectKeyRelease(MyGUI::KeyCode::A);
		observer.released.clear();
		input.injectKeyPress(MyGUI::KeyCode::A, 'a');
		context.getGui().eventFrameStart(0.5f);
		input.injectKeyPress(MyGUI::KeyCode::B, 'b');
		context.getGui().eventFrameStart(0.1f);
		require(observer.pressed.size() == 2, "A new held key must restart the initial repeat delay");
		context.getGui().eventFrameStart(0.4f);
		context.getGui().eventFrameStart(0.06f);
		require(
			observer.text == std::vector<MyGUI::Char>({'a', 'b', 'b'}),
			"Only the most recently pressed key must repeat");
		require(
			observer.released == std::vector<MyGUI::KeyCode>{MyGUI::KeyCode::B},
			"Replacement repeats must release the new key");
		input.injectKeyRelease(MyGUI::KeyCode::B);
		input.injectKeyRelease(MyGUI::KeyCode::A);
	}

	void testFocusCallbackDestroysCapturedChild()
	{
		int presses = 0;
		int focusCallbacks = 0;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* parent = root(context.getGui(), 0);
		parent->setNeedKeyFocus(true);
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 30, 30), MyGUI::Align::Default);
		child->setNeedKeyFocus(false);
		child->eventMouseButtonPressed +=
			MyGUI::newDelegate([&](MyGUI::Widget*, int, int, MyGUI::MouseButton) { ++presses; }, 1);
		parent->eventKeySetFocus += MyGUI::newDelegate(
			[&](MyGUI::Widget*, MyGUI::Widget*)
			{
				++focusCallbacks;
				context.getGui().destroyWidget(child);
			},
			1);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMousePress(10, 10, MyGUI::MouseButton::Left);
		require(
			focusCallbacks == 1 && parent->getChildCount() == 0,
			"The parent focus callback must destroy the captured child");
		require(presses == 0, "Press dispatch must stop when the focus callback destroys its target");
		require(
			!input.isFocusMouse() && !input.isCaptureMouse(),
			"Destruction during focus dispatch must clear capture and mouse focus");
		require(input.getKeyFocusWidget() == parent, "The surviving parent must retain keyboard focus");
		input.injectMouseRelease(10, 10, MyGUI::MouseButton::Left);
		requirePick(10, 10, parent, "Mouse input must recover after callback-driven destruction");
	}

	void testFocusArguments()
	{
		FocusObserver observer;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* first = root(context.getGui(), 0);
		auto* second = root(context.getGui(), 200);
		second->eventKeySetFocus += MyGUI::newDelegate(&observer, &FocusObserver::gained);
		first->eventKeyLostFocus += MyGUI::newDelegate(&observer, &FocusObserver::lost);
		auto& input = MyGUI::InputManager::getInstance();
		input.setKeyFocusWidget(first);
		input.setKeyFocusWidget(second);
		require(
			observer.previous == std::vector<MyGUI::Widget*>{first},
			"Focus gained must identify the previous widget");
		require(observer.next == std::vector<MyGUI::Widget*>{second}, "Focus lost must identify the new widget");
		input.setKeyFocusWidget(second);
		require(observer.previous.size() == 1, "Assigning the same focus must not emit another gained event");
		input.resetKeyFocusWidget();
		require(!input.isFocusKey(), "Reset must clear keyboard focus");
		input.setKeyFocusWidget(second);
		require(observer.previous.back() == nullptr, "Focus gained from no widget must report nullptr");
	}

	void testSharedAncestor()
	{
		FocusObserver observer;
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* parent = root(context.getGui(), 0);
		auto* first =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 30, 30), MyGUI::Align::Default);
		auto* second =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(40, 0, 30, 30), MyGUI::Align::Default);
		parent->eventRootKeyChangeFocus += MyGUI::newDelegate(&observer, &FocusObserver::rootChanged);
		auto& input = MyGUI::InputManager::getInstance();
		input.setKeyFocusWidget(first);
		input.setKeyFocusWidget(second);
		require(parent->getRootKeyFocus(), "Shared parent must retain root focus when switching siblings");
		require(observer.root == std::vector<bool>{true}, "Shared parent must not lose and regain root focus");
		input.resetKeyFocusWidget();
		require(observer.root == std::vector<bool>({true, false}), "Reset must clear ancestor root focus once");
	}

	void testDestroyCapturedSubtree()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* parent = root(context.getGui(), 0);
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 30, 30), MyGUI::Align::Default);
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(10, 10, 0);
		input.injectMousePress(10, 10, MyGUI::MouseButton::Left);
		input.setKeyFocusWidget(child);
		require(input.getMouseFocusWidget() == child && input.isCaptureMouse(), "Press must capture the child");
		context.getGui().destroyWidget(parent);
		require(
			input.getKeyFocusWidget() == nullptr && input.getMouseFocusWidget() == nullptr,
			"Destroying a focused subtree must clear both focus pointers");
		require(!input.isCaptureMouse(), "Destroying the captured widget must release capture");
		input.injectMouseRelease(300, 300, MyGUI::MouseButton::Left);
		unittest::keyStroke(MyGUI::KeyCode::A, 'a');
		context.getGui().eventFrameStart(0.1f);
	}

	void testModalStack()
	{
		unittest::TestContext context;
		unittest::createInputLayer();
		auto* background = root(context.getGui(), 0);
		auto* first = root(context.getGui(), 200);
		auto* second = root(context.getGui(), 400);
		auto& input = MyGUI::InputManager::getInstance();
		input.addWidgetModal(first);
		input.addWidgetModal(second);
		input.injectMouseMove(10, 10, 0);
		input.injectMousePress(10, 10, MyGUI::MouseButton::Left);
		input.injectMouseRelease(10, 10, MyGUI::MouseButton::Left);
		require(input.getMouseFocusWidget() != background, "Modal windows must block background mouse focus");
		require(input.getKeyFocusWidget() == second, "Background click must not steal modal keyboard focus");
		input.removeWidgetModal(second);
		require(
			input.isModalAny() && input.getKeyFocusWidget() == first,
			"Closing the top modal must restore the preceding modal");
		input.removeWidgetModal(first);
		require(!input.isModalAny(), "Removing the last modal must unblock the interface");
		input.injectMouseMove(10, 10, 0);
		require(input.getMouseFocusWidget() == background, "Background must become accessible again");
		input.setKeyFocusWidget(background);
		input.injectMousePress(700, 500, MyGUI::MouseButton::Left);
		input.injectMouseRelease(700, 500, MyGUI::MouseButton::Left);
		require(input.getKeyFocusWidget() == nullptr, "Outside clicks without a modal must still clear keyboard focus");
		input.addWidgetModal(second);
		context.getGui().destroyWidget(second);
		require(
			!input.isModalAny() && input.getKeyFocusWidget() == nullptr,
			"Destroying a modal must remove its input state");
	}

}

int main()
{
	return unittest::runTests({
		{"Overlapping roots and layers", testOverlappingRootsAndLayers},
		{"Child depth", testChildDepth},
		{"Hit-test edges", testHitTestEdges},
		{"Adjacent widget edges", testAdjacentWidgetEdges},
		{"Clipped child edges", testClippedChildEdges},
		{"Overlapped children", testOverlappedChildren},
		{"Ancestor availability", testAncestorAvailability},
		{"Inherited availability after reparenting", testReparentInheritedAvailability},
		{"Disabled popup ancestor", testDisabledAncestorBlocksPopupPicking},
		{"Capture outside bounds", testCaptureOutsideBounds},
		{"Double click", testDoubleClick},
		{"Key repeat", testKeyRepeat},
		{"Key repeat replacement", testKeyRepeatReplacement},
		{"Callbacks change focus", testCallbacksChangeFocus},
		{"Focus callback destroys captured child", testFocusCallbackDestroysCapturedChild},
		{"Focus event arguments", testFocusArguments},
		{"Shared ancestor focus", testSharedAncestor},
		{"Destroy captured subtree", testDestroyCapturedSubtree},
		{"Modal stack", testModalStack},
	});
}
