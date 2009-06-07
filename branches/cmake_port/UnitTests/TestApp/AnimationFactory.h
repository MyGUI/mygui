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
#include "IAnimationGraph.h"
#include "IAnimationState.h"
#include "IAnimationController.h"

namespace anim
{
	class AnimationFactory
	{
	public:
		static IAnimationState * createState(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node);
		static IAnimationController * createController(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node);
	};
}

#endif // __ANIMATION_FACTORY_H__
