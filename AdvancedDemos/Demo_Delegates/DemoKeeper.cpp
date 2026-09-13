/*!
	@file
	@author		George Evmenov
	@date		01/2011
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#include <functional>
#include <memory>

namespace demo
{

	class SomeClass
	{
	public:
		SomeClass(int _value) :
			mValue(_value)
		{
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		int mValue;
	};

	using SomeClassPtr = std::shared_ptr<SomeClass>;

	static void Delegate_W(SomeClassPtr _foo, MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Functor call. " + MyGUI::utility::toString(_foo->getValue()));
	}

	static void handleClick_GlobalFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Function call");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
		const int yStep = 40;
		int y = 0;
		MyGUI::Button* button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("Function");
		button->eventMouseButtonClick += MyGUI::newDelegate(handleClick_GlobalFunction);
		y += yStep;

		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("Class method");
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::handleClick_MemberFunction);
		y += yStep;

		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("Class method");
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::handleClick_MemberFunction);
		y += yStep;

		const DemoKeeper constDemoKeeper;
		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("Const class method");
		button->eventMouseButtonClick +=
			MyGUI::newDelegate(&constDemoKeeper, &DemoKeeper::handleClick_ConstMemberFunction);
		y += yStep;

		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("Static class method");
		button->eventMouseButtonClick += MyGUI::newDelegate(handleClick_StaticMemberFunction);
		// or
		//button->eventMouseButtonClick += MyGUI::newDelegate(DemoKeeper::handleClick_StaticMemberFunction);
		y += yStep;

		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("std::function");

		SomeClassPtr classInstance(new SomeClass(4));
		std::function<void(MyGUI::Widget*)> f = std::bind(Delegate_W, classInstance, std::placeholders::_1); // NOLINT
		// note that we need to specify user-defined delegate Id to make it possible to use `eventMouseButtonClick -=`
		button->eventMouseButtonClick += MyGUI::newDelegate(f, 123);
		y += yStep;

		button =
			gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, y, 200, 30), MyGUI::Align::Default, "Main");
		button->setCaption("lambda");

		// note that we need to specify user-defined delegate Id to make it possible to use `eventMouseButtonClick -=`
		button->eventMouseButtonClick += MyGUI::newDelegate(
			[classInstance](MyGUI::Widget* _sender) { _sender->castType<MyGUI::Button>()->setCaption("Lambda call."); },
			1234);
		y += yStep;
	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::handleClick_MemberFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Class method call");
	}

	void DemoKeeper::handleClick_ConstMemberFunction(MyGUI::Widget* _sender) const
	{
		_sender->castType<MyGUI::Button>()->setCaption("Const class method call");
	}

	void DemoKeeper::handleClick_StaticMemberFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Static class method call");
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
