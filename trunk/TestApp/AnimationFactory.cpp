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
#include "RandomController5.h"

namespace anim
{
	IAnimationState * AnimationFactory::createState(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node)
	{
		IAnimationState * state = 0;
		std::string type = _node->findAttribute("type");
		if (type == "SkeletonState") {
			state = new SkeletonState(_owner, _node);
		}
		else if (type == "ParticleState") {
			state = new ParticleState(_owner, _node);
		}
		else if (type == "SoundState") {
			state = new SoundState(_owner, _node);
		}

		MYGUI_ASSERT(state, "state type'" << type << "' not found");
		return state;
	}

	IAnimationController * AnimationFactory::createController(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node, const VectorState& _states)
	{
		IAnimationController * controller = 0;
		std::string type = _node->findAttribute("type");
		if (type == "LoopController") {
			controller = new LoopController(_owner, _node, _states);
		}
		else if (type == "FadeController") {
			controller = new FadeController(_owner, _node, _states);
		}
		else if (type == "RandomController5") {
			controller = new RandomController5(_owner, _node, _states);
		}

		MYGUI_ASSERT(controller, "controller type'" << type << "' not found");
		return controller;
	}

	float AnimationFactory::getTime(const std::string& _value, const VectorState& _states)
	{
		if (!_value.empty()) {
			if (_value[0] == '#') {
				std::string value = _value.substr(1);
				for (size_t index=0; index<_states.size(); ++index) {
					if (_states[index]->getName() == value) {
						return _states[index]->getLength();
					}
				}
			}
			else {
				return MyGUI::utility::parseFloat(_value);
			}
		}
		return 0;
	}

} // namespace anim