#include "BehaviourTestSupport.h"
#include "MyGUI_ControllerRepeatClick.h"
#include <cmath>
#include <string>

namespace
{

	using unittest::require;

	MyGUI::Widget* createWidget(unittest::TestContext& _context)
	{
		unittest::createInputLayer();
		return _context.getGui().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 100, 80),
			MyGUI::Align::Default,
			"Main",
			"ControlledWidget");
	}

	template<typename Controller>
	Controller* createController()
	{
		auto* item = MyGUI::ControllerManager::getInstance().createItem(Controller::getClassTypeName());
		require(item != nullptr, "The built-in controller factory must be registered");
		return item->template castType<Controller>();
	}

	struct Observer
	{
		MyGUI::Widget* widget;
		MyGUI::ControllerItem* controller;
		std::string events;
		int repeats{0};

		void check(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			require(
				_widget == widget && _controller == controller,
				"Callbacks must identify their widget and controller");
		}

		void pre(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			check(_widget, _controller);
			events += 'P';
		}

		void update(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			check(_widget, _controller);
			events += 'U';
		}

		void post(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			check(_widget, _controller);
			events += 'F';
		}

		void repeat(MyGUI::Widget* _widget, MyGUI::ControllerItem* _controller)
		{
			check(_widget, _controller);
			++repeats;
		}

		void connect()
		{
			controller->eventPreAction += MyGUI::newDelegate(this, &Observer::pre);
			controller->eventUpdateAction += MyGUI::newDelegate(this, &Observer::update);
			controller->eventPostAction += MyGUI::newDelegate(this, &Observer::post);
		}
	};

	void testPosition()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		auto* controller = createController<MyGUI::ControllerPosition>();
		controller->setCoord(MyGUI::IntCoord(300, 200, 200, 120));
		controller->setTime(1);
		Observer observer{widget, controller, {}};
		observer.connect();
		MyGUI::ControllerManager::getInstance().addItem(widget, controller);
		require(observer.events == "P", "Adding a controller must prepare it immediately");
		context.getGui().eventFrameStart(0.5f);
		require(widget->getCoord() == MyGUI::IntCoord(200, 150, 150, 100), "Linear motion must reach the midpoint");
		context.getGui().eventFrameStart(2);
		require(
			widget->getCoord() == MyGUI::IntCoord(300, 200, 200, 120),
			"A long frame must clamp to the destination");
		require(observer.events == "PUUF", "Position callbacks must finish after the final update");
		widget->setPosition(20, 30);
		context.getGui().eventFrameStart(1);
		require(widget->getPosition() == MyGUI::IntPoint(20, 30), "Completed controllers must stop updating");
		require(observer.events == "PUUF", "Completion must only be reported once");
	}

	void testPositionProperties()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		for (const char* property : {"Position", "Size", "Coord"})
		{
			widget->setCoord(100, 100, 100, 80);
			auto* controller = createController<MyGUI::ControllerPosition>();
			controller->setProperty("Time", "2");
			controller->setProperty("Function", "Linear");
			const bool position = std::string_view(property) == "Position";
			const bool size = std::string_view(property) == "Size";
			controller->setProperty(property, position ? "300 200" : size ? "200 120" : "300 200 200 120");
			MyGUI::ControllerManager::getInstance().addItem(widget, controller);
			context.getGui().eventFrameStart(1);
			require(
				widget->getCoord() ==
					MyGUI::IntCoord(size ? 100 : 200, size ? 100 : 150, position ? 100 : 150, position ? 80 : 100),
				"Position and size properties must animate only the requested components at the configured speed");
			context.getGui().eventFrameStart(1);
			require(
				widget->getCoord() ==
					MyGUI::IntCoord(size ? 100 : 300, size ? 100 : 200, position ? 100 : 200, position ? 80 : 120),
				"Property-configured motion must reach its destination");
		}
	}

	void testMovementFunctions()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		for (const char* function : {"Linear", "Inertional", "Accelerated", "Slowed", "Jump"})
		{
			widget->setPosition(0, 100);
			auto* controller = createController<MyGUI::ControllerPosition>();
			controller->setPosition(MyGUI::IntPoint(200, 100));
			controller->setFunction(function);
			MyGUI::ControllerManager::getInstance().addItem(widget, controller);
			context.getGui().eventFrameStart(0.5f);
			const int left = widget->getLeft();
			const std::string_view name(function);
			if (name == "Linear" || name == "Inertional")
				require(std::abs(left - 100) <= 1, "Symmetric movement must reach the midpoint at half time");
			else if (name == "Accelerated")
				require(left > 0 && left < 100, "Accelerated movement must start slowly");
			else if (name == "Slowed")
				require(left > 100 && left < 200, "Slowed movement must start quickly");
			else
				require(left > 200, "Jump movement must overshoot the destination");
			context.getGui().eventFrameStart(0.25f);
			if (name == "Jump")
				require(widget->getLeft() > 200, "Jump movement must overshoot before returning");
			context.getGui().eventFrameStart(1);
			require(
				widget->getPosition() == MyGUI::IntPoint(200, 100),
				"Every movement function must finish at the target");
		}
	}

	void testFade()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		for (bool fadeIn : {false, true})
		{
			widget->setAlpha(fadeIn ? 0 : 1);
			auto* controller = createController<MyGUI::ControllerFadeAlpha>();
			controller->setProperty("Alpha", fadeIn ? "0.75" : "0.25");
			controller->setProperty("Coef", "0.5");
			Observer observer{widget, controller, {}};
			observer.connect();
			MyGUI::ControllerManager::getInstance().addItem(widget, controller);
			context.getGui().eventFrameStart(0.5f);
			require(
				std::abs(widget->getAlpha() - (fadeIn ? 0.25f : 0.75f)) < 0.0001f,
				"Fade speed must use the coefficient");
			require(observer.events == "PU", "An unfinished fade must report an update");
			context.getGui().eventFrameStart(4);
			require(
				std::abs(widget->getAlpha() - (fadeIn ? 0.75f : 0.25f)) < 0.0001f,
				"Fade must clamp in either direction");
			context.getGui().eventFrameStart(1);
			require(observer.events == "PUF", "A fade must complete exactly once");
		}
	}

	void testFadePreparationAndActions()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		widget->setVisible(false);
		auto* fadeIn = createController<MyGUI::ControllerFadeAlpha>();
		fadeIn->setAlpha(1);
		fadeIn->setCoef(1);
		fadeIn->setProperty("Enabled", "false");
		MyGUI::ControllerManager::getInstance().addItem(widget, fadeIn);
		require(
			widget->getVisible() && widget->getAlpha() == 0,
			"Fading in a hidden widget must reveal it at zero alpha");
		require(!widget->getEnabled(), "The enabled property must allow input to be disabled during a fade");
		context.getGui().eventFrameStart(1);
		require(widget->getAlpha() == 1, "Fade-in must reach full opacity");

		auto* fadeOut = createController<MyGUI::ControllerFadeAlpha>();
		fadeOut->setAlpha(0);
		fadeOut->eventPostAction += MyGUI::newDelegate(MyGUI::action::actionWidgetHide);
		MyGUI::ControllerManager::getInstance().addItem(widget, fadeOut);
		context.getGui().eventFrameStart(1);
		require(!widget->getVisible(), "The hide completion action must hide the widget");

		auto* show = createController<MyGUI::ControllerPosition>();
		show->eventPreAction += MyGUI::newDelegate(MyGUI::action::actionWidgetShow);
		MyGUI::ControllerManager::getInstance().addItem(widget, show);
		require(widget->getVisible(), "The show preparation action must reveal the widget");
	}

	void testRepeatClick()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		auto* controller = createController<MyGUI::ControllerRepeatClick>();
		controller->setRepeat(0.5f, 0.25f);
		Observer observer{widget, controller, {}};
		controller->eventRepeatClick += MyGUI::newDelegate(&observer, &Observer::repeat);
		auto& manager = MyGUI::ControllerManager::getInstance();
		manager.addItem(widget, controller);
		context.getGui().eventFrameStart(0.25f);
		require(observer.repeats == 0, "Repeat must wait for the initial delay");
		context.getGui().eventFrameStart(0.25f);
		require(observer.repeats == 1, "Repeat must fire at the initial delay boundary");
		context.getGui().eventFrameStart(0.75f);
		require(observer.repeats == 4, "A long frame must deliver every elapsed repeat");
		manager.removeItem(widget);
		context.getGui().eventFrameStart(1);
		require(observer.repeats == 4, "Removing a repeat controller must stop further callbacks");
		for (float step : {0.0f, -0.25f})
		{
			auto* disabled = createController<MyGUI::ControllerRepeatClick>();
			disabled->setRepeat(0.5f, step);
			observer.controller = disabled;
			disabled->eventRepeatClick += MyGUI::newDelegate(&observer, &Observer::repeat);
			manager.addItem(widget, disabled);
			context.getGui().eventFrameStart(2);
			require(observer.repeats == 4, "A nonpositive repeat step must disable callbacks");
		}
	}

	void testReplacementAndRemoval()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		auto& manager = MyGUI::ControllerManager::getInstance();
		auto* original = createController<MyGUI::ControllerPosition>();
		original->setPosition(MyGUI::IntPoint(300, 100));
		Observer observer{widget, original, {}};
		observer.connect();
		manager.addItem(widget, original);
		context.getGui().eventFrameStart(0.5f);
		auto* replacement = createController<MyGUI::ControllerPosition>();
		replacement->setPosition(MyGUI::IntPoint(0, 100));
		manager.addItem(widget, replacement);
		auto* fade = createController<MyGUI::ControllerFadeAlpha>();
		fade->setAlpha(0);
		manager.addItem(widget, fade);
		context.getGui().eventFrameStart(0.5f);
		require(widget->getLeft() == 100, "Replacement must animate from the current position");
		require(widget->getAlpha() == 0.5f, "Different controller types must run on the same widget");
		require(observer.events == "PU", "Replacement must cancel the old controller without completion");
		manager.removeItem(widget);
		manager.removeItem(widget);
		context.getGui().eventFrameStart(2);
		require(
			widget->getLeft() == 100 && widget->getAlpha() == 0.5f,
			"Removal must stop every controller on the widget");

		auto* restarted = createController<MyGUI::ControllerPosition>();
		restarted->setPosition(MyGUI::IntPoint(400, 100));
		manager.addItem(widget, restarted);
		context.getGui().eventFrameStart(1);
		require(widget->getLeft() == 400, "Adding after removal must resume frame processing");
	}

	void testWidgetDestruction()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		auto* child =
			widget->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 20, 20), MyGUI::Align::Default);
		auto* position = createController<MyGUI::ControllerPosition>();
		Observer parentObserver{widget, position, {}};
		parentObserver.connect();
		auto* repeat = createController<MyGUI::ControllerRepeatClick>();
		Observer childObserver{child, repeat, {}};
		repeat->eventRepeatClick += MyGUI::newDelegate(&childObserver, &Observer::repeat);
		auto& manager = MyGUI::ControllerManager::getInstance();
		manager.addItem(widget, position);
		manager.addItem(child, repeat);
		context.getGui().destroyWidget(widget);
		context.getGui().eventFrameStart(2);
		require(
			parentObserver.events == "P" && childObserver.repeats == 0,
			"Destroying a subtree must cancel its controllers");
	}

	void testDestroyAction()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		const std::string name = widget->getName();
		require(
			context.getGui().findWidget<MyGUI::Widget>(name, false) == widget,
			"The widget must be registered before destruction");
		auto* position = createController<MyGUI::ControllerPosition>();
		position->eventPostAction += MyGUI::newDelegate(MyGUI::action::actionWidgetDestroy);
		auto& manager = MyGUI::ControllerManager::getInstance();
		manager.addItem(widget, position);
		auto* repeat = createController<MyGUI::ControllerRepeatClick>();
		Observer observer{widget, repeat, {}};
		repeat->eventRepeatClick += MyGUI::newDelegate(&observer, &Observer::repeat);
		manager.addItem(widget, repeat);
		context.getGui().eventFrameStart(1);
		require(
			context.getGui().findWidget<MyGUI::Widget>(name, false) == nullptr,
			"The destroy action must remove the widget");
		context.getGui().eventFrameStart(1);
		require(observer.repeats == 0, "Destruction during completion must cancel other controllers before they run");
	}

	void testEdgeHide()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		const MyGUI::IntCoord starts[] =
			{{0, 100, 100, 80}, {699, 100, 100, 80}, {100, 0, 100, 80}, {100, 519, 100, 80}, {100, 100, 100, 80}};
		const MyGUI::IntPoint hidden[] = {{-85, 100}, {789, 100}, {100, -65}, {100, 589}, {100, 100}};
		for (size_t index = 0; index < 5; ++index)
		{
			widget->setCoord(starts[index]);
			auto* controller = createController<MyGUI::ControllerEdgeHide>();
			controller->setProperty("Time", "1");
			controller->setProperty("RemainPixels", "10");
			controller->setProperty("ShadowSize", "5");
			MyGUI::ControllerManager::getInstance().addItem(widget, controller);
			context.getGui().eventFrameStart(0);
			context.getGui().eventFrameStart(0.5f);
			context.getGui().eventFrameStart(2);
			require(
				widget->getPosition() == hidden[index],
				"Edge hiding must respect the visible strip and shadow on each edge");
			require(widget->getSize() == starts[index].size(), "Edge hiding must preserve widget size");
			MyGUI::ControllerManager::getInstance().removeItem(widget);
			context.getGui().eventFrameStart(0);
		}
	}

	void testEdgeFocus()
	{
		unittest::TestContext context;
		auto* widget = createWidget(context);
		widget->setPosition(0, 100);
		auto* child =
			widget->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(85, 0, 15, 20), MyGUI::Align::Default);
		child->setNeedKeyFocus(true);
		auto* controller = createController<MyGUI::ControllerEdgeHide>();
		controller->setTime(1);
		controller->setRemainPixels(10);
		MyGUI::ControllerManager::getInstance().addItem(widget, controller);
		context.getGui().eventFrameStart(0.5f);
		context.getGui().eventFrameStart(2);
		const int hiddenLeft = widget->getLeft();
		require(hiddenLeft == -90, "An unfocused edge widget must hide");
		MyGUI::InputManager::getInstance().setKeyFocusWidget(child);
		context.getGui().eventFrameStart(0.5f);
		require(
			widget->getLeft() > hiddenLeft && widget->getLeft() < 0,
			"Child key focus must move the parent back into view");
		MyGUI::InputManager::getInstance().resetKeyFocusWidget();
		context.getGui().eventFrameStart(1);
		require(widget->getLeft() == hiddenLeft, "Losing focus must resume hiding");
		MyGUI::InputManager::getInstance().injectMouseMove(5, 105, 0);
		require(
			MyGUI::InputManager::getInstance().getMouseFocusWidget() == child,
			"The exposed child must receive mouse focus");
		context.getGui().eventFrameStart(0.5f);
		require(widget->getLeft() > hiddenLeft, "Child mouse focus must also reveal the parent");
	}

}

int main()
{
	return unittest::runTests({
		{"position interpolation and callbacks", testPosition},
		{"position properties and component isolation", testPositionProperties},
		{"movement functions", testMovementFunctions},
		{"fade interpolation and completion", testFade},
		{"fade preparation and visibility actions", testFadePreparationAndActions},
		{"repeat timing and cancellation", testRepeatClick},
		{"controller replacement and removal", testReplacementAndRemoval},
		{"widget subtree destruction", testWidgetDestruction},
		{"destruction during completion", testDestroyAction},
		{"edge hiding geometry", testEdgeHide},
		{"edge hiding focus", testEdgeFocus},
	});
}
