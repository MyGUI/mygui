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
		for (auto& mConstruct : mConstructs)
			delete mConstruct.second;
		mConstructs.clear();
	}

	IAnimationNode* AnimationNodeFactory::createNode(
		std::string_view _type,
		std::string_view _name,
		IAnimationGraph* _holder)
	{
		MapConstruct::iterator item = mConstructs.find(_type);
		if (item == mConstructs.end())
			return nullptr;
		return (*item).second->create(_name, _holder);
	}

	void AnimationNodeFactory::addConstruct(std::string_view _type, IAnimationNodeConstruct* _construct)
	{
		MyGUI::mapSet(mConstructs, _type, _construct);
	}

} // namespace animation
