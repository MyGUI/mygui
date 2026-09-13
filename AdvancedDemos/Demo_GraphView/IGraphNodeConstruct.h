#ifndef GRAPH_NODE_CONSTRUCT_H_
#define GRAPH_NODE_CONSTRUCT_H_

#include "BaseAnimationNode.h"

namespace demo
{

	class IGraphNodeConstruct
	{
	public:
		virtual ~IGraphNodeConstruct() = default;
		virtual BaseAnimationNode* create(std::string_view _name) = 0;
	};

	template<typename Type>
	class GraphNodeConstruct : public IGraphNodeConstruct
	{
	public:
		BaseAnimationNode* create(std::string_view _name) override
		{
			return new Type(_name);
		}
	};

}

#endif // GRAPH_NODE_CONSTRUCT_H_
