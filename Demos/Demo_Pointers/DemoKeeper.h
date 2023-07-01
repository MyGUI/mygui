/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "Base/BaseDemoManager.h"
#include "EnemyPanel.h"
#include "FriendPanel.h"
#include "ControlPanel.h"
#include "PointerContextManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager, public IPointerSetter
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void setupResources() override;

		void injectMouseMove(int _absx, int _absy, int _absz) override;
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;

		void setPointer(std::string_view _name) override;

		void createEntities();
		void destroyEntities();

		void updateCamera(int _x, int _y);
		std::string_view getCursorFromScene(int _x, int _y);

	private:
		EnemyPanel* mEnemyPanel{nullptr};
		FriendPanel* mFriendPanel{nullptr};
		ControlPanel* mControlPanel{nullptr};
		PointerContextManager* mPointerContextManager{nullptr};
		bool mRightButtonPressed{false};
		int mSaveCursorX{0};
		int mSaveCursorY{0};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
