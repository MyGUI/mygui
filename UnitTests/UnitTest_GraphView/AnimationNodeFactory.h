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

		IAnimationNode* createNode(std::string_view _type, std::string_view _name, IAnimationGraph* _holder);

		void addConstruct(std::string_view _type, IAnimationNodeConstruct* _construct);

	private:
		using MapConstruct = std::map<std::string, IAnimationNodeConstruct*, std::less<>>;
		MapConstruct mConstructs;
	};

} // namespace animation

#endif // ANIMATION_NODE_FACTORY_H_
