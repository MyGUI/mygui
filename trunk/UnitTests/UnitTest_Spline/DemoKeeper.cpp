/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	std::vector<MyGUI::FloatPoint> linePoints;
	MyGUI::PolygonalSkin* polygonalSkin;
	MyGUI::Widget* widget;

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("BackHelp.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::StaticText>()->setCaption("PolygonalSkin (aka line).");

		MyGUI::Window* window = getGUI()->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 400, 400), MyGUI::Align::Default, "Main");

		widget = window->createWidget<MyGUI::Widget>("PolygonalSkin", MyGUI::IntCoord(0, 0, 400, 400), MyGUI::Align::Default);

		MyGUI::ISubWidget * main = widget->getSubWidgetMain();
		polygonalSkin = main->castType<MyGUI::PolygonalSkin>();

		polygonalSkin->setWidth(8.0f);

		//linePoints.push_back(MyGUI::FloatPoint(70, 70));
		//linePoints.push_back(MyGUI::FloatPoint(10, 10));
		polygonalSkin->setPoints(linePoints);
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		linePoints.push_back(MyGUI::FloatPoint((float)_absx - widget->getAbsoluteLeft(), (float)_absy - widget->getAbsoluteTop()));
		polygonalSkin->setPoints(linePoints);
		if (_id == MyGUI::MouseButton::Right)
			linePoints.clear();

		base::BaseManager::injectMousePress(_absx, _absy, _id);
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		base::BaseManager::injectMouseMove(_absx, _absy, _absz);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
