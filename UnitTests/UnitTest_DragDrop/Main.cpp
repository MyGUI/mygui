#include "BehaviourTestSupport.h"
#include "TestRunner.h"
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

}

int main()
{
	return unittest::runTests(
		{{"Accepted drop", testDrop<true>},
		 {"Rejected drop", testDrop<false>},
		 {"Cancellation and recovery", testCancelAndMiss}});
}
