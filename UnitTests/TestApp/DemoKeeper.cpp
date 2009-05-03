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

    void DemoKeeper::createScene()
    {
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
