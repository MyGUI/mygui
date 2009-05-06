/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"

#include "DemoKeeper.h"

#include "LoopController.h"
#include "FadeController.h"
#include "SkeletonState.h"

#include "RenderableObject.h"
#include "AnimatebleObject.h"

#include "KinematicalObject.h"
#include "RobotObject.h"

//#include "MyGUI_DefaultLayer.h"

#include <io.h>

namespace demo
{
	MyGUI::StaticImagePtr image;
	MyGUI::RotatingSkin * rotato;

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

	namespace space
	{
		// общий шаблон для кастов
		template <typename T1, typename T2>
		struct Convert
		{
			inline static T1 From(T2 t2);
			inline static T2 To(T1 t1);
		};
	}	

	namespace space1
	{
		// первый тип
		struct A
		{
		};
	}

	namespace space2
	{

		// второй тип
		struct B
		{
			template <typename T>
			B& operator = (const T& _rvalue) { *this = space::Convert<B, T>::From(_rvalue); return *this; }

			template <typename T>
			operator T () { return space::Convert<B, T>::To(*this); }
		};

	}

	namespace space
	{
		// специализация кастов для нужных типов
		template<> struct Convert<space2::B, space1::A>
		{
			inline static space2::B From(space1::A t2) { return space2::B(); }
			inline static space1::A To(space2::B t2) { return space1::A(); }
		};
	}

    void DemoKeeper::createScene()
    {

		space1::A a = space1::A();
		space2::B b = space2::B();

		b = a;
		a = b;

		//b = 1; // ERROR
		//bool test = b; // ERROR


		base::BaseManager::getInstance().addResourceLocation("../../Media/UnitTests/TestApp");

		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(400, 400, 400, 400), MyGUI::Align::Default, "Main");
		/*MyGUI::ListBoxPtr list = window->createWidget<MyGUI::ListBox>("List", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

		std::string data = "this is first item";
		list->addItem(data);
		data = "this is second item";
		list->addItem(data);
		data = "this is 3";
		list->addItem(data);
		data = "this is 4";
		list->addItem(data);
		data = "this is 5";
		list->addItem(data);
		data = "this is 6";
		list->addItem(data);
		data = "this is 7";
		list->addItem(data);
		data = "this is 8";
		list->addItem(data);
		data = "this is 9";
		list->addItem(data);//*/

		image = window->createWidget<MyGUI::StaticImage>("RotatingSkin", MyGUI::IntCoord(150, 150, 100, 150), MyGUI::Align::Default/*, "Main"*/);

		image->setImageTexture("nskingr.jpg");

		MyGUI::ISubWidget * main = image->getSubWidgetMain();
		rotato = main->castType<MyGUI::RotatingSkin>();
		rotato->setCenter(MyGUI::IntPoint(50, 75));

		//window->setSize(128, 289);

	}

    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
	{
		rotato->setAngle(atan2((double)arg.state.X.abs - rotato->getCenter(false).left, (double)arg.state.Y.abs - rotato->getCenter(false).top));

		return BaseManager::mouseMoved( arg );
	}

	bool DemoKeeper::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (id == OIS::MB_Right)
			rotato->setCenter(MyGUI::IntPoint(arg.state.X.abs, arg.state.Y.abs) - image->getAbsolutePosition());

		return BaseManager::mousePressed( arg, id );
	}

} // namespace demo
