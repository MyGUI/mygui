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
	// changing this also require changes in makeBezier, that always use 4 points
	const int PointsCount = 4;
	MyGUI::Window* point[PointsCount];

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
		for (int i = 0; i < PointsCount; ++i)
		{
			points.push_back(MyGUI::FloatPoint(point[i]->getLeft() + 8.0f, point[i]->getTop() + 8.0f));
		}
		makeBezier(points, 16);
		polygonalSkin->setPoints(linePoints);
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia() + "/Common/Tools");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::ResourceManager::getInstance().load("FrameworkSkin.xml");
		MyGUI::ResourceManager::getInstance().load("SelectionSkins.xml");

		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("PolygonalSkin usage. Drag white rectangles to move points for bezier curve.");

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 400, 400), MyGUI::Align::Default, "Main");
		window->setCaption("Bezier curve");

		// create widget with skin that contain specific sub skin - PolygonalSkin
		MyGUI::Widget* widget = window->createWidget<MyGUI::Widget>("PolygonalSkin", MyGUI::IntCoord(0, 0, 400, 400), MyGUI::Align::Default);
		widget->setColour(MyGUI::Colour::Red);
		// get main subskin
		MyGUI::ISubWidget* main = widget->getSubWidgetMain();
		polygonalSkin = main->castType<MyGUI::PolygonalSkin>();

		// set PolygonalSkin properties and points
		polygonalSkin->setWidth(8.0f);
		polygonalSkin->setPoints(linePoints);


		for (int i = 0; i < PointsCount; ++i)
		{
			point[i] = window->createWidget<MyGUI::Window>("PositionBorderWhite", MyGUI::IntCoord(10 + (i + i % 2 * 3) * 40, 10 + (i + i / 2 * 3) * 40, 16, 16), MyGUI::Align::Default);
			point[i]->eventWindowChangeCoord += newDelegate(notifyPointMove);
			point[i]->setColour(MyGUI::Colour::Green);
		}

		notifyPointMove(point[0]);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
