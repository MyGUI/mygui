/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#include "Menu1.h"

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

	Menu1::Menu1():
	mFrameAdvise(false)
	{
	}

	void Menu1::initialise(const std::string& _layout)
	{
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
		if (_sender == mButton1) eventButtonPress(0);
		else if (_sender == mButton2) eventButtonPress(1);
		else if (_sender == mButton3) eventButtonPress(2);
		else if (_sender == mButton4) eventButtonPress(3);
	}

	void Menu1::notifyFrameEvent(float _time)
	{
		mCountTime += _time;

		const int offset = 30;
		const int offset_jamp = 1;
		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const float time_diff = 0.3;
		const float time_anim = 0.5;

		if (!mMainWidget->isShow()) {
			mMainWidget->setPosition(-mMainWidget->getWidth(), view.height - mMainWidget->getHeight() - offset);
			mMainWidget->show();
			mMainWidget->setAlpha(1);

			MyGUI::IntPoint  point(offset, view.height - mMainWidget->getHeight() - offset);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(point, time_anim, MyGUI::newDelegate(JumpFunction<offset_jamp>));
			MyGUI::ControllerManager::getInstance().addItem(mMainWidget, controller);
		}

		if (!mButton1->isShow()) {
			mButton1->setPosition(view.width, offset);
			mButton1->show();
			mButton1->setAlpha(1);
			MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, offset);
			MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(point, time_anim, MyGUI::newDelegate(JumpFunction<offset_jamp>));
			MyGUI::ControllerManager::getInstance().addItem(mButton1, controller);
		}

		if (mCountTime > time_diff) {
			if (!mButton2->isShow()) {
				mButton2->setPosition(view.width, (mButton2->getHeight() + offset) + offset);
				mButton2->show();
				mButton2->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton1->getWidth() - offset, (mButton2->getHeight() + offset) + offset);
				MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(point, time_anim, MyGUI::newDelegate(JumpFunction<offset_jamp>));
				MyGUI::ControllerManager::getInstance().addItem(mButton2, controller);
			}
		}

		if (mCountTime > time_diff*2) {
			if (!mButton3->isShow()) {
				mButton3->setPosition(view.width, (mButton3->getHeight() + offset) * 2 + offset);
				mButton3->show();
				mButton3->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton3->getWidth() - offset, (mButton3->getHeight() + offset) * 2 + offset);
				MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(point, time_anim, MyGUI::newDelegate(JumpFunction<offset_jamp>));
				MyGUI::ControllerManager::getInstance().addItem(mButton3, controller);
			}
		}

		if (mCountTime > time_diff * 3) {
			if (!mButton4->isShow()) {
				mButton4->setPosition(view.width, (mButton4->getHeight() + offset) * 3 + offset);
				mButton4->show();
				mButton4->setAlpha(1);
				MyGUI::IntPoint point(view.width - mButton4->getWidth() - offset, (mButton4->getHeight() + offset) * 3 + offset);
				MyGUI::ControllerPosition * controller = new MyGUI::ControllerPosition(point, time_anim, MyGUI::newDelegate(JumpFunction<offset_jamp>));
				MyGUI::ControllerManager::getInstance().addItem(mButton4, controller);
			}

			FrameAdvise(false);
		}

	}

} // namespace demo
