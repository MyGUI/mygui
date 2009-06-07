/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __INFORMATION_WINDOW_H__
#define __INFORMATION_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class InformationWindow : public wraps::BaseLayout
	{
	public:
		InformationWindow(MyGUI::WidgetPtr _parent);
		virtual ~InformationWindow();

	private:
		void notifyFrameStart(float _time);
		int getRand(int _min, int _max);

	private:
		MyGUI::EditPtr mInfo;
		MyGUI::WidgetPtr mFocus;
	};

} // namespace demo

#endif // __INFORMATION_WINDOW_H__
