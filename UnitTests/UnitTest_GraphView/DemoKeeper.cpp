/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "GraphNodeSimple.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager(),
		mGraphView(nullptr)
	{
	}

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

	void DemoKeeper::createScene()
    {
		base::BaseManager::getInstance().addResourceLocation("../../Media/UnitTests/UnitTest_GraphView");

		mGraphView = new GraphView();
		mGraphView->requestConnectPoint = MyGUI::newDelegate(requestConnectPoint);
		mGraphView->requestDisconnectPoint = MyGUI::newDelegate(requestDisconnectPoint);

		GraphNodeSimple * node1 = new GraphNodeSimple("Node1");
		mGraphView->addItem(node1);
		node1->setPosition(10, 10);

		GraphNodeSimple * node2 = new GraphNodeSimple("Node2");
		mGraphView->addItem(node2);
		node2->setPosition(230, 70);

		GraphNodeSimple * node3 = new GraphNodeSimple("Node3");
		mGraphView->addItem(node3);
		node3->setPosition(450, 10);
	}

    void DemoKeeper::destroyScene()
    {
		delete mGraphView;
    }

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
