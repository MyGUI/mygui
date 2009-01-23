/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_ANIMATION_CONTROLLER_H__
#define __I_ANIMATION_CONTROLLER_H__

#include "IAnimationLink.h"

namespace anim
{

	class IAnimationController : public IAnimationLink
	{
	public:
		virtual ~IAnimationController() { }
		virtual void update(float _time) = 0;
	};

	typedef std::vector<anim::IAnimationController*> VectorController;

} // namespace anim

#endif // __I_ANIMATION_CONTROLLER_H__
