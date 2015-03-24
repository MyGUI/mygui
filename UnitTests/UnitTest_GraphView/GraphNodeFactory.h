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

		BaseAnimationNode* createNode(const std::string& _type, const std::string& _name);

		void addConstruct(const std::string& _type, IGraphNodeConstruct* _construct);

	private:
		typedef std::map<std::string, IGraphNodeConstruct*> MapConstruct;
		MapConstruct mConstructs;

	};

} // namespace demo

#endif // GRAPH_NODE_FACTORY_H_
