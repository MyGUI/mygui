/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef I_ANIMATION_GRAPH_H_
#define I_ANIMATION_GRAPH_H_

#include <Ogre.h>
#include <string>
#include "IAnimationNode.h"

namespace animation
{

	class IAnimationGraph :
		public IAnimationNode
	{
	public:
		IAnimationGraph() : IAnimationNode() { }
		IAnimationGraph(const std::string& _name) : IAnimationNode(_name, 0) { }
		virtual ~IAnimationGraph() { }

		virtual Ogre::Any getData(const std::string& _name) = 0;
		virtual void addData(const std::string& _name, Ogre::Any _any) = 0;

		virtual void addNode(IAnimationNode* _node) = 0;
		virtual void removeNode(IAnimationNode* _node) = 0;
		virtual IAnimationNode* getNodeByName(const std::string& _name) = 0;
	};

} // namespace animation

#endif // I_DATA_DICTONARY_H_
