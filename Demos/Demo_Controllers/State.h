/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __MENU_1_H__
#define __MENU_1_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	struct ControllerType
	{
		enum EnumType { Jump, Accelerated, Slowed, Inertional, MAX };
		ControllerType(EnumType _value = MAX) : value(_value) { }
		friend bool operator == (ControllerType const & a, ControllerType const & b) { return a.value == b.value; }
		friend bool operator != (ControllerType const & a, ControllerType const & b) { return a.value != b.value; }
	private:
		EnumType value;
	};

	class State : public wraps::BaseLayout
	{

	public:
		State(const std::string& _layout, ControllerType _type);
		virtual ~State();

		MyGUI::WidgetPtr getClient() { return mMainWidget->getClientWidget(); }

		void setVisible(bool _visible);

		MyGUI::delegates::CDelegate1<ControllerType> eventButtonPress;

	private:
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyFrameEvent(float _time);
		MyGUI::ControllerPosition * getController(const MyGUI::IntPoint & _point);

		void FrameAdvise(bool _advise)
		{
			if (_advise) {
				if (!mFrameAdvise) {
					mFrameAdvise = true;
					MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &State::notifyFrameEvent);
				}
			}
			else {
				if (mFrameAdvise) {
					mFrameAdvise = false;
					MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &State::notifyFrameEvent);
				}
			}
		}

	private:
		bool mFrameAdvise;
		MyGUI::ButtonPtr mButton1;
		MyGUI::ButtonPtr mButton2;
		MyGUI::ButtonPtr mButton3;
		MyGUI::ButtonPtr mButton4;
		float mCountTime;
		ControllerType mType;
	};

} // namespace demo

#endif // __MENU_1_H__
