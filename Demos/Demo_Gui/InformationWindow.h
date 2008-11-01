/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __INFORMATION_WINDOW_H__
#define __INFORMATION_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	class InformationWindow : public wraps::BaseLayout
	{
	public:
		InformationWindow();

		virtual void initialise();

	};

} // namespace demo

#endif // __INFORMATION_WINDOW_H__
