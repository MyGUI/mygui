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

	const int PointsCount = 4;
	int bezierQuality = 16;
	int stroke = 0;
	MyGUI::Widget* point[PointsCount];

	DemoKeeper::DemoKeeper() :
		mClient(nullptr),
		mPolygonalSkin(nullptr),
		mQualityText(nullptr),
		mStrokeText(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia() + "/Common/Tools");
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_Spline");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::ResourceManager::getInstance().load("SplineSkin.xml");

		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("PolygonalSkin usage. Drag white rectangles to move points for bezier curve.");

		MyGUI::VectorWidgetPtr widgets = MyGUI::LayoutManager::getInstance().loadLayout("SplineWindow.layout");
		mQualityText = widgets.at(0)->findWidget("SplineText")->castType<MyGUI::TextBox>();
		mStrokeText = widgets.at(0)->findWidget("StrokeText")->castType<MyGUI::TextBox>();
		MyGUI::ScrollBar* qualityScroll = widgets.at(0)->findWidget("SplineQuality")->castType<MyGUI::ScrollBar>();
		qualityScroll->eventScrollChangePosition += MyGUI::newDelegate(this, &DemoKeeper::notifyChangeQuality);
		MyGUI::ScrollBar* strokeScroll = widgets.at(0)->findWidget("StrokeQuality")->castType<MyGUI::ScrollBar>();
		strokeScroll->eventScrollChangePosition += MyGUI::newDelegate(this, &DemoKeeper::notifyChangeStroke);
		mClient = widgets.at(0)->findWidget("SplineClient");

		// create widget with skin that contain specific sub skin - PolygonalSkin
		MyGUI::Widget* widget = mClient->createWidget<MyGUI::Widget>("PolygonalSkin", MyGUI::IntCoord(MyGUI::IntPoint(), mClient->getSize()), MyGUI::Align::Stretch);
		widget->setColour(MyGUI::Colour::Red);
		// get main subskin
		MyGUI::ISubWidget* main = widget->getSubWidgetMain();
		mPolygonalSkin = main->castType<MyGUI::PolygonalSkin>();

		// set PolygonalSkin properties and points
		mPolygonalSkin->setWidth(8.0f);
		mPolygonalSkin->setStroke(0);

		for (int i = 0; i < PointsCount; ++i)
		{
			point[i] = mClient->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10 + (i + i % 2 * 3) * 40, 10 + (i + i / 2 * 3) * 40, 16, 16), MyGUI::Align::Default);
			point[i]->eventMouseDrag += newDelegate(this, &DemoKeeper::notifyPointMove);
			point[i]->eventMouseButtonPressed += newDelegate(this, &DemoKeeper::notifyPointPressed);
		}

		notifyChangeQuality(qualityScroll, 12);
		updateSpline();
	}

	void DemoKeeper::makeBezier(const std::vector<MyGUI::FloatPoint>& _points, size_t _pointsNumber)
	{
		mLinePoints.clear();
		for (size_t i = 0; i < _pointsNumber; ++i)
		{
			float t = float(i) / (_pointsNumber - 1);
			float left = _points[0].left * pow(1 - t, 3) + 3 * _points[1].left * pow(1 - t, 2) * t + 3 * _points[2].left * (1 - t) * t * t + t * t * t * _points[3].left;
			float top = _points[0].top * pow(1 - t, 3) + 3 * _points[1].top * pow(1 - t, 2) * t + 3 * _points[2].top * (1 - t) * t * t + t * t * t * _points[3].top;
			mLinePoints.push_back(MyGUI::FloatPoint(left, top));
		}
	}

	void DemoKeeper::updateSpline()
	{
		std::vector<MyGUI::FloatPoint> points;
		for (int i = 0; i < PointsCount; ++i)
		{
			points.push_back(MyGUI::FloatPoint(point[i]->getLeft() + 8.0f, point[i]->getTop() + 8.0f));
		}
		makeBezier(points, bezierQuality);
		mPolygonalSkin->setPoints(mLinePoints);
		mPolygonalSkin->setStroke(stroke);
	}

	void DemoKeeper::notifyPointMove(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		_sender->setPosition(MyGUI::IntPoint(_left, _top) - _sender->getCroppedParent()->getAbsolutePosition() - mOffset);
		updateSpline();
	}

	void DemoKeeper::notifyPointPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		mOffset = MyGUI::IntPoint(_left, _top) - _sender->getAbsolutePosition();
	}

	void DemoKeeper::notifyChangeQuality(MyGUI::ScrollBar* _sender, size_t _position)
	{
		bezierQuality = _position + 4;
		mQualityText->setCaption(MyGUI::utility::toString("Quality: ", bezierQuality));
		updateSpline();
	}

	void DemoKeeper::notifyChangeStroke(MyGUI::ScrollBar* _sender, size_t _position)
	{
		stroke = _position;
		mStrokeText->setCaption(MyGUI::utility::toString("Stroke: ", _position));
		updateSpline();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
