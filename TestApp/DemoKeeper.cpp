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

// это выдрано из ControllerPosition.cpp
inline void MoveFunction(const MyGUI::IntCoord & _startRect, const MyGUI::IntCoord & _destRect, MyGUI::IntCoord & _result, float _k){
	_result.set(_startRect.left   - int( float(_startRect.left   - _destRect.left)   * _k ),
				_startRect.top    - int( float(_startRect.top    - _destRect.top)    * _k ),
				_startRect.width  - int( float(_startRect.width  - _destRect.width)  * _k ),
				_startRect.height - int( float(_startRect.height - _destRect.height) * _k )
	);
};

// это выдрано из ControllerPosition.cpp
template <int N>
void JumpFunction(const MyGUI::IntCoord & _startRect, const MyGUI::IntCoord & _destRect, MyGUI::IntCoord & _result, float _current_time)
{
	float k = pow (_current_time, 2) * (-2 - N/10.f) + _current_time * (3 + N/10.f);
	MoveFunction(_startRect, _destRect, _result, k);
}

// это выдрано из ControllerPosition.cpp
typedef MyGUI::delegates::IDelegate4<const MyGUI::IntCoord&, const MyGUI::IntCoord&, MyGUI::IntCoord&, float> * FrameAction;

    void DemoKeeper::createScene()
    {

		MyGUI::WidgetPtr widget1 = mGUI->createWidget<MyGUI::Widget>("WindowCSX", MyGUI::IntCoord(20, 20, 150, 100), MyGUI::Align::Default, "Overlapped");

		MyGUI::IntCoord coord(200, 200, 150, 100);
		MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(coord, 2, MyGUI::newDelegate(JumpFunction<10>));
		MyGUI::ControllerManager::getInstance().addItem(widget1, controller);

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
