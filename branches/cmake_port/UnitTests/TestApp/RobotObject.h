/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __ROBOT_OBJECT_H__
#define __ROBOT_OBJECT_H__

#include "IBase.h"
#include "IAnimationLink.h"

namespace sim
{

	class RobotObject : public anim::IAnimationLink,  public IBase
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(RobotObject, IBase);

		RobotObject(const std::string& _name) :
			mName(_name),
			mWalk(false),
			mKinematical(0)
		{
		}

		virtual const std::string& getName() { return mName; }

		virtual ~RobotObject() { }

		void Fire()
		{
		}

		void update(float _time)
		{
			bool moving = mKinematical->IsMoving();
			if (mWalk != moving) {
				mWalk = moving;
				float time = context::TimeContext::getCurrentTime();
				if (mWalk) eventExitEventStart2(time);
				else eventExitEventStart1(time);
			}
		}

		virtual void initialise()
		{
			AnimatebleObject * anim = this->queryType<AnimatebleObject>();
			anim->addExternalLink(this);
			anim::IAnimationGraph * graph1 = anim->createAnimationGraph("anim1.xml");
			anim->startGraph(graph1);

			float time = context::TimeContext::getCurrentTime();
			eventExitEventStart1(time);

			mKinematical = this->queryType<IKinematicalObject>();

			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &RobotObject::update);
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

		anim::DelegateLinkEvent eventExitEventStart1;
		anim::DelegateLinkEvent eventExitEventStop1;
		anim::DelegateLinkEvent eventExitEventStart2;
		anim::DelegateLinkEvent eventExitEventStop2;
		anim::DelegateLinkEvent eventExitEventStart3;
		anim::DelegateLinkEvent eventExitEventStop3;

	private:
		bool mWalk;
		std::string mName;
		IKinematicalObject * mKinematical;
	};

} // namespace sim

#endif // __ROBOT_OBJECT_H__
