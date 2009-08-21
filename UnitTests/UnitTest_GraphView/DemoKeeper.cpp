/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "GraphNodeEventController.h"
#include "GraphNodePositionController.h"
#include "GraphNodeWeightController.h"
#include "GraphNodeLoopController.h"
#include "GraphNodeFadeController.h"
#include "GraphNodeGroup2Controller.h"
#include "GraphNodeSkeletonState.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager(),
		mGraphView(nullptr),
		mGraph(nullptr)
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

		mGraph = new animation::AnimationGraph();
		mGraph->addData("OwnerEntity", Ogre::Any(entity));

		createGrapView();
	}

	void DemoKeeper::notifyMenuCtrlAccept(wraps::BaseGraphView* _sender, const std::string& _id)
	{
		std::string name = _id;
		size_t index = name.find("Controller");
		if (index != -1) name.erase(index);
		else
		{
			index = name.find("State");
			if (index != -1) name.erase(index);
		}

		BaseAnimationNode* node = mGraphNodeFactory.createNode("GraphNode" + _id, name);
		mGraphView->addItem(node);
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition();
		node->setAbsolutePosition(point);

		animation::IAnimationNode* anim_node = mNodeFactory.createNode(_id, MyGUI::utility::toString(name, "_", (int)node), mGraph);
		mGraph->addNode(anim_node);
		node->setAnimationNode(anim_node);

		node->eventInvalidateNode = MyGUI::newDelegate(this, &DemoKeeper::notifyInvalidateNode);
	}

	void DemoKeeper::notifyInvalidateNode(BaseAnimationNode* _sender)
	{
		wraps::BaseGraphView::EnumeratorNode node = mGraphView->getNodeEnumerator();
		while (node.next())
		{
			BaseAnimationNode* anim_node = dynamic_cast<BaseAnimationNode*>(node.current());
			if (anim_node)
			{
				anim_node->invalidateNode(_sender);
			}
		}
	}

	void DemoKeeper::notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node)
	{
		BaseAnimationNode* node = dynamic_cast<BaseAnimationNode*>(_node);
		node->getAnimationNode()->getGraph()->removeNode(node->getAnimationNode());
		animation::IAnimationNode* anim_node = node->getAnimationNode();
		_sender->removeItem(_node);
		delete _node;
		delete anim_node;
	}

	void DemoKeeper::notifyConnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
	{
		BaseAnimationNode* node_from = dynamic_cast<BaseAnimationNode*>(_from->getOwnerNode());
		const std::string& name_from = _from->getConnectionName();
		BaseAnimationNode* node_to = dynamic_cast<BaseAnimationNode*>(_to->getOwnerNode());
		const std::string& name_to = _to->getConnectionName();

		node_from->getAnimationNode()->addConnection(name_from, node_to->getAnimationNode(), name_to);
		node_from->addConnection(name_from, node_to, name_to);
	}

	void DemoKeeper::notifyDisconnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
	{
		BaseAnimationNode* node_from = dynamic_cast<BaseAnimationNode*>(_from->getOwnerNode());
		const std::string& name_from = _from->getConnectionName();
		BaseAnimationNode* node_to = dynamic_cast<BaseAnimationNode*>(_to->getOwnerNode());
		const std::string& name_to = _to->getConnectionName();

		node_from->removeConnection(name_from, node_to, name_to);
		node_from->getAnimationNode()->removeConnection(name_from, node_to->getAnimationNode(), name_to);
	}

	void DemoKeeper::createGrapView()
	{
		mGraphView = new GraphView();
		mGraphView->requestConnectPoint = MyGUI::newDelegate(requestConnectPoint);
		mGraphView->requestDisconnectPoint = MyGUI::newDelegate(requestDisconnectPoint);
		mGraphView->eventConnectPoint = MyGUI::newDelegate(this, &DemoKeeper::notifyConnectPoint);
		mGraphView->eventDisconnectPoint = MyGUI::newDelegate(this, &DemoKeeper::notifyDisconnectPoint);

		mGraphView->eventMenuCtrlAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyMenuCtrlAccept);
		mGraphView->eventNodeClosed = MyGUI::newDelegate(this, &DemoKeeper::notifyNodeClosed);

		mGraphView->addMenuItem("EventController", "EventController");
		mGraphView->addMenuItem("WeightController", "WeightController");
		mGraphView->addMenuItem("PositionController", "PositionController");
		mGraphView->addMenuItem("LoopController", "LoopController");
		mGraphView->addMenuItem("FadeController", "FadeController");
		mGraphView->addMenuItem("Group2Controller", "Group2Controller");
		mGraphView->addMenuItem("SkeletonState", "SkeletonState");
	}

    void DemoKeeper::destroyScene()
    {
		delete mGraphView;
    }

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		mGraph->addTime(evt.timeSinceLastFrame);
		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
