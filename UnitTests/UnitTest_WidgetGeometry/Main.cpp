/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI.h"
#include "TestRunner.h"
#include "TestSupport.h"

namespace
{

	using unittest::require;

	struct Fixture
	{
		Fixture()
		{
			MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
			MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
			unittest::loadResources("UnitTest_WidgetGeometry/TestSkin.xml");
		}

		unittest::TestContext context;
	};

	struct CoordObserver
	{
		int count{0};
		MyGUI::IntPoint position;
		MyGUI::IntCoord coord;
		MyGUI::Widget* parent{nullptr};
		MyGUI::IntCoord parentCoord;

		void notify(MyGUI::Widget* _sender)
		{
			++count;
			position = _sender->getAbsolutePosition();
			coord = _sender->getAbsoluteCoord();
			parent = _sender->getParent();
			if (parent != nullptr)
				parentCoord = parent->getCoord();
		}
	};

	struct ClickObserver
	{
		void clicked(MyGUI::Widget*)
		{
			++count;
		}
		int count{0};
	};

	void testAbsoluteCoordEvent()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(10, 20, 100, 100), MyGUI::Align::Default);
		auto* popup = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Popup,
			"Default",
			MyGUI::IntCoord(400, 400, 100, 100),
			MyGUI::Align::Default,
			"Popup");
		CoordObserver parentObserver;
		CoordObserver localObserver;
		CoordObserver childObserver;
		CoordObserver popupObserver;
		parent->eventChangeAbsoluteCoord += MyGUI::newDelegate(&parentObserver, &CoordObserver::notify);
		parent->eventChangeCoord += MyGUI::newDelegate(&localObserver, &CoordObserver::notify);
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&childObserver, &CoordObserver::notify);
		popup->eventChangeAbsoluteCoord += MyGUI::newDelegate(&popupObserver, &CoordObserver::notify);

		parent->setPosition(150, 200);
		require(parentObserver.count == 1, "Direct movement must emit an absolute position event");
		require(localObserver.count == 1, "Direct movement must emit a local coordinate event");
		require(childObserver.count == 1, "Ancestor movement must notify descendants");
		require(childObserver.position == MyGUI::IntPoint(160, 220), "The event must expose the updated position");
		require(popupObserver.count == 0, "Popups must not inherit ancestor movement");
		require(popup->getAbsolutePosition() == MyGUI::IntPoint(400, 400), "Popup coordinates must stay absolute");

		parent->setPosition(parent->getPosition());
		require(localObserver.count == 1, "Unchanged position must not emit a local coordinate event");
		parent->setCoord(parent->getCoord());
		parent->setSize(parent->getSize());
		require(parentObserver.count == 1, "Unchanged coordinates must not emit events");
		require(localObserver.count == 1, "Unchanged coordinates and size must not emit local events");
		parent->setSize(350, 350);
		require(parentObserver.count == 2, "Resizing must emit an absolute coordinate event");
		require(localObserver.count == 2, "Resizing must emit one local coordinate event");
		require(
			parentObserver.coord == MyGUI::IntCoord(150, 200, 350, 350),
			"Resize callbacks must expose the new size");
		require(childObserver.count == 1, "Unchanged descendant positions must not emit events");

		child->setCoord(30, 40, 120, 120);
		require(childObserver.count == 2, "setCoord must notify absolute movement");
		require(childObserver.position == MyGUI::IntPoint(180, 240), "setCoord must expose the updated position");
		child->detachFromWidget("Main");
		require(childObserver.count == 3, "Detaching must notify absolute movement");
		require(childObserver.position == MyGUI::IntPoint(30, 40), "Detached coordinates must be absolute");
		child->attachToWidget(parent, MyGUI::WidgetStyle::Overlapped);
		require(childObserver.count == 4, "Attaching must notify absolute movement");
		require(childObserver.position == MyGUI::IntPoint(180, 240), "Attached coordinates must include the parent");

		auto* popupChild =
			popup->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(5, 10, 20, 20), MyGUI::Align::Default);
		CoordObserver popupChildObserver;
		popupChild->eventChangeAbsoluteCoord += MyGUI::newDelegate(&popupChildObserver, &CoordObserver::notify);
		popup->setPosition(450, 450);
		require(popupObserver.count == 1, "Direct popup movement must emit an event");
		require(popupChildObserver.count == 1, "Popup descendants must inherit direct popup movement");
		require(
			popupChildObserver.position == MyGUI::IntPoint(455, 460),
			"Popup descendants must use popup coordinates");
		parent->setPosition(200, 250);
		require(popupChildObserver.count == 1, "Ancestor movement must not propagate through a popup");

		gui.destroyWidget(parent);
	}

	void testClippingAfterMovement()
	{
		Fixture fixture;
		auto& layers = MyGUI::LayerManager::getInstance();
		layers.getByName("Main")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		auto& gui = fixture.context.getGui();
		auto* root = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 100, 100),
			MyGUI::Align::Default,
			"Main");
		auto* parent =
			root->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default);
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default);
		CoordObserver observer;
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);

		parent->setCoord(-50, 0, 100, 100);
		require(child->_getMarginLeft() == 50, "Movement without resizing must update descendant clipping");
		require(child->_getViewWidth() == 50, "Descendant view width must reflect ancestor clipping");
		require(observer.count == 1, "Movement with clipping must notify descendants once");
		require(
			layers.getWidgetFromPoint(110, 110) == child,
			"Picking must accept the visible part of cropped children");
		require(layers.getWidgetFromPoint(99, 110) == nullptr, "Picking must reject the part outside the root");
		require(layers.getWidgetFromPoint(150, 110) == root, "Picking must exclude the child's right boundary");

		parent->setCoord(0, 0, 100, 100);
		require(child->_getMarginLeft() == 0, "Moving back inside must clear descendant clipping");
		require(child->_getViewWidth() == 100, "Moving back inside must restore the descendant view width");
		require(observer.count == 2, "Moving back inside must notify descendants once");
		require(layers.getWidgetFromPoint(199, 199) == child, "Picking must follow restored local bounds");
		require(
			layers.getWidgetFromPoint(200, 200) == nullptr,
			"Picking must exclude the root's bottom-right boundary");
		gui.destroyWidget(root);
	}

	void testClippingWithUnchangedAlignment(MyGUI::Align _align)
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto& layers = MyGUI::LayerManager::getInstance();
		layers.getByName("Main")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 101, 100}, MyGUI::Align::Default, "Main");
		// Both (101 - 200) / 2 and (102 - 200) / 2 truncate to -49.
		// Center alignment therefore also exercises an unchanged setCoord call.
		const MyGUI::IntCoord childCoord(-49, 0, 200, 40);
		auto* child = parent->createWidget<MyGUI::Widget>("Default", childCoord, _align);
		auto* grandchild = child->createWidget<MyGUI::Widget>("Default", {0, 0, 200, 40}, MyGUI::Align::Default);
		CoordObserver localObserver;
		CoordObserver absoluteObserver;
		child->eventChangeCoord += MyGUI::newDelegate(&localObserver, &CoordObserver::notify);
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&absoluteObserver, &CoordObserver::notify);

		for (int width : {102, 101})
		{
			parent->setSize(width, 100);
			require(child->getCoord() == childCoord, "Alignment must leave the child's coordinates unchanged");
			for (auto* widget : {child, grandchild})
			{
				require(widget->_getMarginLeft() == 49, "The left clipping margin must remain unchanged");
				require(widget->_getViewWidth() == width, "Parent resizing must refresh descendant clipping");
			}
			require(
				layers.getWidgetFromPoint(100 + width - 1, 110) == grandchild,
				"Picking must include the newly exposed rightmost pixel");
			require(
				layers.getWidgetFromPoint(100 + width, 110) == nullptr,
				"Picking must exclude the parent's right boundary");
		}
		require(localObserver.count == 0, "A clipping-only change must not emit local coordinate events");
		require(absoluteObserver.count == 0, "A clipping-only change must not emit absolute coordinate events");
		gui.destroyWidget(parent);
	}

	void testClippingWithRejectedResize()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		const MyGUI::Align alignments[] = {
			MyGUI::Align::HStretch | MyGUI::Align::Top,
			MyGUI::Align::HStretch | MyGUI::Align::Bottom};
		for (auto align : alignments)
		{
			auto* parent = gui.createWidget<MyGUI::Widget>("Default", {0, 0, 101, 100}, MyGUI::Align::Default, "Main");
			const MyGUI::IntCoord childCoord(-49, 0, 200, 40);
			auto* window = parent->createWidget<MyGUI::Window>("Default", childCoord, align);
			window->setMinSize(200, 40);
			window->setMaxSize(200, 40);
			auto* child = window->createWidget<MyGUI::Widget>("Default", {0, 0, 200, 40}, MyGUI::Align::Default);

			for (int width : {102, 100})
			{
				parent->setSize(width, 100);
				require(window->getCoord() == childCoord, "Window constraints must reject the aligned resize");
				require(window->_getViewWidth() == width, "A rejected resize must still refresh clipping");
				require(child->_getViewWidth() == width, "Clipping must propagate through a constrained window");
			}
			gui.destroyWidget(parent);
		}
	}

	void testClippingAfterRotationReparenting(bool _fromRotated, bool _detach)
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* plain = gui.createWidget<MyGUI::Widget>("Default", {0, 0, 200, 200}, MyGUI::Align::Default, "Main");
		auto* rotated = gui.createWidget<MyGUI::Widget>("Default", {300, 0, 200, 200}, MyGUI::Align::Default, "Main");
		rotated->setRotation(0.5f);
		auto* source = _fromRotated ? rotated : plain;
		auto* destination = _fromRotated ? plain : rotated;
		// The container stays unclipped; only its descendant needs a new clipping state.
		auto* container = source->createWidget<MyGUI::Widget>("Default", {0, 0, 100, 100}, MyGUI::Align::Default);
		const MyGUI::IntCoord childCoord(80, 0, 40, 40);
		auto* child = container->createWidget<MyGUI::Widget>("Default", childCoord, MyGUI::Align::Default);
		require(child->_getMarginRight() == (_fromRotated ? 0 : 20), "Initial clipping must match rotation state");

		if (_detach)
			container->detachFromWidget("Main");
		else
			container->attachToWidget(destination);

		const bool rotatedAfter = !_detach && !_fromRotated;
		require(child->getCoord() == childCoord, "Reparenting must preserve the child's local coordinates");
		require(child->_hasRotation() == rotatedAfter, "Reparenting must update inherited rotation");
		require(container->_getViewWidth() == 100, "The container must remain unclipped");
		require(
			child->_getMarginRight() == (rotatedAfter ? 0 : 20),
			"Changing inherited rotation must refresh descendant clipping even when the container is unclipped");
		if (_detach)
			gui.destroyWidget(container);
		gui.destroyWidget(plain);
		gui.destroyWidget(rotated);
	}

	void testResizeNotifications()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(10, 20, 100, 100), MyGUI::Align::Stretch);
		CoordObserver parentObserver;
		CoordObserver childObserver;
		parent->eventChangeAbsoluteCoord += MyGUI::newDelegate(&parentObserver, &CoordObserver::notify);
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&childObserver, &CoordObserver::notify);

		parent->setSize(350, 400);
		require(
			parentObserver.count == 1 && parentObserver.coord == MyGUI::IntCoord(100, 100, 350, 400),
			"Resizing without movement must emit one event with the new absolute coordinates");
		require(
			childObserver.count == 1 && childObserver.coord == MyGUI::IntCoord(110, 120, 150, 200),
			"Stretching without movement must emit one event with the child's new size");

		parent->setCoord(150, 200, 400, 450);
		require(
			parentObserver.count == 2 && parentObserver.coord == MyGUI::IntCoord(150, 200, 400, 450),
			"Moving and resizing must produce one combined coordinate event");
		require(
			childObserver.count == 2 && childObserver.coord == MyGUI::IntCoord(160, 220, 200, 250),
			"Inherited movement and stretching must produce one combined coordinate event");

		parent->setCoord(parent->getCoord());
		parent->setSize(parent->getSize());
		require(
			parentObserver.count == 2 && childObserver.count == 2,
			"Repeating the same geometry must not emit absolute coordinate events");
		gui.destroyWidget(parent);
	}

	void testAbsoluteCoordAlignment()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* child = parent->createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(200, 200, 100, 100),
			MyGUI::Align::Right | MyGUI::Align::Bottom);
		auto* grandchild =
			child->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(5, 5, 20, 20), MyGUI::Align::Default);
		CoordObserver observer;
		CoordObserver grandchildObserver;
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);
		grandchild->eventChangeAbsoluteCoord += MyGUI::newDelegate(&grandchildObserver, &CoordObserver::notify);

		parent->setPosition(150, 200);
		require(observer.count == 1, "Moving a parent must notify its child once");
		require(observer.parentCoord == parent->getCoord(), "Callbacks must see the final parent coordinates");

		parent->setCoord(200, 250, 350, 350);
		require(observer.count == 2, "Movement and alignment must emit one final position change");
		require(observer.position == MyGUI::IntPoint(450, 500), "Callbacks must see the final aligned position");
		require(observer.parentCoord == parent->getCoord(), "Callbacks must see the final parent size");
		require(grandchildObserver.count == 2, "Descendants must receive one final position change");

		parent->setCoord(250, 300, 300, 300);
		require(observer.count == 2, "Movement cancelled by alignment must not emit an event");
		require(grandchildObserver.count == 2, "Cancelled movement must not notify descendants");

		parent->setSize(400, 400);
		require(observer.count == 3, "Alignment caused by resizing must notify movement");
		require(observer.position == MyGUI::IntPoint(550, 600), "Resizing must expose the final aligned position");

		gui.destroyWidget(parent);
	}

	void testAlignmentModes()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		struct AlignmentCase
		{
			MyGUI::Align align;
			MyGUI::IntCoord childCoord;
			MyGUI::IntPoint grandchildPosition;
		};
		const AlignmentCase cases[] = {
			{MyGUI::Align::Default, {20, 30, 100, 100}, {175, 235}},
			{MyGUI::Align::Right | MyGUI::Align::Bottom, {120, 230, 100, 100}, {275, 435}},
			{MyGUI::Align::Stretch, {20, 30, 200, 300}, {275, 435}},
			{MyGUI::Align::Right | MyGUI::Align::VStretch, {120, 30, 100, 300}, {275, 435}},
			{MyGUI::Align::Center, {150, 200, 100, 100}, {305, 405}}};

		for (const auto& test : cases)
		{
			auto* parent = gui.createWidget<MyGUI::Widget>(
				"Default",
				MyGUI::IntCoord(100, 100, 300, 300),
				MyGUI::Align::Default,
				"Main");
			auto* child = parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(20, 30, 100, 100), test.align);
			auto* grandchild = child->createWidget<MyGUI::Widget>(
				"Default",
				MyGUI::IntCoord(5, 5, 20, 20),
				MyGUI::Align::Right | MyGUI::Align::Bottom);
			CoordObserver observer;
			CoordObserver grandchildObserver;
			child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);
			grandchild->eventChangeAbsoluteCoord += MyGUI::newDelegate(&grandchildObserver, &CoordObserver::notify);

			parent->setCoord(150, 200, 400, 500);
			require(child->getCoord() == test.childCoord, "Each alignment mode must retain its layout behavior");
			require(observer.count == 1, "Each alignment mode must notify only the final position");
			require(
				observer.position == test.childCoord.point() + MyGUI::IntPoint(150, 200),
				"Alignment callbacks must see the updated absolute position");
			require(grandchildObserver.count == 1, "Nested alignment must not produce intermediate notifications");
			require(
				grandchildObserver.position == test.grandchildPosition,
				"Nested alignment must combine inherited movement with the final local position");

			parent->setCoord(parent->getCoord());
			require(
				observer.count == 1 && grandchildObserver.count == 1,
				"Repeating the same geometry must not produce position events");
			gui.destroyWidget(parent);
		}
	}

	void testClampedAlignment()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		// Window rejects unchanged geometry before calling Widget's setters. Exercise both virtual setter paths.
		const MyGUI::Align alignments[] = {MyGUI::Align::Stretch, MyGUI::Align::HCenter | MyGUI::Align::VStretch};
		for (const auto align : alignments)
		{
			auto* parent = gui.createWidget<MyGUI::Widget>(
				"Default",
				MyGUI::IntCoord(100, 100, 300, 300),
				MyGUI::Align::Default,
				"Main");
			auto* window = parent->createWidget<MyGUI::Window>("Default", MyGUI::IntCoord(100, 40, 100, 100), align);
			window->setMinSize(100, 100);
			window->setMaxSize(100, 100);
			auto* child =
				window->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(5, 5, 20, 20), MyGUI::Align::Default);
			CoordObserver observer;
			CoordObserver childObserver;
			window->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);
			child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&childObserver, &CoordObserver::notify);

			parent->setCoord(150, 200, 300, 350);
			require(
				window->getCoord() == MyGUI::IntCoord(100, 40, 100, 100),
				"Window size constraints must be honored");
			require(
				observer.count == 1 && observer.position == MyGUI::IntPoint(250, 240),
				"A rejected resize must still inherit ancestor movement once");
			require(
				childObserver.count == 1 && childObserver.position == MyGUI::IntPoint(255, 245),
				"Descendants of a constrained window must inherit movement");

			parent->setSize(300, 400);
			require(
				observer.count == 1 && childObserver.count == 1,
				"A rejected resize without ancestor movement must not notify positions");
			gui.destroyWidget(parent);
		}
	}

	void testSkinAlignment()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"TestGeometryParent",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* client = parent->getClientWidget();
		require(client != nullptr, "The geometry test skin must provide a client widget");
		auto* child = parent->createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(180, 180, 100, 100),
			MyGUI::Align::Right | MyGUI::Align::Bottom);
		CoordObserver observer;
		CoordObserver clientObserver;
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);
		client->eventChangeAbsoluteCoord += MyGUI::newDelegate(&clientObserver, &CoordObserver::notify);

		parent->setCoord(150, 200, 350, 350);
		require(
			client->getCoord() == MyGUI::IntCoord(10, 10, 330, 330),
			"The skin client must stretch with its parent");
		require(
			clientObserver.count == 1 && clientObserver.position == MyGUI::IntPoint(160, 210),
			"Skin children must inherit movement once");
		require(
			observer.count == 1 && observer.position == MyGUI::IntPoint(390, 440),
			"Alignment through a skin client must notify only the final position");

		parent->setCoord(200, 250, 300, 300);
		require(clientObserver.count == 2, "The skin client must continue following ancestor movement");
		require(observer.count == 1, "Alignment through a skin client must suppress cancelled movement");
		require(child->getAbsolutePosition() == MyGUI::IntPoint(390, 440), "Cancelled movement must preserve position");
		gui.destroyWidget(parent);
	}

	void testAbsoluteCoordReparenting()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* otherParent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(500, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* equalParent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(500, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(20, 30, 100, 100), MyGUI::Align::Default);
		auto* grandchild =
			child->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(5, 5, 20, 20), MyGUI::Align::Default);
		CoordObserver observer;
		CoordObserver grandchildObserver;
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&observer, &CoordObserver::notify);
		grandchild->eventChangeAbsoluteCoord += MyGUI::newDelegate(&grandchildObserver, &CoordObserver::notify);

		child->attachToWidget(parent);
		child->setWidgetStyle(MyGUI::WidgetStyle::Overlapped);
		require(observer.count == 0, "Reattaching without moving must not emit events");
		require(grandchildObserver.count == 0, "Reattaching must not expose temporary descendant positions");

		child->attachToWidget(otherParent);
		require(observer.count == 1, "Reparenting must notify the final position once");
		require(observer.parent == otherParent, "Reparenting callbacks must see the final parent");
		require(observer.position == MyGUI::IntPoint(520, 130), "Reparenting must use the final parent's position");
		require(grandchildObserver.count == 1, "Reparenting must notify descendants once");
		require(grandchildObserver.position == MyGUI::IntPoint(525, 135), "Descendants must use the final position");

		child->attachToWidget(equalParent);
		require(child->getParent() == equalParent, "Reparenting must update the hierarchy even without movement");
		require(observer.count == 1, "Parents at the same position must not cause movement events");
		require(grandchildObserver.count == 1, "Unchanged descendant positions must not emit events");

		child->setWidgetStyle(MyGUI::WidgetStyle::Popup, "Popup");
		require(observer.count == 2, "Converting to a popup must notify the final position once");
		require(observer.position == MyGUI::IntPoint(20, 30), "Popup coordinates must become absolute");
		require(child->getLayer()->getName() == "Popup", "Popup conversion must attach the requested layer");
		child->attachToWidget(parent, MyGUI::WidgetStyle::Popup);
		require(observer.count == 2, "Reparenting a popup must preserve its absolute position");
		require(child->getLayer()->getName() == "Popup", "Popup reparenting must preserve the existing layer");

		child->setWidgetStyle(MyGUI::WidgetStyle::Child);
		require(observer.count == 3, "Converting from a popup must notify the final position once");
		require(observer.position == MyGUI::IntPoint(120, 130), "Child coordinates must include the parent");
		child->setWidgetStyle(MyGUI::WidgetStyle::Overlapped);
		require(observer.count == 3, "Switching between Child and Overlapped must preserve position");

		child->detachFromWidget("Main");
		require(observer.count == 4, "Explicit detachment must still notify movement");
		require(observer.parent == nullptr, "Detachment callbacks must see the detached hierarchy");
		require(observer.position == MyGUI::IntPoint(20, 30), "Detached coordinates must become absolute");
		require(child->getLayer()->getName() == "Main", "Detachment must attach the requested layer");
		child->attachToWidget(parent);
		require(observer.count == 5, "Attaching a root widget must notify movement");
		require(observer.position == MyGUI::IntPoint(120, 130), "Attaching must restore parent-relative coordinates");
		require(grandchildObserver.count == 5, "Descendants must follow each final position change once");

		gui.destroyWidget(equalParent);
		gui.destroyWidget(otherParent);
		gui.destroyWidget(parent);
	}

	void expectLocalPoint(MyGUI::Widget* _widget, const MyGUI::FloatPoint& _local, const MyGUI::FloatPoint& _layer)
	{
		const auto transformed = _widget->localToLayer(_local);
		require(
			std::abs(transformed.left - _layer.left) < 0.01f && std::abs(transformed.top - _layer.top) < 0.01f,
			"Local points must map to displayed layer coordinates");
		MyGUI::FloatPoint points[] = {_local, {0, 0}, {17.5f, -9.25f}};
		const MyGUI::FloatPoint expected[] = {
			transformed,
			_widget->localToLayer(points[1]),
			_widget->localToLayer(points[2])};
		_widget->localToLayer(points, 3);
		for (size_t i = 0; i < 3; ++i)
			require(points[i] == expected[i], "Bulk conversion must exactly match individual point conversion");
		_widget->localToLayer(nullptr, 0);
		const auto restored = _widget->layerToLocal(_layer);
		require(
			std::abs(restored.left - _local.left) < 0.01f && std::abs(restored.top - _local.top) < 0.01f,
			"Layer points must map back to widget-local coordinates");
	}

	void expectTransformedPoint(MyGUI::Widget* _widget, const MyGUI::FloatPoint& _expected)
	{
		const MyGUI::FloatPoint point(150, 160);
		// Exercise repeated reads as well as the first read after each change.
		for (int i = 0; i < 2; ++i)
		{
			expectLocalPoint(
				_widget,
				{point.left - _widget->getAbsoluteLeft(), point.top - _widget->getAbsoluteTop()},
				_expected);
			const auto transformed = _widget->rotatePoint(point);
			require(
				std::abs(transformed.left - _expected.left) < 0.01f &&
					std::abs(transformed.top - _expected.top) < 0.01f,
				"World transform must reflect the current hierarchy and pivot");
			const auto restored = _widget->unrotatePoint(_expected);
			require(
				std::abs(restored.left - point.left) < 0.01f && std::abs(restored.top - point.top) < 0.01f,
				"Inverse world transform must reflect the current hierarchy and pivot");
		}
	}

	void testWorldRotationCacheInvalidation()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent =
			gui.createWidget<MyGUI::Widget>("TestGeometryParent", {100, 100, 200, 200}, MyGUI::Align::Default, "Main");
		auto* client = parent->getClientWidget();
		require(client != nullptr, "Test skin must provide a client widget");
		auto* child = parent->createWidget<MyGUI::Widget>("Default", {20, 30, 40, 60}, MyGUI::Align::Default);
		auto* popup = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Popup,
			"Default",
			{400, 400, 100, 100},
			MyGUI::Align::Default,
			"Popup");
		auto* popupChild = popup->createWidget<MyGUI::Widget>("Default", {5, 10, 20, 20}, MyGUI::Align::Default);
		auto check = [&](const MyGUI::FloatPoint& expected, bool rotated)
		{
			for (auto* widget : {parent, client, child, popup, popupChild})
			{
				expectTransformedPoint(widget, expected);
				require(widget->_hasRotation() == rotated, "Rotation presence must follow ancestor changes");
			}
		};
		check({150, 160}, false);
		parent->setRotation(1.5707963268f);
		check({240, 150}, true);
		// Changing between nonzero angles keeps inheritance but must refresh the transform.
		parent->setRotation(3.1415926536f);
		check({250, 240}, true);
		parent->setRotation(-1.5707963268f);
		check({160, 250}, true);
		parent->setRotation(1.5707963268f);
		parent->setSize(300, 200);
		check({290, 100}, true);
		parent->setPosition(120, 130);
		check({340, 110}, true);
		parent->setRotationCenter({0, 0});
		check({90, 160}, true);
		parent->setRotation(0);
		check({150, 160}, false);
		require(
			child->getCoord() == MyGUI::IntCoord(20, 30, 40, 60) &&
				popup->getAbsolutePosition() == MyGUI::IntPoint(400, 400),
			"Transform invalidation must reach children whose layout coordinates stay unchanged");
	}

	void testWorldRotationCacheReparenting()
	{
		Fixture fixture;
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 200, 200}, MyGUI::Align::Default, "Main");
		auto* other = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 200, 200}, MyGUI::Align::Default, "Main");
		parent->setRotation(1.5707963268f);
		other->setRotation(-1.5707963268f);
		auto* child = parent->createWidget<MyGUI::Widget>("Default", {20, 30, 40, 60}, MyGUI::Align::Default);
		auto* popup = child->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Popup,
			"Default",
			{400, 400, 100, 100},
			MyGUI::Align::Default,
			"Popup");
		auto check = [&](const MyGUI::FloatPoint& expected, bool rotated)
		{
			for (auto* widget : {child, popup})
			{
				expectTransformedPoint(widget, expected);
				require(widget->_hasRotation() == rotated, "Reparenting must refresh rotation presence");
			}
		};
		check({240, 150}, true);
		child->attachToWidget(other);
		check({160, 250}, true);
		child->detachFromWidget("Main");
		check({150, 160}, false);
		child->attachToWidget(parent, MyGUI::WidgetStyle::Popup, "Popup");
		check({240, 150}, true);
		child->attachToWidget(other, MyGUI::WidgetStyle::Popup, "Popup");
		check({160, 250}, true);
		child->setWidgetStyle(MyGUI::WidgetStyle::Child);
		check({160, 250}, true);

		// Opposing rotations still require ancestor clipping even when the net angle is zero.
		child->setRotation(1.5707963268f);
		check({170, 260}, true);
		require(
			child->unrotateVector({30, 40}) == MyGUI::IntPoint(30, 40),
			"Opposing rotations must cancel when transforming displacements");
		other->setRotation(0);
		child->setRotation(0);
		check({150, 160}, false);
	}

	void testPickingEligibility()
	{
		Fixture fixture;
		auto& layers = MyGUI::LayerManager::getInstance();
		layers.getByName("Main")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		auto* parent = fixture.context.getGui()
						   .createWidget<MyGUI::Widget>("Default", {100, 100, 100, 100}, MyGUI::Align::Default, "Main");
		auto* child = parent->createWidget<MyGUI::Widget>("Default", {10, 10, 40, 40}, MyGUI::Align::Default);
		auto* grandchild = child->createWidget<MyGUI::Widget>("Default", {10, 10, 20, 20}, MyGUI::Align::Default);
		for (float angle : {0.0f, 0.5f})
		{
			parent->setRotation(angle);
			const MyGUI::FloatPoint point = grandchild->localToLayer({10, 10});
			auto pick = [&]
			{
				return layers.getWidgetFromPoint((int)point.left, (int)point.top);
			};
			require(pick() == grandchild, "Picking must reach eligible descendants");
			parent->setVisible(false);
			require(pick() == nullptr, "Hidden ancestors must exclude their descendants from picking");
			parent->setVisible(true);
			parent->setEnabled(false);
			require(pick() == nullptr, "Disabled ancestors must exclude their descendants from picking");
			parent->setEnabled(true);
			child->setNeedMouseFocus(false);
			child->setInheritsPick(false);
			require(pick() == parent, "Non-pickable children must exclude their descendants");
			child->setInheritsPick(true);
			require(pick() == grandchild, "InheritsPick must allow descendants of a non-focusable widget");
			child->setInheritsPick(false);
			child->setNeedMouseFocus(true);
		}
	}

	void testLocalCoordinatesAndPicking()
	{
		Fixture fixture;
		auto& layers = MyGUI::LayerManager::getInstance();
		layers.getByName("Main")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		layers.getByName("Popup")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 300, 300}, MyGUI::Align::Default, "Main");
		auto* child = parent->createWidget<MyGUI::Widget>("Default", {40, 50, 80, 60}, MyGUI::Align::Default);
		expectLocalPoint(parent, {30, 40}, {130, 140});
		expectLocalPoint(child, {30, 40}, {170, 190});
		require(layers.getWidgetFromPoint(170, 190) == child, "New widgets must include their layout translation");

		parent->setRotation(1.5707963268f);
		require(
			layers.getWidgetFromPoint(310, 170) == child,
			"Unrotated children must be picked in their rotated parent's local coordinates");
		child->setRotationCenter({10, 20});
		child->setRotation(-1.5707963268f);
		expectLocalPoint(child, {30, 40}, {350, 170});
		require(layers.getWidgetFromPoint(350, 170) == child, "Picking must use the child's local pivot");
		child->setWidgetStyle(MyGUI::WidgetStyle::Overlapped);
		require(layers.getWidgetFromPoint(350, 170) == child, "Overlapped children must use parent-local picking");

		auto* popup = parent->createWidget<MyGUI::Widget>(
			MyGUI::WidgetStyle::Popup,
			"Default",
			{450, 300, 80, 60},
			MyGUI::Align::Default,
			"Popup");
		auto* popupChild = popup->createWidget<MyGUI::Widget>("Default", {10, 15, 20, 20}, MyGUI::Align::Default);
		require(
			layers.getWidgetFromPoint(180, 465) == popupChild,
			"Unrotated popup entries must still apply inherited rotation before picking children");
		popup->setRotationCenter({0, 0});
		popup->setRotation(-1.5707963268f);
		expectLocalPoint(popup, {15, 20}, {215, 470});
		expectLocalPoint(popupChild, {5, 5}, {215, 470});
		require(
			layers.getWidgetFromPoint(215, 470) == popupChild,
			"Popup picking must convert layer coordinates before traversing local children outside the parent");

		parent->setPosition(120, 130);
		expectLocalPoint(child, {30, 40}, {370, 200});
		expectLocalPoint(popupChild, {5, 5}, {265, 480});
		require(
			layers.getWidgetFromPoint(265, 480) == popupChild,
			"Popup entry conversion must follow its parent's moved rotation pivot");
		require(popup->getAbsolutePosition() == MyGUI::IntPoint(450, 300), "Popup layout must remain absolute");
		popup->setPosition(460, 310);
		expectLocalPoint(popupChild, {5, 5}, {255, 490});
		require(layers.getWidgetFromPoint(255, 490) == popupChild, "Popup children must follow direct movement");
	}

	void testNestedRotationAndPicking()
	{
		Fixture fixture;
		MyGUI::LayerManager::getInstance().getByName("Main")->castType<MyGUI::OverlappedLayer>()->setPick(true);
		auto& gui = fixture.context.getGui();
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 200, 200),
			MyGUI::Align::Default,
			"Main");
		auto* child =
			parent->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(50, 50, 100, 100), MyGUI::Align::Default);
		parent->setProperty("Rotation", "0.7853981634");
		child->setProperty("Rotation", "0.7853981634");
		require(std::abs(parent->getRotation() - 0.7853981634f) < 0.0001f, "Rotation property must set radians");
		require(
			parent->getRotationCenter() == MyGUI::FloatPoint(100.0f, 100.0f),
			"Default pivot must track the widget center");
		const MyGUI::FloatPoint corner = child->rotatePoint(MyGUI::FloatPoint(240.0f, 200.0f));
		require(
			std::abs(corner.left - 200.0f) < 0.01f && std::abs(corner.top - 240.0f) < 0.01f,
			"Parent and child rotations must compose");
		const MyGUI::FloatPoint restored = child->unrotatePoint(corner);
		require(
			std::abs(restored.left - 240.0f) < 0.01f && std::abs(restored.top - 200.0f) < 0.01f,
			"Input transform must invert the displayed transform");
		require(
			child->unrotateVector(MyGUI::IntPoint(0, 40)) == MyGUI::IntPoint(40, 0),
			"Screen displacement must convert through both rotations without depending on the pivot");
		MyGUI::RenderTargetInfo renderInfo;
		renderInfo.pixScaleX = 1.0f / 800.0f;
		renderInfo.pixScaleY = 1.0f / 600.0f;
		MyGUI::Vertex vertex;
		vertex.x = 2.0f * 240.0f / 800.0f - 1.0f;
		vertex.y = 1.0f - 2.0f * 200.0f / 600.0f;
		child->_transformVertices(&vertex, 1, renderInfo);
		require(
			std::abs(vertex.x - (2.0f * 200.0f / 800.0f - 1.0f)) < 0.001f &&
				std::abs(vertex.y - (1.0f - 2.0f * 240.0f / 600.0f)) < 0.001f,
			"Rendered vertices must use the same nested rotation as picking");
		require(
			MyGUI::LayerManager::getInstance().getWidgetFromPoint(200, 240) == child,
			"Picking must follow nested rotation");
		MyGUI::InputManager::getInstance().injectMouseMove(200, 240, 0);
		require(
			MyGUI::InputManager::getInstance().getMousePositionForWidget(child) == MyGUI::IntPoint(240, 200),
			"Widget input coordinates must be inverse-rotated for text and drag handling");
		ClickObserver clicks;
		child->eventMouseButtonClick += MyGUI::newDelegate(&clicks, &ClickObserver::clicked);
		MyGUI::InputManager::getInstance().injectMousePress(200, 240, MyGUI::MouseButton::Left);
		MyGUI::InputManager::getInstance().injectMouseRelease(200, 240, MyGUI::MouseButton::Left);
		require(clicks.count == 1, "A click on the rotated child must reach that child");
		require(
			MyGUI::InputManager::getInstance().getLastPressedPositionForWidget(MyGUI::MouseButton::Left, child) ==
				MyGUI::IntPoint(240, 200),
			"Press coordinates must match the child's unrotated layout");
		require(
			MyGUI::LayerManager::getInstance().getWidgetFromPoint(100, 100) == nullptr,
			"Picking must reject an unrotated corner outside the displayed widget");

		child->setProperty("RotationCenter", "0 0");
		require(
			child->getRotationCenter() == MyGUI::FloatPoint(0.0f, 0.0f),
			"RotationCenter property must set a widget-local pivot");
		const MyGUI::FloatPoint nestedPivot = child->rotatePoint(MyGUI::FloatPoint(200.0f, 150.0f));
		require(
			std::abs(nestedPivot.left - 200.0f) < 0.01f && std::abs(nestedPivot.top - 179.2893f) < 0.01f,
			"Parent and child transforms must compose around their separate pivots");
		const MyGUI::FloatPoint beforeMove = child->rotatePoint(MyGUI::FloatPoint(240.0f, 200.0f));
		parent->setPosition(MyGUI::IntPoint(130, 120));
		const MyGUI::FloatPoint afterMove = child->rotatePoint(MyGUI::FloatPoint(270.0f, 220.0f));
		require(
			std::abs(afterMove.left - beforeMove.left - 30.0f) < 0.01f &&
				std::abs(afterMove.top - beforeMove.top - 20.0f) < 0.01f,
			"Cached transforms must follow parent movement");
		parent->setSize(MyGUI::IntSize(220, 220));
		const MyGUI::FloatPoint defaultPivot = parent->rotatePoint(MyGUI::FloatPoint(240.0f, 230.0f));
		require(
			std::abs(defaultPivot.left - 240.0f) < 0.01f && std::abs(defaultPivot.top - 230.0f) < 0.01f,
			"Cached transform must follow the default pivot when resized");
		parent->setRotationCenter(MyGUI::FloatPoint(110.0f, 110.0f));
		parent->setSize(MyGUI::IntSize(240, 240));
		require(
			parent->getRotationCenter() == MyGUI::FloatPoint(110.0f, 110.0f),
			"Explicitly setting the current default pivot must keep it fixed after resize");
		gui.destroyWidget(parent);
	}

}

int main()
{
	return unittest::runTests({
		{"Absolute coordinate events", testAbsoluteCoordEvent},
		{"Clipping after movement", testClippingAfterMovement},
		{"Clipping after unchanged aligned position",
		 []
		 {
			 testClippingWithUnchangedAlignment(MyGUI::Align::Left | MyGUI::Align::Bottom);
		 }},
		{"Clipping after unchanged aligned size",
		 []
		 {
			 testClippingWithUnchangedAlignment(MyGUI::Align::Left | MyGUI::Align::VStretch);
		 }},
		{"Clipping after unchanged aligned coordinates",
		 []
		 {
			 testClippingWithUnchangedAlignment(MyGUI::Align::HCenter | MyGUI::Align::VStretch);
		 }},
		{"Clipping after rejected aligned resize", testClippingWithRejectedResize},
		{"Clipping when attaching to a rotated parent", std::bind(testClippingAfterRotationReparenting, false, false)},
		{"Clipping when attaching to an unrotated parent",
		 std::bind(testClippingAfterRotationReparenting, true, false)},
		{"Clipping when detaching from a rotated parent", std::bind(testClippingAfterRotationReparenting, true, true)},
		{"Resize notifications", testResizeNotifications},
		{"Absolute coordinate alignment", testAbsoluteCoordAlignment},
		{"Alignment modes", testAlignmentModes},
		{"Clamped alignment", testClampedAlignment},
		{"Skin alignment", testSkinAlignment},
		{"Absolute coordinate reparenting", testAbsoluteCoordReparenting},
		{"Nested rotation and picking", testNestedRotationAndPicking},
		{"Local coordinates and picking", testLocalCoordinatesAndPicking},
		{"Picking eligibility with and without rotation", testPickingEligibility},
		{"World rotation cache invalidation", testWorldRotationCacheInvalidation},
		{"World rotation cache reparenting", testWorldRotationCacheReparenting},
	});
}
