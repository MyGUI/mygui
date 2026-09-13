/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef I_ANIMATION_GRAPH_H_
#define I_ANIMATION_GRAPH_H_

#include <any>
#include <string>
#include "IAnimationNode.h"

namespace animation
{

	class IAnimationGraph : public IAnimationNode
	{
	public:
		IAnimationGraph() :
			IAnimationNode()
		{
		}
		IAnimationGraph(std::string_view _name) :
			IAnimationNode(_name, nullptr)
		{
		}

		virtual std::any getData(std::string_view _name) = 0;
		virtual void addData(std::string_view _name, std::any _any) = 0;

		virtual void addNode(IAnimationNode* _node) = 0;
		virtual void removeNode(IAnimationNode* _node) = 0;
		virtual IAnimationNode* getNodeByName(std::string_view _name) = 0;
	};

} // namespace animation

#endif // I_DATA_DICTONARY_H_
