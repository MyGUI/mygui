/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __SKELETON_STATE_H__
#define __SKELETON_STATE_H__

#include <Ogre.h>
#include "IBase.h"
#include "IRenderableObject.h"
#include "IAnimationState.h"

namespace anim
{

	class SkeletonState : public anim::IAnimationState
	{
	public:
		SkeletonState(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node)
		{
			mName = _node->findAttribute("id");
			std::string state = _node->findAttribute("state");
			sim::IRenderableObject * rend = _owner->queryType<sim::IRenderableObject>();
			m_state = rend->getEntity()->getAnimationState(state);
			m_state->setEnabled( true );
		}

		virtual float getLength() { return m_state->getLength(); }
		virtual const std::string& getName() { return mName; }

		virtual anim::DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual anim::DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual void setLinkEvent(anim::DelegateLinkEvent * _delegate, const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual void setLinkValue(anim::DelegateLinkValue * _delegate, const std::string& _name)
		{
			if (_name == "weight") {
				*_delegate += MyGUI::newDelegate(this, &SkeletonState::setWeight);
				m_state->setWeight(0);
			}
			else if (_name == "position") {
				*_delegate += MyGUI::newDelegate(this, &SkeletonState::setPosition);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}

		virtual ~SkeletonState() { }

	private:
		void setPosition(float _time, float _value) { m_state->setTimePosition(_value * m_state->getLength()); }
		void setWeight(float _time, float _value) { m_state->setWeight(_value); }

	private:
		Ogre::AnimationState * m_state;
		std::string mName;
	};

} // namespace anim

#endif // __SKELETON_STATE_H__
