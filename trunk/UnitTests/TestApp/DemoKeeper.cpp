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

#include "MyGUI_LayerKeeper.h"

#include <io.h>

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

    void DemoKeeper::createScene()
    {

		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10, 10, 250, 289), MyGUI::Align::Default, "Main");
		MyGUI::ListBoxPtr list = window->createWidget<MyGUI::ListBox>("List", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

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

		window->setSize(128, 289);

	}

    void DemoKeeper::destroyScene()
    {
    }

} // namespace demo
