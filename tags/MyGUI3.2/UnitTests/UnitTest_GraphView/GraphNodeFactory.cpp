#include "GraphNodeFactory.h"
#include "GraphNodeFadeController.h"
#include "GraphNodeLoopController.h"
#include "GraphNodeGroup2Controller.h"
#include "GraphNodeEventController.h"
#include "GraphNodeWeightController.h"
#include "GraphNodePositionController.h"
#include "GraphNodeSkeletonState.h"

namespace demo
{

	GraphNodeFactory::GraphNodeFactory()
	{
		addConstruct("GraphNodeFadeController", new GraphNodeConstruct<GraphNodeFadeController>());
		addConstruct("GraphNodeLoopController", new GraphNodeConstruct<GraphNodeLoopController>());
		addConstruct("GraphNodeGroup2Controller", new GraphNodeConstruct<GraphNodeGroup2Controller>());
		addConstruct("GraphNodeEventController", new GraphNodeConstruct<GraphNodeEventController>());
		addConstruct("GraphNodeWeightController", new GraphNodeConstruct<GraphNodeWeightController>());
		addConstruct("GraphNodePositionController", new GraphNodeConstruct<GraphNodePositionController>());
		addConstruct("GraphNodeSkeletonState", new GraphNodeConstruct<GraphNodeSkeletonState>());
	}

	GraphNodeFactory::~GraphNodeFactory()
	{
		for (MapConstruct::iterator item = mConstructs.begin(); item != mConstructs.end(); ++ item)
			delete (*item).second;
		mConstructs.clear();
	}

	BaseAnimationNode* GraphNodeFactory::createNode(const std::string& _type, const std::string& _name)
	{
		MapConstruct::iterator item = mConstructs.find(_type);
		if (item == mConstructs.end())
			return nullptr;
		return (*item).second->create(_name);
	}

	void GraphNodeFactory::addConstruct(const std::string& _type, IGraphNodeConstruct* _construct)
	{
		mConstructs[_type] = _construct;
	}

} // namespace demo
