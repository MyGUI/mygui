/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef __FRIEND_PANEL_H__
#define __FRIEND_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(FriendPanel, "FriendPanel.layout");
	class FriendPanel :
		public wraps::BaseLayout
	{
	public:
		FriendPanel();

		bool isIntersect(int _x, int _y);
		void setVisible(bool _value);
	};

} // namespace demo

#endif // __FRIEND_PANEL_H__
