/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	std::vector<MyGUI::FloatPoint> linePoints;
	MyGUI::PolygonalSkin* polygonalSkin;
	MyGUI::Widget* widget;

	void makeBezier(const std::vector<MyGUI::FloatPoint>& _points, size_t _pointsNumber)
	{
		linePoints.clear();
		for (size_t i = 0; i < _pointsNumber; ++i)
		{
			float t = float(i) / (_pointsNumber - 1);
			float left = _points[0].left * pow(1 - t, 3) + 3 * _points[1].left * pow(1 - t, 2) * t + 3 * _points[2].left * (1 - t) * t * t + t * t * t * _points[3].left;
			float top = _points[0].top * pow(1 - t, 3) + 3 * _points[1].top * pow(1 - t, 2) * t + 3 * _points[2].top * (1 - t) * t * t + t * t * t * _points[3].top;
			linePoints.push_back(MyGUI::FloatPoint(left, top));
		}
	}

	void notifyPointMove(MyGUI::Window* _sender)
	{

		std::vector<MyGUI::FloatPoint> points;
		points.push_back(MyGUI::FloatPoint(10, 10));
		points.push_back(MyGUI::FloatPoint(40, 10));
		points.push_back(MyGUI::FloatPoint(40, 80));
		points.push_back(MyGUI::FloatPoint(70, 80));
		makeBezier(points, 16);
		//linePoints.clear();
		linePoints.push_back(MyGUI::FloatPoint(100, 10));
		linePoints.push_back(MyGUI::FloatPoint(_sender->getLeft() - 2.0f, _sender->getTop() - 2.0f));
		linePoints.push_back(MyGUI::FloatPoint(100, 100));
		polygonalSkin->setPoints(linePoints);
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("BackHelp.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::StaticText>()->setCaption("PolygonalSkin (aka line).");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 400, 400), MyGUI::Align::Default, "Main");
		window->setCaption("Click Me!");


		// create widget with skin that contain specific sub skin - PolygonalSkin
		widget = window->createWidget<MyGUI::Widget>("PolygonalSkin", MyGUI::IntCoord(0, 0, 400, 400), MyGUI::Align::Default);
		widget->setColour(MyGUI::Colour::Red);
		// get main subskin
		MyGUI::ISubWidget* main = widget->getSubWidgetMain();
		polygonalSkin = main->castType<MyGUI::PolygonalSkin>();

		// set PolygonalSkin properties and points
		polygonalSkin->setWidth(8.0f);
		polygonalSkin->setPoints(linePoints);


		MyGUI::Window* point = window->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(140, 30, 16, 16), MyGUI::Align::Default);
		point->eventWindowChangeCoord += newDelegate(notifyPointMove);
		notifyPointMove(point);
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
