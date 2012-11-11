/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/

#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "GraphNodeEventController.h"
#include "GraphNodePositionController.h"
#include "GraphNodeWeightController.h"
#include "GraphNodeLoopController.h"
#include "GraphNodeFadeController.h"
#include "GraphNodeGroup2Controller.h"
#include "GraphNodeSkeletonState.h"
#include "DialogManager.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mGraphView(nullptr),
		mGraph(nullptr),
		mFileDialog(nullptr),
		mFileDialogSave(false),
		mContextMenu(nullptr)
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
			if (isConnectionOut(_from->getType()))
			{
				_result = true;
			}
		}
		else
		{
			if (
				_from != _to
				&& isConnectionTypeSimple(_from->getType()) == isConnectionTypeSimple(_to->getType())
				&& isConnectionOut(_from->getType()) && !isConnectionOut(_to->getType())
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
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_GraphView");
		addResourceLocation(getRootMedia() + "/Common/Tools");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
		MyGUI::ResourceManager::getInstance().load("SplineSkin.xml");
		MyGUI::ResourceManager::getInstance().load("GraphNodeSkin.xml");

		new tools::DialogManager();
		tools::DialogManager::getInstance().initialise();

		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* entity = getSceneManager()->createEntity("Object", "Robot.mesh");
		node->attachObject(entity);
		getCamera()->setPosition(400, 400, 400);

		mFileDialog = new tools::OpenSaveFileDialog();
		mFileDialog->eventEndDialog = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDialog);

		mGraph = new animation::AnimationGraph();
		mGraph->addData("OwnerEntity", Ogre::Any(entity));

		createGrapView();

		mContextMenu = new wraps::ContextMenu("ContextMenu.layout");
		mContextMenu->eventMenuAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyMenuCtrlAccept);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);

		for (VectorBaseAnimationNode::iterator item = mNodes.begin(); item != mNodes.end(); ++ item)
		{
			animation::IAnimationNode* anim_node = (*item)->getAnimationNode();
			delete anim_node;
			delete (*item);
		}
		mNodes.clear();

		delete mFileDialog;
		mFileDialog = nullptr;

		delete mGraphView;
		mGraphView = nullptr;

		delete mGraph;
		mGraph = nullptr;

		delete mContextMenu;
		mContextMenu = nullptr;

		tools::DialogManager::getInstance().shutdown();
		delete tools::DialogManager::getInstancePtr();
	}

	void DemoKeeper::notifyMenuCtrlAccept(wraps::ContextMenu* _sender, const std::string& _id)
	{
		if (_id == "SaveGraph")
		{
			SaveGraph();
			return;
		}
		else if (_id == "LoadGraph")
		{
			LoadGraph();
			return;
		}
		else if (_id == "ClearGraph")
		{
			ClearGraph();
			return;
		}

		std::string name = _id;
		size_t index = name.find("Controller");
		if (index != MyGUI::ITEM_NONE) name.erase(index);
		else
		{
			index = name.find("State");
			if (index != MyGUI::ITEM_NONE) name.erase(index);
		}

		static size_t name_index = 0;
		name_index++;
		name = MyGUI::utility::toString(name, "_", name_index);

		createNode(_id, name);
	}

	BaseAnimationNode* DemoKeeper::createNode(const std::string& _type, const std::string& _name)
	{
		BaseAnimationNode* node = mGraphNodeFactory.createNode("GraphNode" + _type, _name);
		assert(node);

		mNodes.push_back(node);

		mGraphView->addItem(node);
		node->setAbsolutePosition(mClickPosition);

		animation::IAnimationNode* anim_node = mNodeFactory.createNode(_type, _name, mGraph);
		mGraph->addNode(anim_node);
		node->setAnimationNode(anim_node);

		node->eventInvalidateNode = MyGUI::newDelegate(this, &DemoKeeper::notifyInvalidateNode);

		return node;
	}

	void DemoKeeper::SaveGraph()
	{
		mFileDialogSave = true;
		mFileDialog->setDialogInfo("Save as ...", "Save");
		mFileDialog->doModal();
	}

	void DemoKeeper::LoadGraph()
	{
		mFileDialogSave = false;
		mFileDialog->setDialogInfo("Load", "Load");
		mFileDialog->doModal();
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
		delete anim_node;

		mNodes.erase(std::remove(mNodes.begin(), mNodes.end(), _node), mNodes.end());
		delete _node;
	}

	void DemoKeeper::notifyConnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
	{
		BaseAnimationNode* node_from = dynamic_cast<BaseAnimationNode*>(_from->getOwnerNode());
		const std::string& name_from = _from->getName();
		BaseAnimationNode* node_to = dynamic_cast<BaseAnimationNode*>(_to->getOwnerNode());
		const std::string& name_to = _to->getName();

		connectPoints(node_from, node_to, name_from, name_to);
	}

	void DemoKeeper::notifyDisconnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
	{
		BaseAnimationNode* node_from = dynamic_cast<BaseAnimationNode*>(_from->getOwnerNode());
		const std::string& name_from = _from->getName();
		BaseAnimationNode* node_to = dynamic_cast<BaseAnimationNode*>(_to->getOwnerNode());
		const std::string& name_to = _to->getName();

		disconnectPoints(node_from, node_to, name_from, name_to);
	}

	void DemoKeeper::createGrapView()
	{
		mGraphView = new GraphView();
		mGraphView->requestConnectPoint = MyGUI::newDelegate(requestConnectPoint);
		mGraphView->requestDisconnectPoint = MyGUI::newDelegate(requestDisconnectPoint);
		mGraphView->eventConnectPoint = MyGUI::newDelegate(this, &DemoKeeper::notifyConnectPoint);
		mGraphView->eventDisconnectPoint = MyGUI::newDelegate(this, &DemoKeeper::notifyDisconnectPoint);

		mGraphView->eventNodeClosed = MyGUI::newDelegate(this, &DemoKeeper::notifyNodeClosed);
		mGraphView->getClient()->eventMouseButtonReleased += MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
	}

	void DemoKeeper::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
		{
			mClickPosition.left = _left;
			mClickPosition.top = _top;
			mContextMenu->setVisible(true);
		}
	}

	void DemoKeeper::notifyFrameStarted(float _time)
	{
		mGraph->addTime(_time);
	}

	void DemoKeeper::notifyEndDialog(tools::Dialog* _dialog, bool _result)
	{
		if (_result)
		{
			if (mFileDialogSave)
			{
				std::string filename = mFileDialog->getFileName();
				size_t index = filename.find_first_of('.');
				if (index == std::string::npos)
					filename += ".xml";
				filename = mFileDialog->getCurrentFolder() + "/" + filename;

				saveToFile(filename);
			}
			else
			{
				ClearGraph();

				std::string filename = mFileDialog->getFileName();
				filename = mFileDialog->getCurrentFolder() + "/" + filename;

				loadFromFile(filename);
			}
		}

		_dialog->endModal();
	}

	void DemoKeeper::ClearGraph()
	{
		mGraphView->removeAllItems();
	}

	void DemoKeeper::saveToFile(const std::string& _filename)
	{
		MyGUI::xml::Document doc;

		// есть такой файл
		if (!doc.open(_filename))
		{
			doc.clear();
		}

		doc.createDeclaration();
		MyGUI::xml::ElementPtr root = doc.createRoot("AnimationGraph");

		// сохраняем сами ноды
		wraps::BaseGraphView::EnumeratorNode node = mGraphView->getNodeEnumerator();
		while (node.next())
		{
			BaseAnimationNode* anim_node = dynamic_cast<BaseAnimationNode*>(node.current());
			if (anim_node)
			{
				MyGUI::xml::ElementPtr node_type = root->createChild("Node");
				node_type->addAttribute("type", anim_node->getType());
				node_type->addAttribute("name", anim_node->getName());
				anim_node->serialization(node_type);
			}
		}

		// сохраняем соединения
		node = mGraphView->getNodeEnumerator();
		while (node.next())
		{
			BaseAnimationNode* anim_node = dynamic_cast<BaseAnimationNode*>(node.current());
			if (anim_node && anim_node->isAnyConnection())
			{
				MyGUI::xml::ElementPtr connection = root->createChild("Connections");
				connection->addAttribute("node", anim_node->getName());

				wraps::EnumeratorConnection node_conn = anim_node->getConnectionEnumerator();
				while (node_conn.next())
				{
					wraps::EnumeratorConnection conn = node_conn->getConnectionEnumerator();
					while (conn.next())
					{
						BaseAnimationNode* anim_node2 = dynamic_cast<BaseAnimationNode*>(conn->getOwnerNode());
						if (anim_node2)
						{
							MyGUI::xml::ElementPtr item = connection->createChild("Connection");
							item->addAttribute("node", anim_node2->getName());
							item->addAttribute("from", node_conn->getName());
							item->addAttribute("to", conn->getName());
						}
					}
				}
			}
		}

		// сохраняем данные для редактора
		MyGUI::xml::ElementPtr data = root->createChild("EditorData");
		node = mGraphView->getNodeEnumerator();
		while (node.next())
		{
			BaseAnimationNode* anim_node = dynamic_cast<BaseAnimationNode*>(node.current());
			if (anim_node)
			{
				MyGUI::xml::ElementPtr item_data = data->createChild("Node");
				item_data->addAttribute("name", anim_node->getName());
				item_data->addAttribute("coord", anim_node->getCoord().print());
			}
		}

		doc.save(_filename);
	}

	void DemoKeeper::loadFromFile(const std::string& _filename)
	{
		MyGUI::xml::Document doc;

		if (!doc.open(_filename))
			return;

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "AnimationGraph")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Node")
			{
				BaseAnimationNode* anim_node = createNode(node->findAttribute("type"), node->findAttribute("name"));
				anim_node->deserialization(node.current());
			}
			else if (node->getName() == "Connections")
			{
				MyGUI::xml::ElementEnumerator conn = node->getElementEnumerator();
				BaseAnimationNode* anim_node = getNodeByName(node.current()->findAttribute("node"));
				if (anim_node)
				{
					while (conn.next("Connection"))
					{
						BaseAnimationNode* anim_node2 = getNodeByName(conn.current()->findAttribute("node"));
						if (anim_node2)
						{
							//соединить точки в ноде
							const std::string& from_point = conn.current()->findAttribute("from");
							const std::string& to_point = conn.current()->findAttribute("to");

							wraps::BaseGraphConnection* from_conn = anim_node->getConnectionByName(from_point, "EventOut");
							if (!from_conn) from_conn = anim_node->getConnectionByName(from_point, "PositionOut");
							if (!from_conn) from_conn = anim_node->getConnectionByName(from_point, "WeightOut");

							wraps::BaseGraphConnection* to_conn = anim_node2->getConnectionByName(to_point, "EventIn");
							if (!to_conn) to_conn = anim_node2->getConnectionByName(to_point, "PositionIn");
							if (!to_conn) to_conn = anim_node2->getConnectionByName(to_point, "WeightIn");

							if (from_conn && to_conn)
							{
								from_conn->addConnectionPoint(to_conn);
								connectPoints(anim_node, anim_node2, from_point, to_point);
							}
						}
					}
				}
			}
			else if (node->getName() == "EditorData")
			{
				MyGUI::xml::ElementEnumerator item_data = node->getElementEnumerator();
				while (item_data.next("Node"))
				{
					BaseAnimationNode* anim_node = getNodeByName(item_data.current()->findAttribute("name"));
					if (anim_node)
					{
						anim_node->setCoord(MyGUI::IntCoord::parse(item_data.current()->findAttribute("coord")));
					}
				}
			}
		}

	}

	BaseAnimationNode* DemoKeeper::getNodeByName(const std::string& _name)
	{
		wraps::BaseGraphView::EnumeratorNode node = mGraphView->getNodeEnumerator();
		while (node.next())
		{
			BaseAnimationNode* anim_node = dynamic_cast<BaseAnimationNode*>(node.current());
			if (anim_node && anim_node->getName() == _name)
			{
				return anim_node;
			}
		}
		return nullptr;
	}

	void DemoKeeper::connectPoints(BaseAnimationNode* _node_from, BaseAnimationNode* _node_to, const std::string& _name_from, const std::string& _name_to)
	{
		_node_from->getAnimationNode()->addConnection(_name_from, _node_to->getAnimationNode(), _name_to);
		_node_from->addConnection(_name_from, _node_to, _name_to);
	}

	void DemoKeeper::disconnectPoints(BaseAnimationNode* _node_from, BaseAnimationNode* _node_to, const std::string& _name_from, const std::string& _name_to)
	{
		_node_from->removeConnection(_name_from, _node_to, _name_to);
		_node_from->getAnimationNode()->removeConnection(_name_from, _node_to->getAnimationNode(), _name_to);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
