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
#include "PointerManager.h"

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

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

	private:
		EnemyPanel* mEnemyPanel;
		FriendPanel* mFriendPanel;
		ControlPanel* mControlPanel;
		PointerManager* mPointerManager;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
