#ifndef __GRAPH_NODE_CONSTRUCT_H__
#define __GRAPH_NODE_CONSTRUCT_H__

#include "BaseAnimationNode.h"

namespace demo
{

	class IGraphNodeConstruct
	{
	public:
		IGraphNodeConstruct() { }
		virtual ~IGraphNodeConstruct() { }
		virtual BaseAnimationNode* create(const std::string& _name) = 0;
	};

	template <typename Type>
	class GraphNodeConstruct :
		public IGraphNodeConstruct
	{
	public:
		GraphNodeConstruct() { }
		virtual ~GraphNodeConstruct() { }
		virtual BaseAnimationNode* create(const std::string& _name)
		{
			return new Type(_name);
		}
	};

}

#endif // __GRAPH_NODE_CONSTRUCT_H__
