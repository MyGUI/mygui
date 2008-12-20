/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

#include "LoopController.h"
#include "FadeController.h"
#include "SkeletonState.h"

#include "RenderableObject.h"
#include "AnimatebleObject.h"

namespace demo
{

	class MainController : public anim::IAnimationLink
	{
	public:
		MainController(const std::string& _name) :
			mName(_name)
		{
		}
		virtual const std::string& getName() { return mName; }

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
		virtual ~MainController() { }

		anim::DelegateLinkEvent eventExitEventStart1;
		anim::DelegateLinkEvent eventExitEventStop1;
		anim::DelegateLinkEvent eventExitEventStart2;
		anim::DelegateLinkEvent eventExitEventStop2;
		anim::DelegateLinkEvent eventExitEventStart3;
		anim::DelegateLinkEvent eventExitEventStop3;

	private:
		std::string mName;
	};

	MainController * main_controller = 0;

    void DemoKeeper::createScene()
    {

		// главня частьдля удобства
		sim::IBase * object = 0;
		// создаем ентити
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// создаем анимационную часть
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();

		// агрегируем части
		object = rend;
		object->addBase(anim);

		// проверка агрегации
		rend = object->queryType<sim::RenderableObject>();
		anim = object->queryType<sim::AnimatebleObject>();

		main_controller = new MainController("c_Main");
		anim->addExternalLink(main_controller);

		sim::AnimationGraph * graph1 = anim->createAnimationGraph("anim1.xml");
		anim->startGraph(graph1);

	}
 
    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		float time = context::TimeContext::getCurrentTime();

		if (arg.key == OIS::KC_1) main_controller->eventExitEventStart1(time);
		else if (arg.key == OIS::KC_2) main_controller->eventExitEventStop1(time);
		else if (arg.key == OIS::KC_3) main_controller->eventExitEventStart2(time);
		else if (arg.key == OIS::KC_4) main_controller->eventExitEventStop2(time);
		else if (arg.key == OIS::KC_5) main_controller->eventExitEventStart3(time);
		else if (arg.key == OIS::KC_6) main_controller->eventExitEventStop3(time);

		return BaseManager::keyPressed(arg);
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		return BaseManager::frameStarted(evt);
	}
	 
} // namespace demo
