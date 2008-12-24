/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#include "Menu1.h"

namespace demo
{

	Menu1::Menu1() :
		mFrameAdvise(false)
	{
	}

	void Menu1::initialise(const std::string& _layout, ControllerType _type)
	{
		mType = _type;
		mLayoutName = _layout;
		loadLayout();

		assignWidget(mButton1, "Button1");
		assignWidget(mButton2, "Button2");
		assignWidget(mButton3, "Button3");
		assignWidget(mButton4, "Button4");

		mMainWidget->hide();
		mButton1->hide();
		mButton2->hide();
		mButton3->hide();
		mButton4->hide();

		mButton1->eventMouseButtonClick = MyGUI::newDelegate(this, &Menu1::notifyMouseButtonClick);
		mButton2->eventMouseButtonClick = MyGUI::newDelegate(this, &Menu1::notifyMouseButtonClick);
		mButton3->eventMouseButtonClick = MyGUI::newDelegate(this, &Menu1::notifyMouseButtonClick);
		mButton4->eventMouseButtonClick = MyGUI::newDelegate(this, &Menu1::notifyMouseButtonClick);
	}

	void Menu1::show()
	{
		mMainWidget->hide();
		mButton1->hide();
		mButton2->hide();
		mButton3->hide();
		mButton4->hide();

		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mButton1);
		MyGUI::LayerManager::getInstance().upLayerItem(mButton2);
		MyGUI::LayerManager::getInstance().upLayerItem(mButton3);
		MyGUI::LayerManager::getInstance().upLayerItem(mButton4);

		FrameAdvise(true);
		mCountTime = 0;

	}

	void Menu1::hide()
	{
		MyGUI::ControllerFadeAlpha * controller = new MyGUI::ControllerFadeAlpha(0, 3, true);
		MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);

		controller = new MyGUI::ControllerFadeAlpha(0, 3, true);
		MyGUI::ControllerManager::getInstance().addItem(mButton1, controller);
		controller = new MyGUI::ControllerFadeAlpha(0, 3, true);
		MyGUI::ControllerManager::getInstance().addItem(mButton2, controller);
		controller = new MyGUI::ControllerFadeAlpha(0, 3, true);
		MyGUI::ControllerManager::getInstance().addItem(mButton3, controller);
		controller = new MyGUI::ControllerFadeAlpha(0, 3, true);
		MyGUI::ControllerManager::getInstance().addItem(mButton4, controller);
	}

	void Menu1::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		if (_sender == mButton1) eventButtonPress(ControllerType::Inertional);
		else if (_sender == mButton2) eventButtonPress(ControllerType::Accelerated);
		else if (_sender == mButton3) eventButtonPress(ControllerType::Slowed);
		else if (_sender == mButton4) eventButtonPress(ControllerType::Jamp);
	}

	MyGUI::ControllerPosition * Menu1::getController(const MyGUI::IntPoint & _point)
	{
		const float time_anim = 0.5;
		if (mType == ControllerType::Inertional) return new MyGUI::ControllerPosition(_point, time_anim, MyGUI::ControllerPosition::Inertional);
		else if (mType == ControllerType::Accelerated) return new MyGUI::ControllerPosition(_point, time_anim, MyGUI::ControllerPosition::Accelerated);
		else if (mType == ControllerType::Slowed) return new MyGUI::ControllerPosition(_point, time_anim, MyGUI::ControllerPosition::Slowed);
		return new MyGUI::ControllerPosition(_point, time_anim, MyGUI::ControllerPosition::Jump);
	}

	void Menu1::notifyFrameEvent(float _time)
	{
		mCountTime += _time;

		const int offset = 30;
		const int offset_jamp = 1;
		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const float time_diff = 0.3;

		if (!mMainWidget->isShow()) {
			mMainWidget->setPosition(-mMainWidget->getWidth(), view.height - mMainWidget->getHeight() - offset);
			mMainWidget->show();
			mMainWidget->setAlpha(1);

			MyGUI::IntPoint  point(offset, view.height - mMainWidget->getHeight() - offset);
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, getController(point));
		}

		if (!mButton1->isShow()) {
			mButton1->setPosition(view.width, offset);
			mButton1->show();
			mButton1->setAlpha(1);
			MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, offset);
			MyGUI::ControllerManager::getInstance().addItem(mButton1, getController(point));
		}

		if (mCountTime > time_diff) {
			if (!mButton2->isShow()) {
				mButton2->setPosition(view.width, (mButton2->getHeight() + offset) + offset);
				mButton2->show();
				mButton2->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, (mButton2->getHeight() + offset) + offset);
				MyGUI::ControllerManager::getInstance().addItem(mButton2, getController(point));
			}
		}

		if (mCountTime > time_diff*2) {
			if (!mButton3->isShow()) {
				mButton3->setPosition(view.width, (mButton3->getHeight() + offset) * 2 + offset);
				mButton3->show();
				mButton3->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton3->getWidth() - offset, (mButton3->getHeight() + offset) * 2 + offset);
				MyGUI::ControllerManager::getInstance().addItem(mButton3, getController(point));
			}
		}

		if (mCountTime > time_diff * 3) {
			if (!mButton4->isShow()) {
				mButton4->setPosition(view.width, (mButton4->getHeight() + offset) * 3 + offset);
				mButton4->show();
				mButton4->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton4->getWidth() - offset, (mButton4->getHeight() + offset) * 3 + offset);
				MyGUI::ControllerManager::getInstance().addItem(mButton4, getController(point));
			}

			FrameAdvise(false);
		}

	}

} // namespace demo
