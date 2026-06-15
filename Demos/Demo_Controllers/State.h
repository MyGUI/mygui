/*!
	@file
	@author		Albert Semenov
	@date		12/2008
*/
#ifndef MENU_1_H_
#define MENU_1_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	struct ControllerType
	{
		enum EnumType
		{
			Jump,
			Accelerated,
			Slowed,
			Inertional,
			MAX
		};

		ControllerType(EnumType _value = MAX) :
			value(_value)
		{
		}

		friend bool operator==(ControllerType const& a, ControllerType const& b)
		{
			return a.value == b.value;
		}

		friend bool operator!=(ControllerType const& a, ControllerType const& b)
		{
			return a.value != b.value;
		}

	private:
		EnumType value;
	};

	class State : public wraps::BaseLayout
	{
	public:
		State(std::string_view _layout, ControllerType _type);
		~State() override;

		MyGUI::Widget* getClient();

		void setVisible(bool _visible);

		MyGUI::delegates::Delegate<ControllerType, bool> eventButtonPress;

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyFrameEvent(float _time);
		void notifyPostAction(MyGUI::Widget* _sender, MyGUI::ControllerItem* _controller) const;

		MyGUI::ControllerPosition* createControllerPosition(const MyGUI::IntPoint& _point);
		MyGUI::ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);

		void FrameAdvise(bool _advise);

	private:
		bool mFrameAdvise{false};
		MyGUI::Button* mButton1;
		MyGUI::Button* mButton2;
		MyGUI::Button* mButton3;
		MyGUI::Button* mButton4;
		float mCountTime;
		ControllerType mType;
	};

} // namespace demo

#endif // MENU_1_H_
