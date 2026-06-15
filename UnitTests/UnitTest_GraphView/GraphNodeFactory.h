#ifndef GRAPH_NODE_FACTORY_H_
#define GRAPH_NODE_FACTORY_H_

#include "IGraphNodeConstruct.h"

namespace demo
{

	class GraphNodeFactory
	{
	public:
		GraphNodeFactory();
		~GraphNodeFactory();

		BaseAnimationNode* createNode(std::string_view _type, std::string_view _name);

		void addConstruct(std::string_view _type, IGraphNodeConstruct* _construct);

	private:
		using MapConstruct = std::map<std::string, IGraphNodeConstruct*, std::less<>>;
		MapConstruct mConstructs;
	};

} // namespace demo

#endif // GRAPH_NODE_FACTORY_H_
