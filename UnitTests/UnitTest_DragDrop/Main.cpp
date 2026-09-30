#include "BehaviourTestSupport.h"
#include "TestRunner.h"
#include <cmath>
#include <vector>

namespace
{

	using unittest::require;
	struct Fixture
	{
		bool accept{true};
		bool start{true};
		std::vector<MyGUI::DDItemState> states;
		std::vector<bool> results;
		MyGUI::DDItemInfo resultInfo;
		unittest::TestContext context;
		MyGUI::ItemBox* source;
		MyGUI::ItemBox* target;
		MyGUI::Widget* dragPreview{nullptr};

		Fixture()
		{
			unittest::createInputLayer();
			MyGUI::LayerManager::getInstance().createLayerAt("DragAndDrop", "OverlappedLayer", 1);
			unittest::loadResources("UnitTest_ItemBox/TestSkin.xml");
			source = create(20);
			target = create(220);
			source->eventStartDrag += MyGUI::newDelegate(this, &Fixture::starting);
			source->eventRequestDrop += MyGUI::newDelegate(this, &Fixture::request);
			source->eventDropResult += MyGUI::newDelegate(this, &Fixture::result);
			source->eventChangeDDState += MyGUI::newDelegate(this, &Fixture::state);
		}
		MyGUI::ItemBox* create(int _left)
		{
			auto* box = context.getGui().createWidget<MyGUI::ItemBox>(
				"TestItemBox",
				MyGUI::IntCoord(_left, 20, 120, 80),
				MyGUI::Align::Default,
				"Main");
			box->requestCoordItem = MyGUI::newDelegate(this, &Fixture::coordinate);
			box->requestCreateWidgetItem = MyGUI::newDelegate(this, &Fixture::createItem);
			box->setNeedDragDrop(true);
			box->setSize(120, 80);
			box->addItem(1);
			box->addItem(2);
			return box;
		}
		void coordinate(MyGUI::ItemBox*, MyGUI::IntCoord& _coord, bool)
		{
			_coord = {0, 0, 40, 20};
		}
		void createItem(MyGUI::ItemBox*, MyGUI::Widget* _item)
		{
			_item->setNeedKeyFocus(true);
			if (_item->getParent() == nullptr)
				dragPreview = _item;
		}
		void starting(MyGUI::DDContainer*, const MyGUI::DDItemInfo&, bool& _result)
		{
			_result = start;
		}
		void request(MyGUI::DDContainer*, const MyGUI::DDItemInfo& _info, bool& _result)
		{
			require(_info.sender == source && _info.sender_index == 0, "Drop request must identify the dragged item");
			_result = accept && _info.receiver == target;
		}
		void result(MyGUI::DDContainer*, const MyGUI::DDItemInfo& _info, bool _result)
		{
			results.push_back(_result);
			resultInfo = _info;
		}
		void state(MyGUI::DDContainer*, MyGUI::DDItemState _state)
		{
			states.push_back(_state);
		}
		void begin()
		{
			auto& input = MyGUI::InputManager::getInstance();
			input.injectMouseMove(25, 25, 0);
			input.injectMousePress(25, 25, MyGUI::MouseButton::Left);
			input.injectMouseMove(265, 25, 0);
		}
	};

	template<bool Accept>
	void testDrop()
	{
		Fixture fixture;
		fixture.accept = Accept;
		fixture.begin();
		require(
			fixture.states ==
				std::vector<MyGUI::DDItemState>{
					MyGUI::DDItemState::Start,
					Accept ? MyGUI::DDItemState::Accept : MyGUI::DDItemState::Refuse},
			"Drag must report start and target acceptance");
		MyGUI::InputManager::getInstance().injectMouseRelease(265, 25, MyGUI::MouseButton::Left);
		require(fixture.results == std::vector<bool>{Accept}, "Release must emit exactly one drop result");
		require(
			fixture.resultInfo.receiver == fixture.target && fixture.resultInfo.receiver_index == 1,
			"Drop must preserve target container and item identity");
		require(fixture.states.back() == MyGUI::DDItemState::End, "Release must end the drag");
		require(!MyGUI::InputManager::getInstance().isCaptureMouse(), "Release must clear capture");
	}

	void testCancelAndMiss()
	{
		Fixture fixture;
		fixture.begin();
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(500, 400, 0);
		require(fixture.states.back() == MyGUI::DDItemState::Miss, "Leaving all containers must clear acceptance");
		input.injectMouseMove(265, 25, 0);
		input.injectMousePress(265, 25, MyGUI::MouseButton::Right);
		input.injectMouseRelease(265, 25, MyGUI::MouseButton::Right);
		input.injectMouseRelease(265, 25, MyGUI::MouseButton::Left);
		require(fixture.results == std::vector<bool>{false}, "Cancellation must reject the drop exactly once");
		require(fixture.states.back() == MyGUI::DDItemState::End, "Cancellation must end the state sequence");
		fixture.context.getGui().eventFrameStart(1.0f);
		fixture.begin();
		input.injectMouseRelease(265, 25, MyGUI::MouseButton::Left);
		require(fixture.results == std::vector<bool>({false, true}), "A new drag after cancellation must work");
	}

	void testRotatedDragPreview()
	{
		Fixture fixture;
		fixture.source->setRotation(1.5707963268f);
		const MyGUI::FloatPoint start = fixture.source->rotatePoint(MyGUI::FloatPoint(25.0f, 25.0f));
		const MyGUI::IntPoint press((int)std::lround(start.left), (int)std::lround(start.top));
		MyGUI::Widget* grabbed = MyGUI::LayerManager::getInstance().getWidgetFromPoint(press.left, press.top);
		require(grabbed != nullptr && grabbed != fixture.source, "Rotated item must be pickable");
		const MyGUI::IntPoint origin = grabbed->getAbsolutePosition();
		const MyGUI::FloatPoint displayedOrigin =
			grabbed->rotatePoint(MyGUI::FloatPoint((float)origin.left, (float)origin.top));
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(press.left, press.top, 0);
		input.injectMousePress(press.left, press.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(press.left + 80, press.top + 40, 0);
		require(fixture.dragPreview != nullptr, "Dragging a rotated item must create its preview");
		require(
			fixture.dragPreview->getPosition() ==
				MyGUI::IntPoint(
					(int)std::lround(displayedOrigin.left) + 80,
					(int)std::lround(displayedOrigin.top) + 40),
			"Drag preview must follow the displayed item origin without jumping");
		input.injectMouseRelease(press.left + 80, press.top + 40, MyGUI::MouseButton::Left);
	}

}

int main()
{
	return unittest::runTests({
		{"Accepted drop", testDrop<true>},
		{"Rejected drop", testDrop<false>},
		{"Cancellation and recovery", testCancelAndMiss},
		{"Rotated drag preview", testRotatedDragPreview},
	});
}
