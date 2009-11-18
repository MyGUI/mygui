/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "Base/BaseManager.h"
#include "EnemyPanel.h"
#include "FriendPanel.h"
#include "ControlPanel.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

	private:
		EnemyPanel* mEnemyPanel;
		FriendPanel* mFriendPanel;
		ControlPanel* mControlPanel;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
