/*!
	@file
	@author		George Evmenov
	@date		01/2011
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

//#define BOOST

#ifdef BOOST
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "FunctorDelegate.h"
#endif

namespace demo
{

#ifdef BOOST
	typedef boost::function<void (MyGUI::Widget* _sender)> Delegate_W_Type;

	class SomeClass
	{
	public:
		SomeClass(int _value) :
			mValue(_value)
		{
		}

		int getValue() { return mValue; }
	private:
		int mValue;
	};

	typedef boost::shared_ptr<SomeClass> SomeClassPtr;

	static void Delegate_W(SomeClassPtr _foo, MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Functor call. " + MyGUI::utility::toString(_foo->getValue()));
	}
#endif

	void handleClick_GlobalFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Function call");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
		MyGUI::Button* button1 = gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 200, 30), MyGUI::Align::Default, "Main");
		button1->setCaption("Function");
		button1->eventMouseButtonClick += MyGUI::newDelegate(handleClick_GlobalFunction);

		MyGUI::Button* button2 = gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 50, 200, 30), MyGUI::Align::Default, "Main");
		button2->setCaption("Class method");
		button2->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::handleClick_MemberFunction);

		MyGUI::Button* button3 = gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 90, 200, 30), MyGUI::Align::Default, "Main");
		button3->setCaption("Static class method");
		button3->eventMouseButtonClick += MyGUI::newDelegate(handleClick_StaticMemberFunction);
		// or
		//button3->eventMouseButtonClick += MyGUI::newDelegate(DemoKeeper::handleClick_StaticMemberFunction);

#ifdef BOOST
		MyGUI::Button* button4 = gui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 130, 200, 30), MyGUI::Align::Default, "Main");
		button4->setCaption("Boost functor");

		SomeClassPtr classInstance(new SomeClass(4));
		Delegate_W_Type f = boost::bind(Delegate_W, classInstance, _1);
		button4->eventMouseButtonClick += MyGUI::newDelegate(f);

		//button4->eventMouseButtonClick += MyGUI::newDelegate2(boost::bind(Delegate_W, classInstance, _1));
#endif
	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::handleClick_MemberFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Class method call");
	}

	void DemoKeeper::handleClick_StaticMemberFunction(MyGUI::Widget* _sender)
	{
		_sender->castType<MyGUI::Button>()->setCaption("Static class method call");
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
