/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_ANIMATION_STATE_H__
#define __I_ANIMATION_STATE_H__

#include "IAnimationLink.h"

namespace anim
{

	class IAnimationState : public IAnimationLink
	{
	public:
		virtual ~IAnimationState() { }
		virtual float getLength() = 0;
	};

	typedef std::vector<anim::IAnimationState*> VectorState;

} // namespace anim

#endif // __I_ANIMATION_STATE_H__
