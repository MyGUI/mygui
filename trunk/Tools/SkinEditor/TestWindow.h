/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __TEST_WINDOW_H__
#define __TEST_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "Dialog.h"

namespace tools
{

	class TestWindow :
		public wraps::BaseLayout,
		public Dialog
	{
	public:
		TestWindow();
		virtual ~TestWindow();

		virtual void setVisible(bool _value);
		virtual bool getVisible();

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

	};

} // namespace tools

#endif // __TEST_WINDOW_H__
