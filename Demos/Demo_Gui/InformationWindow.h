/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __INFORMATION_WINDOW_H__
#define __INFORMATION_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class InformationWindow :
		public wraps::BaseLayout
	{
	public:
		InformationWindow(MyGUI::Widget* _parent);
		virtual ~InformationWindow();

	private:
		void notifyFrameStart(float _time);
		int getRand(int _min, int _max);

	private:
		MyGUI::EditBox* mInfo;
		MyGUI::Widget* mFocus;
	};

} // namespace demo

#endif // __INFORMATION_WINDOW_H__
