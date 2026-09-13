#ifndef I_ANIMATION_NODE_CONSTRUCT_H_
#define I_ANIMATION_NODE_CONSTRUCT_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class IAnimationNodeConstruct
	{
	public:
		virtual ~IAnimationNodeConstruct() = default;
		virtual IAnimationNode* create(std::string_view _name, IAnimationGraph* _holder) = 0;
	};

	template<typename Type>
	class AnimationNodeConstruct : public IAnimationNodeConstruct
	{
	public:
		IAnimationNode* create(std::string_view _name, IAnimationGraph* _holder) override
		{
			return new Type(_name, _holder);
		}
	};

} // namespace animation

#endif // I_ANIMATION_NODE_CONSTRUCT_H_
