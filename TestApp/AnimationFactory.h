/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __ANIMATION_FACTORY_H__
#define __ANIMATION_FACTORY_H__

#include <MyGUI.h>
#include "IBase.h"
#include "IAnimationState.h"
#include "IAnimationController.h"

namespace anim
{
	class AnimationFactory
	{
	public:
		static IAnimationState * createState(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node);
		static IAnimationController * createController(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node, const VectorState& _states);
		static float getTime(const std::string& _value, const VectorState& _states);
	};
}

#endif // __ANIMATION_FACTORY_H__
