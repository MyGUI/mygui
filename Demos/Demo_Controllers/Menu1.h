/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __MENU_1_H__
#define __MENU_1_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	struct ControllerType
	{
		enum EnumType { Jump, Accelerated, Slowed, Inertional, MAX };
		ControllerType(EnumType _value = MAX) : value(_value) { }
		friend bool operator == (ControllerType const & a, ControllerType const & b) { return a.value == b.value; }
		friend bool operator != (ControllerType const & a, ControllerType const & b) { return a.value != b.value; }
	private:
		int value;
	};

	class Menu1 : public wraps::BaseLayout
	{

	public:
		Menu1();
		void initialise(const std::string& _layout, ControllerType _type);
		virtual void shutdown() { FrameAdvise(false); }

		MyGUI::WidgetPtr getClient() { return mainWidget()->getClientWidget(); }

		void show();
		void hide();

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
					MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &Menu1::notifyFrameEvent);
				}
			}
			else {
				if (mFrameAdvise) {
					mFrameAdvise = false;
					MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &Menu1::notifyFrameEvent);
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
