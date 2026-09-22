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
