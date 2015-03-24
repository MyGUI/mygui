#ifndef ANIMATION_NODE_FACTORY_H_
#define ANIMATION_NODE_FACTORY_H_

#include "IAnimationNode.h"
#include "IAnimationNodeConstruct.h"
#include <map>

namespace animation
{

	class AnimationNodeFactory
	{
	public:
		AnimationNodeFactory();
		~AnimationNodeFactory();

		IAnimationNode* createNode(const std::string& _type, const std::string& _name, IAnimationGraph* _holder);

		void addConstruct(const std::string& _type, IAnimationNodeConstruct* _construct);

	private:
		typedef std::map<std::string, IAnimationNodeConstruct*> MapConstruct;
		MapConstruct mConstructs;

	};

} // namespace animation

#endif // ANIMATION_NODE_FACTORY_H_
