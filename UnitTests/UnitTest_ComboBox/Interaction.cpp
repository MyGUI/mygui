#include "Interaction.h"
#include "BehaviourTestSupport.h"
#include "FixedFont.h"
#include "SkinTestContext.h"
#include <vector>

namespace interaction
{

	class InputCombo : public MyGUI::ComboBox
	{
		MYGUI_RTTI_DERIVED(InputCombo)
	public:
		MyGUI::Widget* part(std::string_view _name)
		{
			MyGUI::Widget* result = nullptr;
			assignWidget(result, _name);
			return result;
		}
	};

}

namespace
{

	struct ComboFixture
	{
		ComboFixture()
		{
			context.loadSkins();
			unittest::registerFixedFont();
			unittest::loadMedia("MyGUI_BlueWhiteTemplates.xml");
			unittest::createInputLayer();
			auto* popup = MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
			popup->castType<MyGUI::OverlappedLayer>()->setPick(true);
			MyGUI::FactoryManager::getInstance().registerFactory<interaction::InputCombo>(
				MyGUI::WidgetManager::getInstance().getCategoryName());
			combo = MyGUI::Gui::getInstance().createWidget<interaction::InputCombo>(
				"ComboBox",
				MyGUI::IntCoord(100, 100, 180, 26),
				MyGUI::Align::Default,
				"Main");
			combo->setSmoothShow(false);
			combo->setItemHeight(20);
			for (int i = 0; i < 10; ++i)
				combo->addItem(MyGUI::utility::toString("Item ", i));
			list = combo->part("List")->castType<MyGUI::ListBox>();
		}
		void open()
		{
			const auto button = combo->part("Button")->getAbsolutePosition() + MyGUI::IntPoint(3, 3);
			unittest::clickAt(button.left, button.top);
			unittest::require(list->getVisible(), "Button must open the combo popup");
		}
		unittest::SkinTestContext context;
		interaction::InputCombo* combo;
		MyGUI::ListBox* list;
	};

}

void testComboBoxPlacement()
{
	using unittest::require;
	using MyGUI::FlowDirection;
	struct Placement
	{
		FlowDirection direction;
		MyGUI::IntPoint position;
		MyGUI::IntCoord expected;
	};
	const Placement cases[]{
		{FlowDirection::TopToBottom, {100, 100}, {100, 126, 180, 60}},
		{FlowDirection::TopToBottom, {100, 550}, {100, 490, 180, 60}},
		{FlowDirection::BottomToTop, {100, 100}, {100, 40, 180, 60}},
		{FlowDirection::BottomToTop, {100, 20}, {100, 46, 180, 60}},
		{FlowDirection::LeftToRight, {100, 100}, {280, 100, 60, 26}},
		{FlowDirection::LeftToRight, {600, 100}, {540, 100, 60, 26}},
		{FlowDirection::RightToLeft, {100, 100}, {40, 100, 60, 26}},
		{FlowDirection::RightToLeft, {20, 100}, {200, 100, 60, 26}},
	};
	ComboFixture f;
	f.combo->setMaxListLength(60);
	for (const auto& item : cases)
	{
		f.combo->setFlowDirection(item.direction);
		f.combo->setPosition(item.position);
		f.open();
		require(
			f.list->getCoord() == item.expected,
			"Popup must honor its direction, edge fallback and maximum length");
		MyGUI::InputManager::getInstance().injectMouseMove(790, 590, 0);
		MyGUI::InputManager::getInstance().setKeyFocusWidget(nullptr);
		require(!f.list->getVisible(), "Leaving popup focus must close it before the next opening");
		MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	}
}

void testComboBoxEditableInput()
{
	using unittest::require;
	std::vector<size_t> changed, accepted;
	ComboFixture f;
	f.combo->setComboModeDrop(false);
	f.combo->setIndexSelected(2);
	f.combo->eventComboChangePosition +=
		MyGUI::newDelegate([&](MyGUI::ComboBox*, size_t _index) { changed.push_back(_index); }, 1);
	f.combo->eventComboAccept +=
		MyGUI::newDelegate([&](MyGUI::ComboBox*, size_t _index) { accepted.push_back(_index); }, 2);
	MyGUI::InputManager::getInstance().setKeyFocusWidget(f.combo);
	f.combo->setTextSelection(0, f.combo->getTextLength());
	unittest::keyStroke(MyGUI::KeyCode::X, 'x');
	require(
		f.combo->getOnlyText() == "x" && f.combo->getIndexSelected() == MyGUI::ITEM_NONE &&
			f.list->getIndexSelected() == MyGUI::ITEM_NONE && changed == std::vector<size_t>{MyGUI::ITEM_NONE} &&
			accepted.empty(),
		"Typing over a selected item must clear both selections and emit one change without accepting");
	unittest::keyStroke(MyGUI::KeyCode::Y, 'y');
	require(
		f.combo->getOnlyText() == "xy" && changed.size() == 1,
		"Further free typing must not repeat selection changes");
	f.open();
	unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
	unittest::keyStroke(MyGUI::KeyCode::Return);
	require(
		f.combo->getIndexSelected() == 0 && f.combo->getCaption() == "Item 0" &&
			changed == std::vector<size_t>({MyGUI::ITEM_NONE, 0}) && accepted.empty() && !f.list->getVisible(),
		"Choosing a suggestion must restore its caption and emit a change; editable mode must not emit drop "
		"acceptance");
}

void testComboBoxInteractions()
{
	using unittest::require;
	using interaction::InputCombo;
	std::vector<size_t> accepted, changed;
	unittest::SkinTestContext context;
	context.loadSkins();
	unittest::registerFixedFont();
	unittest::loadMedia("MyGUI_BlueWhiteTemplates.xml");
	unittest::createInputLayer();
	auto* popup = MyGUI::LayerManager::getInstance().createLayerAt("Popup", "OverlappedLayer", 1);
	popup->castType<MyGUI::OverlappedLayer>()->setPick(true);
	MyGUI::FactoryManager::getInstance().registerFactory<InputCombo>(
		MyGUI::WidgetManager::getInstance().getCategoryName());
	auto* combo = MyGUI::Gui::getInstance().createWidget<InputCombo>(
		"ComboBox",
		MyGUI::IntCoord(20, 20, 180, 26),
		MyGUI::Align::Default,
		"Main");
	combo->setComboModeDrop(true);
	combo->setSmoothShow(false);
	combo->setItemHeight(20);
	combo->addItem("First");
	combo->addItem("Second");
	combo->addItem("Third");
	combo->setIndexSelected(0);
	combo->eventComboAccept +=
		MyGUI::newDelegate([&](MyGUI::ComboBox*, size_t _index) { accepted.push_back(_index); }, 1);
	combo->eventComboChangePosition +=
		MyGUI::newDelegate([&](MyGUI::ComboBox*, size_t _index) { changed.push_back(_index); }, 2);
	auto& input = MyGUI::InputManager::getInstance();
	input.setKeyFocusWidget(combo);
	auto point = combo->getClientWidget()->getAbsolutePosition() + MyGUI::IntPoint(5, 5);
	input.injectMouseMove(point.left, point.top, 0);
	input.injectMouseMove(point.left, point.top, -1);
	require(
		combo->getIndexSelected() == 1 && combo->getCaption() == "Second",
		"Wheel must update selected index and caption");
	require(changed == std::vector<size_t>{1} && accepted.empty(), "Wheel must change selection without accepting it");
	auto button = combo->part("Button")->getAbsolutePosition() + MyGUI::IntPoint(3, 3);
	unittest::clickAt(button.left, button.top, -1);
	auto* list = combo->part("List")->castType<MyGUI::ListBox>();
	require(list->getVisible() && input.getKeyFocusWidget() == list, "Button must open and focus the list");
	unittest::keyStroke(MyGUI::KeyCode::ArrowDown);
	unittest::keyStroke(MyGUI::KeyCode::Return);
	require(
		combo->getIndexSelected() == 2 && combo->getCaption() == "Third",
		"Keyboard acceptance must copy the selected item");
	require(
		accepted == std::vector<size_t>{2} && !list->getVisible() && input.getKeyFocusWidget() == combo,
		"Acceptance must emit once, close popup and return focus");
	MyGUI::Gui::getInstance().eventFrameStart(1.0f);
	unittest::clickAt(point.left, point.top, -1);
	require(list->getVisible(), "Drop-mode client click must open the list");
	auto row = list->getWidgetByIndex(0)->getAbsolutePosition() + MyGUI::IntPoint(5, 5);
	unittest::clickAt(row.left, row.top, -1);
	require(
		combo->getIndexSelected() == 0 && combo->getCaption() == "First" && accepted == std::vector<size_t>({2, 0}),
		"Mouse activation must accept exactly the clicked row");
	require(!list->getVisible(), "Mouse activation must close the popup");
	input.setKeyFocusWidget(nullptr);
	input.injectMouseMove(point.left, point.top, -2);
	require(combo->getIndexSelected() == 0, "Wheel on an unfocused combo must not change selection");
}

void testComboBoxWheelBubbling()
{
	using unittest::require;
	ComboFixture f;
	unittest::loadResources("UnitTest_ComboBox/TestSkin.xml");
	auto* outer = MyGUI::Gui::getInstance().createWidget<MyGUI::ScrollView>(
		"TestScrollView",
		MyGUI::IntCoord(0, 0, 400, 300),
		MyGUI::Align::Default,
		"Main");
	outer->setCanvasAlign(MyGUI::Align::Left | MyGUI::Align::Top);
	outer->setCanvasSize(400, 1000);
	f.combo->attachToWidget(outer);
	f.combo->setFontName("UnitTestFixedFont");
	f.combo->setFontHeight(20);
	f.combo->setIndexSelected(8);
	auto& input = MyGUI::InputManager::getInstance();
	input.setKeyFocusWidget(f.combo);
	auto* client = f.combo->getClientWidget();
	int changes = 0;
	f.combo->eventComboChangePosition += MyGUI::newDelegate([&](MyGUI::ComboBox*, size_t) { ++changes; }, 1);
	client->_riseMouseWheel(-1);
	require(
		f.combo->getIndexSelected() == 9 && changes == 1 && outer->getViewOffset().top == 0,
		"The last selection step must be consumed without scrolling the parent");
	client->_riseMouseWheel(-1);
	require(changes == 1 && outer->getViewOffset().top == -50, "A selection boundary must allow parent scrolling");
	input.setKeyFocusWidget(nullptr);
	client->_riseMouseWheel(1);
	require(
		f.combo->getIndexSelected() == 9 && outer->getViewOffset().top == 0,
		"An unfocused combo must pass wheel input to its parent");
	input.setKeyFocusWidget(f.combo);
	client->eventMouseWheelRouted += MyGUI::newDelegate([](MyGUI::MouseWheelEvent& event) { event.setHandled(); }, 2);
	client->_riseMouseWheel(1);
	require(f.combo->getIndexSelected() == 9 && changes == 1, "A child routed handler can intercept combo selection");
	client->eventMouseWheelRouted.clear();
	f.combo->removeAllItems();
	client->_riseMouseWheel(-1);
	require(outer->getViewOffset().top == -50, "An empty combo must not consume wheel input");

	// Changing a long caption must not also execute EditBox wheel scrolling.
	f.combo->addItem("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	f.combo->addItem("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
	f.combo->setIndexSelected(0);
	const auto captionOffset = f.combo->getHScrollPosition();
	f.combo->setIndexSelected(1);
	f.combo->setHScrollPosition(0);
	client->_riseMouseWheel(1);
	require(
		f.combo->getIndexSelected() == 0 && f.combo->getHScrollPosition() == captionOffset,
		"Combo selection must take priority over scrolling a long editable caption");
	client->_riseMouseWheel(0);
	require(f.combo->getIndexSelected() == 0, "Zero delta must not change combo selection");

	// A long caption must not consume boundary input that belongs to the outer view.
	require(f.combo->getHScrollRange() > 1, "The long caption must have a horizontal scroll range");
	for (auto* target : {static_cast<MyGUI::Widget*>(f.combo), client})
	{
		outer->setViewOffset(MyGUI::IntPoint(0, -100));
		f.combo->setHScrollPosition(f.combo->getHScrollRange() - 1);
		const auto offset = f.combo->getHScrollPosition();
		target->_riseMouseWheel(1);
		require(
			f.combo->getIndexSelected() == 0 && f.combo->getHScrollPosition() == offset &&
				outer->getViewOffset().top == -50,
			"Wheel at the first selection must bubble without scrolling the caption");

		input.setKeyFocusWidget(nullptr);
		f.combo->setHScrollPosition(0);
		target->_riseMouseWheel(-1);
		require(
			f.combo->getIndexSelected() == 0 && f.combo->getHScrollPosition() == 0 &&
				outer->getViewOffset().top == -100,
			"An unfocused combo must bubble without scrolling its long caption");
		input.setKeyFocusWidget(f.combo);
	}

	outer->setViewOffset(MyGUI::IntPoint());
	for (int i = 0; i < 10; ++i)
		f.combo->addItem("popup row");
	f.combo->setMaxListLength(60);
	f.open();
	f.list->setScrollPosition(0);
	const auto popupSelection = f.combo->getIndexSelected();
	f.list->_riseMouseWheel(-1);
	require(
		f.list->_getScrollPosition() > 0 && f.combo->getIndexSelected() == popupSelection &&
			outer->getViewOffset().top == 0,
		"The popup list must scroll independently of combo selection and the outer view");
	for (int i = 0; i < 20; ++i)
		f.list->_riseMouseWheel(-1);
	const auto popupOffset = f.list->_getScrollPosition();
	f.list->_riseMouseWheel(-1);
	require(
		f.list->_getScrollPosition() == popupOffset && f.combo->getIndexSelected() == popupSelection &&
			outer->getViewOffset().top == 0,
		"The popup boundary must stop unconsumed wheel input at the end of the list");
}
