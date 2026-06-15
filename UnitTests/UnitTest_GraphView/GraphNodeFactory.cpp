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
		for (auto& mConstruct : mConstructs)
			delete mConstruct.second;
		mConstructs.clear();
	}

	BaseAnimationNode* GraphNodeFactory::createNode(std::string_view _type, std::string_view _name)
	{
		MapConstruct::iterator item = mConstructs.find(_type);
		if (item == mConstructs.end())
			return nullptr;
		return (*item).second->create(_name);
	}

	void GraphNodeFactory::addConstruct(std::string_view _type, IGraphNodeConstruct* _construct)
	{
		MyGUI::mapSet(mConstructs, _type, _construct);
	}

} // namespace demo
