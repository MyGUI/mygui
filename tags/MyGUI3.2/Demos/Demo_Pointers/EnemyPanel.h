/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef __ENEMY_PANEL_H__
#define __ENEMY_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(EnemyPanel, "EnemyPanel.layout");
	class EnemyPanel :
		public wraps::BaseLayout
	{
	public:
		EnemyPanel();

		bool isIntersect(int _x, int _y);
		void setVisible(bool _value);
	};

} // namespace demo

#endif // __ENEMY_PANEL_H__
