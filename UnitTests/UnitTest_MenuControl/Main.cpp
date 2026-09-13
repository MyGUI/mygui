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

	void checkOpenSubmenu(MyGUI::MenuControl* _menu, MyGUI::MenuItem* _item)
	{
		auto* submenu = _item->getItemChild();
		const MyGUI::IntPoint expected = _menu->getVerticalAlignment()
			? MyGUI::IntPoint(_menu->getAbsoluteRect().right, _item->getAbsoluteTop())
			: MyGUI::IntPoint(_item->getAbsoluteLeft(), _menu->getAbsoluteRect().bottom);
		require(
			submenu->getVisible() && submenu->getPosition() == expected,
			"Open submenus must follow their owners immediately");
	}

	template<typename MenuType>
	void testMovement(MyGUI::Gui& _gui, bool _horizontal, unittest::CountingLayer& _popupLayer)
	{
		auto* scroll = _gui.createWidget<MyGUI::ScrollView>(
			"Default",
			MyGUI::IntCoord(100, 100, 400, 400),
			MyGUI::Align::Default,
			"Main");
		scroll->setCanvasSize(800, 800);
		auto* menu = scroll->createWidget<MenuType>("Default", MyGUI::IntCoord(), MyGUI::Align::Default);
		menu->setVerticalAlignment(!_horizontal);
		menu->addItem("First");
		auto* owner = menu->addItem("Open", MyGUI::MenuItemType::Popup);
		auto* submenu = owner->createItemChild();
		MyGUI::LayerManager::getInstance().attachToLayerNode("Popup", submenu);
		submenu->addItem("First child");
		auto* nestedOwner = submenu->addItem("Nested", MyGUI::MenuItemType::Popup);
		auto* nested = nestedOwner->createItemChild();
		MyGUI::LayerManager::getInstance().attachToLayerNode("Popup", nested);
		nested->addItem("Leaf");
		menu->setCoord(20, 100, 120, 30);
		submenu->setSize(80, 40);
		nested->setSize(50, 20);

		MyGUI::InputManager::getInstance().setKeyFocusWidget(nested->getItemAt(0));
		owner->setItemChildVisible(true);
		nestedOwner->setItemChildVisible(true);
		_gui.eventFrameStart(0.05f);
		const float alpha = submenu->getAlpha();
		auto* focus = MyGUI::InputManager::getInstance().getKeyFocusWidget();
		scroll->getClientWidget()->setPosition(-20, -30);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		require(submenu->getAlpha() == alpha, "Repositioning must not restart the fade animation");
		require(MyGUI::InputManager::getInstance().getKeyFocusWidget() == focus, "Repositioning must preserve focus");

		scroll->setPosition(120, 80);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		menu->setCoord(30, 110, 140, 40);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		owner->setPosition(_horizontal ? 30 : 0, _horizontal ? 0 : 30);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		_popupLayer.sizeQueryCount = 0;
		menu->eventChangeCoord(menu);
		owner->eventChangeCoord(owner);
		submenu->eventChangeCoord(submenu);
		require(_popupLayer.sizeQueryCount == 0, "Duplicate coordinates must not recalculate submenu placement");
		menu->insertItemAt(0, "Inserted");
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		submenu->setSize(100, 50);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		menu->changeWidgetSkin("Default");
		owner->setPosition(_horizontal ? 40 : 0, _horizontal ? 0 : 40);
		checkOpenSubmenu(menu, owner);
		checkOpenSubmenu(submenu, nestedOwner);
		menu->detachFromWidget("Main");
		checkOpenSubmenu(menu, owner);
		menu->attachToWidget(scroll);
		checkOpenSubmenu(menu, owner);

		owner->setItemChildVisible(false);
		_gui.eventFrameStart(1.0f);
		const auto hiddenPosition = submenu->getPosition();
		scroll->setPosition(140, 100);
		require(submenu->getPosition() == hiddenPosition, "Hidden submenus must not track movement");
		owner->setItemChildVisible(true);
		checkOpenSubmenu(menu, owner);
		menu->removeItemChild(owner);
		scroll->setPosition(100, 100);
		_gui.destroyWidget(scroll);
		_gui.eventFrameStart(1.0f);
	}

	void testOrientationChange(MyGUI::Gui& _gui)
	{
		auto* menu = _gui.createWidget<MyGUI::MenuBar>(
			"Default",
			MyGUI::IntCoord(100, 100, 10, 10),
			MyGUI::Align::Default,
			"Main");
		menu->setVerticalAlignment(true);
		auto* owner = menu->addItem("", MyGUI::MenuItemType::Popup);
		auto* submenu = owner->createItemChild();
		MyGUI::LayerManager::getInstance().attachToLayerNode("Popup", submenu);
		submenu->addItem("Child");
		menu->setSize(10, 10);
		owner->setItemChildVisible(true);
		const auto coord = menu->getCoord();
		checkOpenSubmenu(menu, owner);
		menu->setVerticalAlignment(false);
		require(menu->getCoord() == coord, "The square menu must keep its size when changing orientation");
		checkOpenSubmenu(menu, owner);
		_gui.destroyWidget(menu);
	}

	void checkPlacement(MyGUI::Widget* _parent, int _top, int _submenuHeight, int _expectedTop, const char* _name)
	{
		auto& gui = MyGUI::Gui::getInstance();
		const MyGUI::IntCoord coord(10, _top, 100, 150);
		auto* menu = _parent != nullptr
			? _parent->createWidget<MyGUI::MenuControl>("Default", coord, MyGUI::Align::Default)
			: gui.createWidget<MyGUI::MenuControl>("Default", coord, MyGUI::Align::Default, "Main");

		// Default menu items have a minimum height of 10 pixels without text rendering.
		for (int index = 0; index < 15; ++index)
			menu->addItem("Item", index == 13 ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
		auto* submenu = menu->createItemChildAt(13);
		MyGUI::LayerManager::getInstance().attachToLayerNode("Popup", submenu);
		submenu->addItem("Child");
		submenu->setSize(100, _submenuHeight);
		menu->setCoord(coord);

		menu->setItemChildVisibleAt(13, true);
		const MyGUI::IntPoint expected(menu->getAbsoluteRect().right, _expectedTop);
		require(submenu->getVisible(), MyGUI::utility::toString(_name, ": submenu must be visible"));
		require(
			submenu->getPosition() == expected,
			MyGUI::utility::toString(_name, ": expected submenu at ", expected, ", got ", submenu->getPosition()));
		gui.destroyWidget(menu);
	}

	void testPlacement(MyGUI::Gui& _gui)
	{
		auto* parent = _gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 200, 300, 400),
			MyGUI::Align::Default,
			"Main");
		checkPlacement(parent, 100, 200, 250, "Offset parent: align with absolute bottom");
		// The fit check must also use the absolute bottom: locally 250 - 300 is negative.
		checkPlacement(parent, 100, 300, 150, "Offset parent: absolute bottom alignment fits");
		checkPlacement(parent, 100, 100, 430, "Submenu fits without repositioning");
		checkPlacement(parent, 100, 500, 100, "Aligning above zero requires the screen-bottom clamp");
		_gui.destroyWidget(parent);

		auto* scroll = _gui.createWidget<MyGUI::ScrollView>(
			"Default",
			MyGUI::IntCoord(100, 0, 300, 600),
			MyGUI::Align::Default,
			"Main");
		scroll->setCanvasSize(300, 1200);
		// Scrolling moves the canvas to a negative coordinate relative to its viewport.
		scroll->getClientWidget()->setPosition(0, -100);
		checkPlacement(scroll, 400, 200, 250, "Negative scroll offset: align with absolute bottom");
		checkPlacement(scroll, 400, 500, 100, "Negative scroll offset: absolute bottom alignment does not fit");
		_gui.destroyWidget(scroll);

		checkPlacement(nullptr, 300, 200, 250, "Top-level menu");
	}
}

int main()
{
	try
	{
		unittest::TestContext context;
		auto& gui = context.getGui();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		auto* popupLayer = unittest::createCountingLayer("Popup", 1);
		testPlacement(gui);
		testMovement<MyGUI::MenuControl>(gui, false, *popupLayer);
		testMovement<MyGUI::MenuBar>(gui, true, *popupLayer);
		testMovement<MyGUI::PopupMenu>(gui, false, *popupLayer);
		testOrientationChange(gui);
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
	return 0;
}
