/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI.h"
#include "TestSupport.h"
#include <iostream>

namespace
{
	using unittest::require;

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

	void testAbsoluteCoordEvent(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
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
		CoordObserver childObserver;
		CoordObserver popupObserver;
		parent->eventChangeAbsoluteCoord += MyGUI::newDelegate(&parentObserver, &CoordObserver::notify);
		child->eventChangeAbsoluteCoord += MyGUI::newDelegate(&childObserver, &CoordObserver::notify);
		popup->eventChangeAbsoluteCoord += MyGUI::newDelegate(&popupObserver, &CoordObserver::notify);

		parent->setPosition(150, 200);
		require(parentObserver.count == 1, "Direct movement must emit an absolute position event");
		require(childObserver.count == 1, "Ancestor movement must notify descendants");
		require(childObserver.position == MyGUI::IntPoint(160, 220), "The event must expose the updated position");
		require(popupObserver.count == 0, "Popups must not inherit ancestor movement");
		require(popup->getAbsolutePosition() == MyGUI::IntPoint(400, 400), "Popup coordinates must stay absolute");

		parent->setPosition(parent->getPosition());
		parent->setCoord(parent->getCoord());
		require(parentObserver.count == 1, "Unchanged coordinates must not emit events");
		parent->setSize(350, 350);
		require(parentObserver.count == 2, "Resizing must emit an absolute coordinate event");
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

		_gui.destroyWidget(parent);
	}

	void testClippingAfterMovement(MyGUI::Gui& _gui)
	{
		auto* root =
			_gui.createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default, "Main");
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

		parent->setCoord(0, 0, 100, 100);
		require(child->_getMarginLeft() == 0, "Moving back inside must clear descendant clipping");
		require(child->_getViewWidth() == 100, "Moving back inside must restore the descendant view width");
		require(observer.count == 2, "Moving back inside must notify descendants once");
		_gui.destroyWidget(root);
	}

	void testResizeNotifications(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
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
		_gui.destroyWidget(parent);
	}

	void testAbsoluteCoordAlignment(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
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

		_gui.destroyWidget(parent);
	}

	void testAlignmentModes(MyGUI::Gui& _gui)
	{
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
			auto* parent = _gui.createWidget<MyGUI::Widget>(
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
			_gui.destroyWidget(parent);
		}
	}

	void testClampedAlignment(MyGUI::Gui& _gui)
	{
		// Window rejects unchanged geometry before calling Widget's setters. Exercise both virtual setter paths.
		const MyGUI::Align alignments[] = {MyGUI::Align::Stretch, MyGUI::Align::HCenter | MyGUI::Align::VStretch};
		for (const auto align : alignments)
		{
			auto* parent = _gui.createWidget<MyGUI::Widget>(
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
			_gui.destroyWidget(parent);
		}
	}

	void testSkinAlignment(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
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
		_gui.destroyWidget(parent);
	}

	void testAbsoluteCoordReparenting(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* otherParent = _gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(500, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		auto* equalParent = _gui.createWidget<MyGUI::Widget>(
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

		_gui.destroyWidget(equalParent);
		_gui.destroyWidget(otherParent);
		_gui.destroyWidget(parent);
	}

}

int main()
{
	try
	{
		unittest::TestContext context;
		auto& gui = context.getGui();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
		unittest::loadResources("UnitTest_WidgetGeometry/TestSkin.xml");
		testAbsoluteCoordEvent(gui);
		testClippingAfterMovement(gui);
		testResizeNotifications(gui);
		testAbsoluteCoordAlignment(gui);
		testAlignmentModes(gui);
		testClampedAlignment(gui);
		testSkinAlignment(gui);
		testAbsoluteCoordReparenting(gui);
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
	return 0;
}
