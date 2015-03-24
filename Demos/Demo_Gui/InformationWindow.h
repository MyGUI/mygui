/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef INFORMATION_WINDOW_H_
#define INFORMATION_WINDOW_H_

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

#endif // INFORMATION_WINDOW_H_
