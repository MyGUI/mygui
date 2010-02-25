/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "BaseManager.h"
#include "EnemyPanel.h"
#include "FriendPanel.h"
#include "ControlPanel.h"
#include "PointerContextManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager,
		public IPointerSetter
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

		virtual void setPointer(const std::string& _name);

		void createEntities();
		void destroyEntities();

		void updateCamera(int _x, int _y);
		std::string getCursorFromScene(int _x, int _y);

	private:
		EnemyPanel* mEnemyPanel;
		FriendPanel* mFriendPanel;
		ControlPanel* mControlPanel;
		PointerContextManager* mPointerContextManager;
		bool mRightButtonPressed;
		int mSaveCursorX;
		int mSaveCursorY;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
