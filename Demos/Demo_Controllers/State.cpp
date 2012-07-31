/*!
	@file
	@author		Albert Semenov
	@date		12/2008
*/
#include "Precompiled.h"
#include "State.h"

namespace demo
{

	State::State(const std::string& _layout, ControllerType _type) :
		wraps::BaseLayout(_layout),
		mFrameAdvise(false)
	{
		mType = _type;

		assignWidget(mButton1, "Button1");
		assignWidget(mButton2, "Button2");
		assignWidget(mButton3, "Button3");
		assignWidget(mButton4, "Button4");

		mMainWidget->setVisible(false);
		mButton1->setVisible(false);
		mButton2->setVisible(false);
		mButton3->setVisible(false);
		mButton4->setVisible(false);

		mButton1->eventMouseButtonClick += MyGUI::newDelegate(this, &State::notifyMouseButtonClick);
		mButton2->eventMouseButtonClick += MyGUI::newDelegate(this, &State::notifyMouseButtonClick);
		mButton3->eventMouseButtonClick += MyGUI::newDelegate(this, &State::notifyMouseButtonClick);
		mButton4->eventMouseButtonClick += MyGUI::newDelegate(this, &State::notifyMouseButtonClick);
	}

	State::~State()
	{
		FrameAdvise(false);
	}

	void State::setVisible(bool _visible)
	{
		if (_visible)
		{
			mMainWidget->setVisible(false);
			mButton1->setVisible(false);
			mButton2->setVisible(false);
			mButton3->setVisible(false);
			mButton4->setVisible(false);

			MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);
			MyGUI::LayerManager::getInstance().upLayerItem(mButton1);
			MyGUI::LayerManager::getInstance().upLayerItem(mButton2);
			MyGUI::LayerManager::getInstance().upLayerItem(mButton3);
			MyGUI::LayerManager::getInstance().upLayerItem(mButton4);

			FrameAdvise(true);
			mCountTime = 0;
		}
		else
		{
			MyGUI::ControllerManager& manager = MyGUI::ControllerManager::getInstance();

			MyGUI::ControllerFadeAlpha* controller = createControllerFadeAlpha(0, 3, true);
			manager.addItem(mMainWidget, controller);

			controller = createControllerFadeAlpha(0, 3, true);
			manager.addItem(mButton1, controller);
			controller = createControllerFadeAlpha(0, 3, true);
			manager.addItem(mButton2, controller);
			controller = createControllerFadeAlpha(0, 3, true);
			manager.addItem(mButton3, controller);
			controller = createControllerFadeAlpha(0, 3, true);
			manager.addItem(mButton4, controller);
		}
	}

	void State::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		if (_sender == mButton1) eventButtonPress(ControllerType::Inertional, false);
		else if (_sender == mButton2) eventButtonPress(ControllerType::Accelerated, false);
		else if (_sender == mButton3) eventButtonPress(ControllerType::Slowed, false);
		else if (_sender == mButton4) eventButtonPress(ControllerType::Jump, false);
	}

	void State::notifyFrameEvent(float _time)
	{
		mCountTime += _time;

		const int offset = 30;
		const float time_diff = 0.3;
		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();

		if (!mMainWidget->getVisible())
		{
			mMainWidget->setPosition(-mMainWidget->getWidth(), view.height - mMainWidget->getHeight() - offset);
			mMainWidget->setVisible(true);
			mMainWidget->setAlpha(1);

			MyGUI::IntPoint  point(offset, view.height - mMainWidget->getHeight() - offset);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, createControllerPosition(point));
		}

		if (!mButton1->getVisible())
		{
			mButton1->setPosition(view.width, offset);
			mButton1->setVisible(true);
			mButton1->setAlpha(1);
			MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, offset);
			MyGUI::ControllerManager::getInstance().addItem(mButton1, createControllerPosition(point));
		}

		if (mCountTime > time_diff)
		{
			if (!mButton2->getVisible())
			{
				mButton2->setPosition(view.width, (mButton2->getHeight() + offset) + offset);
				mButton2->setVisible(true);
				mButton2->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, (mButton2->getHeight() + offset) + offset);
				MyGUI::ControllerManager::getInstance().addItem(mButton2, createControllerPosition(point));
			}
		}

		if (mCountTime > time_diff * 2)
		{
			if (!mButton3->getVisible())
			{
				mButton3->setPosition(view.width, (mButton3->getHeight() + offset) * 2 + offset);
				mButton3->setVisible(true);
				mButton3->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton3->getWidth() - offset, (mButton3->getHeight() + offset) * 2 + offset);
				MyGUI::ControllerManager::getInstance().addItem(mButton3, createControllerPosition(point));
			}
		}

		if (mCountTime > time_diff * 3)
		{
			if (!mButton4->getVisible())
			{
				mButton4->setPosition(view.width, (mButton4->getHeight() + offset) * 3 + offset);
				mButton4->setVisible(true);
				mButton4->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton4->getWidth() - offset, (mButton4->getHeight() + offset) * 3 + offset);
				MyGUI::ControllerPosition* controller = createControllerPosition(point);
				MyGUI::ControllerManager::getInstance().addItem(mButton4, controller);
				controller->eventPostAction += MyGUI::newDelegate(this, &State::notifyPostAction);
			}

			FrameAdvise(false);
		}
	}

	void State::notifyPostAction(MyGUI::Widget* _sender, MyGUI::ControllerItem* _controller)
	{
		eventButtonPress(ControllerType::Slowed, true);
	}

	void State::FrameAdvise(bool _advise)
	{
		if (_advise)
		{
			if (!mFrameAdvise)
			{
				mFrameAdvise = true;
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &State::notifyFrameEvent);
			}
		}
		else
		{
			if (mFrameAdvise)
			{
				mFrameAdvise = false;
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &State::notifyFrameEvent);
			}
		}
	}

	MyGUI::ControllerPosition* State::createControllerPosition(const MyGUI::IntPoint& _point)
	{
		MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerPosition::getClassTypeName());
		MyGUI::ControllerPosition* controller = item->castType<MyGUI::ControllerPosition>();

		controller->setPosition(_point);
		controller->setTime(0.5);

		if (mType == ControllerType::Inertional) controller->setAction(MyGUI::newDelegate(MyGUI::action::inertionalMoveFunction));
		else if (mType == ControllerType::Accelerated) controller->setAction(MyGUI::newDelegate(MyGUI::action::acceleratedMoveFunction<30>));
		else if (mType == ControllerType::Slowed) controller->setAction(MyGUI::newDelegate(MyGUI::action::acceleratedMoveFunction<4>));
		else controller->setAction(MyGUI::newDelegate(MyGUI::action::jumpMoveFunction<5>));

		return controller;
	}

	MyGUI::ControllerFadeAlpha* State::createControllerFadeAlpha(float _alpha, float _coef, bool _enable)
	{
		MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerFadeAlpha::getClassTypeName());
		MyGUI::ControllerFadeAlpha* controller = item->castType<MyGUI::ControllerFadeAlpha>();

		controller->setAlpha(_alpha);
		controller->setCoef(_coef);
		controller->setEnabled(_enable);

		return controller;
	}

	MyGUI::Widget* State::getClient()
	{
		return mMainWidget->getClientWidget();
	}

} // namespace demo
