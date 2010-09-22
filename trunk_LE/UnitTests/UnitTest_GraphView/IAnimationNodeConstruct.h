#ifndef __I_ANIMATION_NODE_CONSTRUCT_H__
#define __I_ANIMATION_NODE_CONSTRUCT_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class IAnimationNodeConstruct
	{
	public:
		IAnimationNodeConstruct() { }
		virtual ~IAnimationNodeConstruct() { }
		virtual IAnimationNode* create(const std::string& _name, IAnimationGraph* _holder) = 0;
	};

	template <typename Type>
	class AnimationNodeConstruct :
		public IAnimationNodeConstruct
	{
	public:
		AnimationNodeConstruct() { }
		virtual ~AnimationNodeConstruct() { }
		virtual IAnimationNode* create(const std::string& _name, IAnimationGraph* _holder)
		{
			return new Type(_name, _holder);
		}
	};

} // namespace animation

#endif // __I_ANIMATION_NODE_CONSTRUCT_H__
