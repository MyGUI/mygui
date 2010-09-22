#include "AnimationNodeFactory.h"
#include "FadeController.h"
#include "LoopController.h"
#include "Group2Controller.h"
#include "EventController.h"
#include "WeightController.h"
#include "PositionController.h"
#include "SkeletonState.h"
//#include "SoundState.h"

namespace animation
{

	AnimationNodeFactory::AnimationNodeFactory()
	{
		addConstruct("FadeController", new AnimationNodeConstruct<FadeController>());
		addConstruct("LoopController", new AnimationNodeConstruct<LoopController>());
		addConstruct("Group2Controller", new AnimationNodeConstruct<Group2Controller>());
		addConstruct("EventController", new AnimationNodeConstruct<EventController>());
		addConstruct("WeightController", new AnimationNodeConstruct<WeightController>());
		addConstruct("PositionController", new AnimationNodeConstruct<PositionController>());
		addConstruct("SkeletonState", new AnimationNodeConstruct<SkeletonState>());
		//addConstruct("SoundState", new AnimationNodeConstruct<SoundState>());
	}

	AnimationNodeFactory::~AnimationNodeFactory()
	{
		for (MapConstruct::iterator item = mConstructs.begin(); item != mConstructs.end(); ++item)
			delete (*item).second;
		mConstructs.clear();
	}

	IAnimationNode* AnimationNodeFactory::createNode(const std::string& _type, const std::string& _name, IAnimationGraph* _holder)
	{
		MapConstruct::iterator item = mConstructs.find(_type);
		if (item == mConstructs.end()) return 0;
		return (*item).second->create(_name, _holder);
	}

	void AnimationNodeFactory::addConstruct(const std::string& _type, IAnimationNodeConstruct* _construct)
	{
		mConstructs[_type] = _construct;
	}

} // namespace animation
