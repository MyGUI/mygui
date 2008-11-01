/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __ITEM_BOX_WINDOW_H__
#define __ITEM_BOX_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	class ItemBoxWindow : public wraps::BaseLayout
	{
	public:
		ItemBoxWindow();

		virtual void initialise();

	};

} // namespace demo

#endif // __ITEM_BOX_WINDOW_H__
