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

    void DemoKeeper::createScene()
    {

		// ������ �������� ��������
		sim::IBase * object = 0;
		// ������� ������
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// ������� ������������ �����
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();
		// ������� �������������� �����
		mKinematical = new sim::KinematicalObject("c_Main");
		// ������� ����� ��� ��������
		mAbility = new sim::AbilityObject("c_Ability");

		// ���������� �����
		object = rend;
		object->addBase(anim);
		object->addBase(mKinematical);
		object->addBase(mAbility);

		// ����� ��������� �������������
		object->initialiseBase();

		// �������� ���������
		rend = object->queryType<sim::RenderableObject>();
		anim = object->queryType<sim::AnimatebleObject>();
		mKinematical = object->queryType<sim::KinematicalObject>();
		mAbility = object->queryType<sim::AbilityObject>();


		MyGUI::ButtonPtr button1 = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 100, 26), MyGUI::Align::Default, "Main");
		button1->setCaption(L"������1");
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
		// ������ �������� ��������
		sim::IBase * object = 0;
		// ������� ������
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// ������� ������������ �����
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();
		// ������� �������������� �����
		sim::KinematicalObject * kin = new sim::KinematicalObject("c_Main");
		// ������� ����� ��� ��������
		sim::AbilityObject * ability = new sim::AbilityObject("c_Ability");

		// ���������� �����
		object = rend;
		object->addBase(anim);
		object->addBase(kin);
		object->addBase(ability);

		// ����� ��������� �������������
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
