/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __PARTICLE_STATE_H__
#define __PARTICLE_STATE_H__

#include <Ogre.h>
#include "IBase.h"
#include "IRenderableObject.h"
#include "IAnimationState.h"

namespace anim
{

	class ParticleState : public anim::IAnimationState
	{
	public:
		ParticleState(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node)
		{
			mName = _node->findAttribute("id");
			std::string particle = _node->findAttribute("particle");

			sim::IRenderableObject * rend = _owner->queryType<sim::IRenderableObject>();

			mParticle = Ogre::Root::getSingleton().getSceneManager("BaseSceneManager")->createParticleSystem(MyGUI::utility::toString(this, "_", particle), particle);
			mNode = rend->getNode()->createChildSceneNode();
	        mNode->attachObject(mParticle);
			// максимальное колличесво частиц
			mRate = 15;
			mParticle->getEmitter(0)->setEmissionRate(0);
		}

		virtual float getLength() { return 1; }
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
				*_delegate += MyGUI::newDelegate(this, &ParticleState::setWeight);
			}
			else if (_name == "position") {
				*_delegate += MyGUI::newDelegate(this, &ParticleState::setPosition);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}

		virtual ~ParticleState() { }

	private:
		void setPosition(float _time, float _value)
		{
		}
		void setWeight(float _time, float _value)
		{
			mParticle->getEmitter(0)->setEmissionRate(_value * mRate);
		}

	private:
		Ogre::SceneNode* mNode;
		Ogre::ParticleSystem * mParticle;
		std::string mName;
		float mRate;
	};

} // namespace anim

#endif // __PARTICLE_STATE_H__
