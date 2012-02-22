#ifndef __GRAPH_NODE_FACTORY_H__
#define __GRAPH_NODE_FACTORY_H__

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

#endif // __GRAPH_NODE_FACTORY_H__
