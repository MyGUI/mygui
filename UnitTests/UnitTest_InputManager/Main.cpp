#include "BehaviourTestSupport.h"
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
		{"Focus event arguments", testFocusArguments},
		{"Shared ancestor focus", testSharedAncestor},
		{"Destroy captured subtree", testDestroyCapturedSubtree},
		{"Modal stack", testModalStack},
	});
}
