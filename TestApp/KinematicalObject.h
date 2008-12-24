/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __KINEMATICAL_OBJECT_H__
#define __KINEMATICAL_OBJECT_H__

#include "IBase.h"
#include "IAnimationGraph.h"
#include "TimeContext.h"
#include  "AnimatebleObject.h"

namespace sim
{

	class KinematicalObject : public anim::IAnimationLink, public IBase
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(KinematicalObject, IBase);

		KinematicalObject(const std::string& _name) :
			mName(_name),
			mWalk(false)
		{
		}
		virtual const std::string& getName() { return mName; }

		virtual void initialise()
		{
			AnimatebleObject * anim = this->queryType<AnimatebleObject>();
			anim->addExternalLink(this);
			anim::IAnimationGraph * graph1 = anim->createAnimationGraph("anim1.xml");
			anim->startGraph(graph1);

			float time = context::TimeContext::getCurrentTime();
			eventExitEventStart1(time);
		}

		virtual anim::DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			if (_name == "exit_eventStart1") {
				return &eventExitEventStart1;
			}
			else if (_name == "exit_eventStop1") {
				return &eventExitEventStop1;
			}
			if (_name == "exit_eventStart2") {
				return &eventExitEventStart2;
			}
			else if (_name == "exit_eventStop2") {
				return &eventExitEventStop2;
			}
			if (_name == "exit_eventStart3") {
				return &eventExitEventStart3;
			}
			else if (_name == "exit_eventStop3") {
				return &eventExitEventStop3;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
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
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual ~KinematicalObject() { }

		anim::DelegateLinkEvent eventExitEventStart1;
		anim::DelegateLinkEvent eventExitEventStop1;
		anim::DelegateLinkEvent eventExitEventStart2;
		anim::DelegateLinkEvent eventExitEventStop2;
		anim::DelegateLinkEvent eventExitEventStart3;
		anim::DelegateLinkEvent eventExitEventStop3;

		void keyPressed( const OIS::KeyEvent &arg )
		{
			float time = context::TimeContext::getCurrentTime();
			if (arg.key == OIS::KC_W) eventExitEventStart2(time);
			else if (arg.key == OIS::KC_SPACE) {
				//eventExitEventStop3(time);
				eventExitEventStart3(time);
			}
		}

		void keyReleased( const OIS::KeyEvent &arg )
		{
			float time = context::TimeContext::getCurrentTime();
			if (arg.key == OIS::KC_W) eventExitEventStart1(time);
		}

	private:
		std::string mName;
		bool mWalk;
	};

} // namespace sim

#endif // __KINEMATICAL_OBJECT_H__
