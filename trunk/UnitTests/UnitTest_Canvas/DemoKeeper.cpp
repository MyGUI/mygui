/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	bool isConnectionOut(const std::string& _type)
	{
		return _type == "SimpleOut" || _type == "ExtensionOut";
	}

	bool isConnectionTypeSimple(const std::string& _type)
	{
		return _type == "SimpleIn" || _type == "SimpleOut";
	}

	void requestConnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to, bool& _result)
	{
		if (_to == nullptr)
		{
			if (isConnectionOut(_from->getConnectionType()))
			{
				_result = true;
			}
		}
		else
		{
			if (
				_from != _to
				&& isConnectionTypeSimple(_from->getConnectionType()) == isConnectionTypeSimple(_to->getConnectionType())
				&& isConnectionOut(_from->getConnectionType()) && !isConnectionOut(_to->getConnectionType())
				&& !_sender->isConnecting(_from, _to)
			)
			{
				_result = true;
			}
		}
	}

	void requestDisconnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to, bool& _result)
	{
		_result = true;
	}

	DemoKeeper::DemoKeeper() :
		mGraphView(nullptr),
		mNode1(nullptr),
		mNode2(nullptr),
		mNode3(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_GraphView");
	}

	void DemoKeeper::eventChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size)
	{
		const MyGUI::IntCoord& coord = mScrollView3->getClientCoord();
		if (_size.width < coord.width) _size.width = coord.width;
		if (_size.height < coord.height) _size.height = coord.height;
		mScrollView3->setCanvasSize(_size);
	}

	void DemoKeeper::createScene()
	{
		mPanel3 = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, 700, 500), MyGUI::Align::Default, "Overlapped");
		mPanel3->setCaption("GraphView");
		mScrollView3 = mPanel3->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel3->getClientCoord().size()), MyGUI::Align::Stretch);
		mScrollView3->setCanvasAlign(MyGUI::Align::Default);

		mCanvas3 = mScrollView3->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel3->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas3->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
		mCanvas3->updateTexture();


		mGraphView = new wraps::BaseGraphView("", mCanvas3);
		mGraphView->requestConnectPoint = MyGUI::newDelegate(requestConnectPoint);
		mGraphView->requestDisconnectPoint = MyGUI::newDelegate(requestDisconnectPoint);
		mGraphView->eventChangeSize = MyGUI::newDelegate(this, &DemoKeeper::eventChangeSize);

		mNode1 = new GraphNodeSimple("Node1");
		mGraphView->addItem(mNode1);
		mNode1->setPosition(10, 10);

		mNode2 = new GraphNodeSimple("Node2");
		mGraphView->addItem(mNode2);
		mNode2->setPosition(230, 70);

		mNode3 = new GraphNodeSimple("Node3");
		mGraphView->addItem(mNode3);
		mNode3->setPosition(450, 10);
	}

	void DemoKeeper::destroyScene()
	{
		delete mGraphView;
		mGraphView = nullptr;

		delete mNode1;
		mNode1 = nullptr;

		delete mNode2;
		mNode2 = nullptr;

		delete mNode3;
		mNode3 = nullptr;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
