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

	}
 
    void DemoKeeper::destroyScene()
    {
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
		return BaseManager::frameStarted(evt);
	}
	 
} // namespace demo
