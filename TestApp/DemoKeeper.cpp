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

	struct ControllerType
	{
		enum EnumType { Jamp, Accelerated, Slowed, Inertional, MAX };
		ControllerType(EnumType _value = MAX) : value(_value) { }
		friend bool operator == (ControllerType const & a, ControllerType const & b) { return a.value == b.value; }
		friend bool operator != (ControllerType const & a, ControllerType const & b) { return a.value != b.value; }
	private:
		int value;
	};

    void DemoKeeper::createScene()
    {



		ControllerType type = ControllerType::Jamp;
		ControllerType type2(ControllerType::Jamp); 


		base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		//Ogre::Root::getSingleton().loadPlugin("Plugin_ParticleFX_d");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		Ogre::ParticleSystem* pSys2 = mSceneMgr->createParticleSystem("fountain1", "Examples/Smoke");
		Ogre::SceneNode* fNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        fNode->attachObject(pSys2);

		//std::string file = MyGUI::helper::getResourcePath("smoke.particle");

		//Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");

		return;

		// главня частьдля удобства
		sim::IBase * object = 0;
		// создаем ентити
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// создаем анимационную часть
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();
		// создаем кинематическую часть
		mKinematical = new sim::KinematicalObject("c_Main");
		// создаем часть для эффектов
		mAbility = new sim::AbilityObject("c_Ability");

		// агрегируем части
		object = rend;
		object->addBase(anim);
		object->addBase(mKinematical);
		object->addBase(mAbility);

		// после агрегации инициализруем
		object->initialiseBase();

		// проверка агрегации
		rend = object->queryType<sim::RenderableObject>();
		anim = object->queryType<sim::AnimatebleObject>();
		mKinematical = object->queryType<sim::KinematicalObject>();
		mAbility = object->queryType<sim::AbilityObject>();


		MyGUI::ButtonPtr button1 = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 100, 26), MyGUI::Align::Default, "Main");
		button1->setCaption(L"абилко1");
		button1->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
		button1->setUserString("AbilityType", "Ability1");

		createBot(Ogre::Vector3(-200, 0, 0));
		createBot(Ogre::Vector3(0, 0, -200));
	}

    void DemoKeeper::destroyScene()
    {
    }

	void DemoKeeper::createBot(const Ogre::Vector3& _position)
	{
		// главня частьдля удобства
		sim::IBase * object = 0;
		// создаем ентити
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// создаем анимационную часть
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();
		// создаем кинематическую часть
		sim::KinematicalObject * kin = new sim::KinematicalObject("c_Main");
		// создаем часть для эффектов
		sim::AbilityObject * ability = new sim::AbilityObject("c_Ability");

		// агрегируем части
		object = rend;
		object->addBase(anim);
		object->addBase(kin);
		object->addBase(ability);

		// после агрегации инициализруем
		object->initialiseBase();

		rend->getNode()->setPosition(_position);

		mSimulationObjects.push_back(object);
	}
 
	void DemoKeeper::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		mAbility->notifyAbility(_sender->getUserString("AbilityType"));
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		mKinematical->keyPressed(arg);
		return BaseManager::keyPressed(arg);
	}

	bool DemoKeeper::keyReleased( const OIS::KeyEvent &arg )
	{
		mKinematical->keyReleased(arg);
		return BaseManager::keyReleased(arg);
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		return true;
		static float current_time = 0;
		current_time += evt.timeSinceLastFrame;

		if (current_time > 1) {
			current_time = 0;


			float time = context::TimeContext::getCurrentTime();
			int index = rand() % 2;
			int num = rand() % 7;
			if (num == 0) {
				sim::KinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::KinematicalObject>();
				kin->eventExitEventStart1(time);
			}
			else if (num == 1) {
				sim::KinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::KinematicalObject>();
				kin->eventExitEventStart2(time);
			}
			else if (num == 2) {
				sim::KinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::KinematicalObject>();
				kin->eventExitEventStart3(time);
			}

		}

		return BaseManager::frameStarted(evt);
	}
	 
} // namespace demo
