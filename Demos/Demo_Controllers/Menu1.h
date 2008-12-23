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

	class Menu1 : public wraps::BaseLayout
	{
	public:
		Menu1();
		void initialise(const std::string& _layout);
		virtual void shutdown() { FrameAdvise(false); }

		MyGUI::WidgetPtr getClient() { return mainWidget()->getClientWidget(); }

		void show();
		void hide();

		MyGUI::delegates::CDelegate1<size_t> eventButtonPress;

	private:
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyFrameEvent(float _time);

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

	};

} // namespace demo

#endif // __MENU_1_H__
