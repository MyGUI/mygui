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

	MyGUI::ListBox* openList(MyGUI::ComboBox* _combo)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.setKeyFocusWidget(_combo);
		input.injectKeyPress(MyGUI::KeyCode::ArrowDown);
		input.injectKeyRelease(MyGUI::KeyCode::ArrowDown);
		auto* list = input.getKeyFocusWidget()->castType<MyGUI::ListBox>();
		require(list->getVisible(), "The drop-down must be open");
		return list;
	}

	void checkBelow(MyGUI::ComboBox* _combo, MyGUI::ListBox* _list)
	{
		const auto coord = _combo->getAbsoluteCoord();
		require(
			_list->getCoord() == MyGUI::IntCoord(coord.left, coord.bottom(), coord.width, 60),
			"The open drop-down must follow the combo box");
	}

	void testScrolling(MyGUI::Gui& _gui, bool _smooth, unittest::CountingLayer& _popupLayer)
	{
		auto* scroll = _gui.createWidget<MyGUI::ScrollView>(
			"TestScrollView",
			MyGUI::IntCoord(100, 100, 300, 300),
			MyGUI::Align::Default,
			"Main");
		scroll->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
		scroll->setCanvasSize(600, 600);
		auto* parent =
			scroll->createWidget<MyGUI::Widget>("Default", MyGUI::IntCoord(80, 100, 200, 200), MyGUI::Align::Default);
		auto* combo = parent->createWidget<MyGUI::ComboBox>(
			"TestComboBox",
			MyGUI::IntCoord(10, 10, 120, 24),
			MyGUI::Align::Default);
		combo->addItem("First");
		combo->addItem("Second");
		combo->addItem("Third");
		combo->setIndexSelected(1);
		combo->setSmoothShow(_smooth);
		auto* list = openList(combo);
		checkBelow(combo, list);

		// Use the same wheel event as the scroll canvas receives from input handling.
		auto* canvas = scroll->getClientWidget();
		canvas->eventMouseWheel(canvas, -1);
		require(scroll->getViewOffset().top == -50, "The mouse wheel must scroll the canvas");
		checkBelow(combo, list);
		scroll->setViewOffset(MyGUI::IntPoint(-40, -80));
		checkBelow(combo, list);
		parent->setPosition(90, 120);
		checkBelow(combo, list);
		combo->setCoord(20, 20, 140, 24);
		checkBelow(combo, list);
		combo->setSize(160, 30);
		checkBelow(combo, list);
		combo->setPosition(30, 30);
		checkBelow(combo, list);
		_popupLayer.sizeQueryCount = 0;
		combo->eventChangeCoord(combo);
		require(_popupLayer.sizeQueryCount == 0, "Duplicate coordinates must not recalculate drop-down placement");
		require(combo->getIndexSelected() == 1, "Moving the list must preserve the selection");
		require(MyGUI::InputManager::getInstance().getKeyFocusWidget() == list, "The list must retain focus");

		// The normal screen-edge placement rule must still apply after moving.
		scroll->setPosition(100, 500);
		require(list->getCoord().bottom() == combo->getAbsoluteTop(), "The list must flip above the combo box");

		MyGUI::InputManager::getInstance().setKeyFocusWidget(nullptr);
		_gui.eventFrameStart(1.0f);
		require(!list->getVisible(), "Losing focus must close the list");
		const auto closedCoord = list->getCoord();
		scroll->setPosition(100, 100);
		require(list->getCoord() == closedCoord, "Closed lists must stop tracking movement");
		list = openList(combo);
		checkBelow(combo, list);

		// Position updates must also follow changes to the widget hierarchy.
		combo->detachFromWidget("Main");
		checkBelow(combo, list);
		combo->attachToWidget(parent);
		checkBelow(combo, list);
		parent->detachFromWidget("Main");
		checkBelow(combo, list);
		parent->attachToWidget(scroll);
		checkBelow(combo, list);
		scroll->setViewOffset(MyGUI::IntPoint(-20, -40));
		checkBelow(combo, list);

		// Destroying an open combo must remain safe, including during animation.
		_gui.destroyWidget(scroll);
		_gui.eventFrameStart(0.016f);
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
		unittest::loadResources("UnitTest_ComboBox/TestSkin.xml");
		testScrolling(gui, false, *popupLayer);
		testScrolling(gui, true, *popupLayer);
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
	return 0;
}
