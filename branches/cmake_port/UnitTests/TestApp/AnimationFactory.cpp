/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#include "precompiled.h"
#include "AnimationFactory.h"

#include "SkeletonState.h"
#include "ParticleState.h"
#include "SoundState.h"
#include "LoopController.h"
#include "FadeController.h"
#include "RandomController.h"

namespace anim
{
	IAnimationState * AnimationFactory::createState(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node)
	{
		IAnimationState * state = 0;
		std::string type = _node->findAttribute("type");
		if (type == "SkeletonState") {
			state = new SkeletonState(_parent, _owner, _node);
		}
		else if (type == "ParticleState") {
			state = new ParticleState(_parent, _owner, _node);
		}
		else if (type == "SoundState") {
			state = new SoundState(_parent, _owner, _node);
		}

		MYGUI_ASSERT(state, "state type'" << type << "' not found");
		return state;
	}

	IAnimationController * AnimationFactory::createController(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node)
	{
		IAnimationController * controller = 0;
		std::string type = _node->findAttribute("type");
		if (type == "LoopController") {
			controller = new LoopController(_parent, _owner, _node);
		}
		else if (type == "FadeController") {
			controller = new FadeController(_parent, _owner, _node);
		}
		else if (type == "RandomController") {
			controller = new RandomController(_parent, _owner, _node);
		}

		MYGUI_ASSERT(controller, "controller type'" << type << "' not found");
		return controller;
	}

} // namespace anim