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
#include "GraphNodeEvent.h"
#include "GraphNodePosition.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager(),
		mGraphView(nullptr)
	{
	}

	bool isConnectionOut(const std::string& _type)
	{
		return _type == "EventOut" || _type == "PositionOut";
	}

	bool isConnectionTypeSimple(const std::string& _type)
	{
		return _type == "EventIn" || _type == "EventOut";
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

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		base::BaseManager::getInstance().addResourceLocation("../../Media/UnitTests/UnitTest_GraphView");
	}

	void DemoKeeper::createScene()
    {
		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* entity = mSceneMgr->createEntity("Object", "Robot.mesh");
		node->attachObject(entity);

		createGrapView();
	}

	void DemoKeeper::notifyMenuCtrlAccept(wraps::BaseGraphView* _sender, const std::string& _id)
	{
		wraps::BaseGraphNode* node = nullptr;
		if (_id == "GraphEventNode")
			node = new GraphNodeEvent("Event");
		else if (_id == "GraphPositionNode")
			node = new GraphNodePosition("Position");
		else
			return;

		mGraphView->addItem(node);
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition();
		node->setAbsolutePosition(point);
	}

	void DemoKeeper::notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node)
	{
		_sender->removeItem(_node);
	}

	void DemoKeeper::createGrapView()
	{
		mGraphView = new GraphView();
		mGraphView->requestConnectPoint = MyGUI::newDelegate(requestConnectPoint);
		mGraphView->requestDisconnectPoint = MyGUI::newDelegate(requestDisconnectPoint);
		mGraphView->eventMenuCtrlAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyMenuCtrlAccept);
		mGraphView->eventNodeClosed = MyGUI::newDelegate(this, &DemoKeeper::notifyNodeClosed);

		mGraphView->addMenuItem("GraphEventNode", "GraphEventNode");
		mGraphView->addMenuItem("GraphPositionNode", "GraphPositionNode");

		/*GraphNodeEvent * node1 = new GraphNodeEvent("Node1");
		mGraphView->addItem(node1);
		node1->setPosition(10, 10);

		GraphNodePosition * node2 = new GraphNodePosition("Node2");
		mGraphView->addItem(node2);
		node2->setPosition(230, 70);

		GraphNodeSimple * node3 = new GraphNodeSimple("Node3");
		mGraphView->addItem(node3);
		node3->setPosition(450, 10);*/
	}

    void DemoKeeper::destroyScene()
    {
		delete mGraphView;
    }

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
